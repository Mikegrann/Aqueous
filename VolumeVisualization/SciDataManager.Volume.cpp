#include "SciDataManager.h"


void SciDataManager::createVolumeFromGridValues(IColorMapper * ColorMapper)
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Values.size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return;
	}

	ColorMapper->preProcessValues(GridValues);

	GLubyte * volumeData = new GLubyte[size];

	for (int i = 0; i < GridDimensions[2]; ++ i)
	{
		for (int j = 0; j < GridDimensions[1]; ++ j)
		{
			for (int k = 0; k < GridDimensions[0]; ++ k)
			{
				int index = k + (GridDimensions[2] - i - 1) * GridDimensions[0] + j * GridDimensions[2] * GridDimensions[0];

				int ValueIndex = k + j * GridDimensions[0] + i * GridDimensions[1] * GridDimensions[0];

				SColorAf Color = ColorMapper->getColor(GridValues.Values[ValueIndex]);

				for (int t = 0; t < 4; ++ t)
					volumeData[index * 4 + t] = clamp<unsigned char>((unsigned char) (Color[t] * 255.f), 0, 255);
			}
		}
	}


	glEnable(GL_TEXTURE_3D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (! VolumeHandle)
		glGenTextures(1, & VolumeHandle);
	glBindTexture(GL_TEXTURE_3D, VolumeHandle);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, GridDimensions[0], GridDimensions[2], GridDimensions[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, volumeData);
	
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	delete []volumeData;
}

f64 const SciDataManager::getGridVolume(std::string const & Field, f64 const Value, f64 const Range, int const Mode) const
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Values.size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return 0.0;
	}

	f64 TotalSum = 0;

	if (Mode == 0 || Mode == 2)
	{
		SciData const **** LocalGrid = new SciData const ***[2];
		double *** IsoValues = new double **[2];

		for (int a = 0; a < 2; ++ a)
		{
			LocalGrid[a] = new SciData const **[2];
			IsoValues[a] = new double *[2];
			for (int b = 0; b < 2; ++ b)
			{
				LocalGrid[a][b] = new SciData const *[2];
				IsoValues[a][b] = new double[2];
			}
		}

		for (int i = 0; i < GridDimensions[2] - 1; ++ i)
		{
			for (int j = 0; j < GridDimensions[1] - 1; ++ j)
			{
				for (int k = 0; k < GridDimensions[0] - 1; ++ k)
				{
					u8 TruthTable = 0;

					for (int a = 0; a < 2; ++ a)
					{
						for (int b = 0; b < 2; ++ b)
						{
							for (int c = 0; c < 2; ++ c)
							{
								int ValueIndex = (k + c) + (j + b) * GridDimensions[0] + (i + a) * GridDimensions[1] * GridDimensions[0];
								int FieldIndex = 4 * c + 2 * b + a;
								LocalGrid[a][b][c] = & GridValues.Values[ValueIndex];
								IsoValues[a][b][c] = Range - abs(LocalGrid[a][b][c]->getField(Field) - Value);
								if (IsoValues[a][b][c] >= 0.0)
									TruthTable |= 1 << FieldIndex;
							}
						}
					}

					
					if (TruthTable == 0x00)
						TotalSum += 0.0;
					else if (TruthTable == 0xFF)
						TotalSum += 1.0;
					else
					{
						if (Mode == 0)
						{
							int BitCount = 0;
							for (int t = 0; t < 8; ++ t)
								if (TruthTable & 1 << t)
									BitCount ++;
							if (BitCount == 0 || BitCount == 8)
							{
								// Unexpected...
								std::cerr << "Unexpected but recoverable state: unknown truth table value in grid volume calculation." << std::endl;
								TotalSum += BitCount ? 1.0 : 0.0;
							}
							else
							{
								TotalSum += (1.0 / 8.0) * BitCount;
							}
						}
						else if (Mode == 2)
						{
							for (int a = 0; a < 2; ++ a)
							{
								for (int b = 0; b < 2; ++ b)
								{
									for (int c = 0; c < 2; ++ c)
									{
										double IsoValue = IsoValues[a][b][c];
										if (IsoValue < 0.0)
											continue;
										
										vec3i Current(a, b, c);
										vec3d Scale;
										for (int i = 0; i < 3; ++ i)
										{
											vec3i Adjacent = Current;
											Adjacent[i] = Adjacent[i] ? 0 : 1;

											f64 const OtherIsoValue = IsoValues[Adjacent.X][Adjacent.Y][Adjacent.Z];
											if (OtherIsoValue >= 0.0)
												Scale[i] = 0.5;
											else
											{
												double ratio = IsoValue / abs(OtherIsoValue - IsoValue);
												Scale[i] = ratio;
											}
										}
										TotalSum += Scale.X * Scale.Y * Scale.Z;
									} // for c
								} // for b
							} // for a
						} // else if (Mode == 2)
					} // else
				}
			}
		}

		for (int a = 0; a < 2; ++ a)
		{
			for (int b = 0; b < 2; ++ b)
			{
				delete [] LocalGrid[a][b];
				delete [] IsoValues[a][b];
			}
			delete [] LocalGrid[a];
			delete [] IsoValues[a];
		}
		delete [] LocalGrid;
		delete [] IsoValues;
	}
	else if (Mode == 1)
	{
		for (int i = 0; i < GridDimensions[2]; ++ i)
		{
			for (int j = 0; j < GridDimensions[1]; ++ j)
			{
				for (int k = 0; k < GridDimensions[0]; ++ k)
				{
					int ValueIndex = (k + 0) + (j + 0) * GridDimensions[0] + (i + 0) * GridDimensions[1] * GridDimensions[0];
					SciData const * Data = & GridValues.Values[ValueIndex];
					f64 const IsoValue = Range - abs(Data->getField(Field) - Value);

					if (IsoValue < 0.0)
						continue; //{printf("%f\t", Data->getField(Field)); continue;}
					
					vec3d Scale;

					for (int a = -1; a <= 1; ++ a)
					{
						for (int b = -1; b <= 1; ++ b)
						{
							for (int c = -1; c <= 1; ++ c)
							{
								int ZeroCount = 0;
								int AxisIndex = -1;
								ZeroCount += a ? (AxisIndex = 0, 0) : 1;
								ZeroCount += b ? (AxisIndex = 1, 0) : 1;
								ZeroCount += c ? (AxisIndex = 2, 0) : 1;

								if (ZeroCount == 2)
								{
									
									if ((k + a) < 0 || (k + a) >= GridDimensions[0] ||
										(j + b) < 0 || (j + b) >= GridDimensions[1] ||
										(i + c) < 0 || (i + c) >= GridDimensions[2])
									{
										Scale[AxisIndex] += 0.5;
										continue;
									}

									int ValueIndex = (k + a) + (j + b) * GridDimensions[0] + (i + c) * GridDimensions[1] * GridDimensions[0];
									SciData const * OtherData = & GridValues.Values[ValueIndex];
									f64 const OtherIsoValue = Range - abs(OtherData->getField(Field) - Value);

									if (OtherIsoValue >= 0.0)
										Scale[AxisIndex] += 0.5;
									else
									{
										double ratio = IsoValue / abs(OtherIsoValue - IsoValue);
										//if (ratio < 0.0)
										//	ratio += 1.0;
										Scale[AxisIndex] += ratio;
										//printf("ratio %f\t", ratio);
									}
								}
							}
						}
					}

					TotalSum += Scale.X * Scale.Y * Scale.Z;
				}
			}
		}
	}

	return TotalSum ;//* 20 * 20;
}

SColorf const ratioToSpectrum(float Ratio)
{
	if (Ratio >= 1.f)
		Ratio = 0.99999f;
	else if (Ratio < 0.f)
		Ratio = 0.f;
	Ratio *= 100.f;
	Ratio /= (100.f / 6.f);
	int i = (int) Ratio;

	float v = 1.f;
	float s = 1.f;
	float ff = Ratio - i;
	float p = v * (1.f - s);
    float q = v * (1.f - (s * ff));
    float t = v * (1.f - (s * (1.f - ff)));

	SColorf out;

	switch(i)
	{
	case 0:
		out.Red = v;
		out.Green = t;
		out.Blue = p;
		break;
	case 1:
		out.Red = q;
		out.Green = v;
		out.Blue = p;
		break;
	case 2:
		out.Red = p;
		out.Green = v;
		out.Blue = t;
		break;
	case 3:
		out.Red = p;
		out.Green = q;
		out.Blue = v;
		break;
	case 4:
		out.Red = t;
		out.Green = p;
		out.Blue = v;
		break;
	case 5:
	default:
		out.Red = v;
		out.Green = 1.f - q;
		out.Blue = 1.f;
		break;
    }

	return out;
}

#include "CProgramContext.h"
#include "CVolumeSceneObject.h"
#include "ProgressPrinter.h"

void SciDataManager::produceVolumeMaps()
{
	ProgressPrinter p;
	p.begin();

	Range ValueRange = GridValues.getValueRange("o1", 5.0);
	CVolumeSceneObject const * const VolumeObject = CProgramContext::get().Scene.VolumeSceneObject;
	CVolumeSceneObject::SControl const & VolumeControl = VolumeObject->Control;

	for (int i = 2; i < 3; ++ i)
	{
		u32 const ImageSize = 256;
		u8 * const ImageData = new u8[ImageSize * ImageSize * 3];

		for (u32 y = 0; y < ImageSize; ++ y)
		{
			for (u32 x = 0; x < ImageSize; ++ x)
			{
				float const yR = (float) y / (float) (ImageSize - 1);
				float const xR = (float) x / (float) (ImageSize - 1);
				u32 const Index = x * 3 + y * ImageSize * 3;

				float const EmphasisLocation = xR;
				float const LocalRange = yR;

				f64 const GridVolume = getGridVolume("o1", 
					EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first, 
					LocalRange / 2.f * (ValueRange.second - ValueRange.first), 
					i);

				f32 const VolumeRatio = (f32) GridVolume / (28.f * 23.f * 14.f);

				SColori Color = ratioToSpectrum((f32) VolumeRatio);
				
				ImageData[Index + 0] = Color.Red;
				ImageData[Index + 1] = Color.Green;
				ImageData[Index + 2] = Color.Blue;
			}
			
			p.update(y * 33 / ImageSize + (i ? (i == 2 ? 66 : 33) : 0));
		}

		CImage * Image = new CImage(ImageData, ImageSize, ImageSize, false);
		std::stringstream s;
		s << "VolumeMapMode" << i << ".bmp";
		Image->write(s.str());

		delete Image;
	}

	p.end();
	std::cout << ("Volume maps written!") << std::endl;
}
