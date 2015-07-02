

#include "GifWriter.h"

#include "gif_lib.h"


GifWriter::GifWriter(vec2i const & Dimensions)
	: Size(Dimensions), OutputPalette()
{}

bool GifWriter::AddFrame(u8 const * const data, float deltaTime)
{
	u32 const PixelCount = Size.X * Size.Y;
	s32 PaletteSize = 256;
	Frame output(PixelCount);


	if (Frames.size()==0)
	{
		std::vector<u8> r(PixelCount), g(PixelCount), b(PixelCount);

		for (u32 i = 0, j = 0; i< PixelCount; ++ i)
		{
			r[i] = data[j ++];
			g[i] = data[j ++];
			b[i] = data[j ++];
		}

		OutputPalette = GifMakeMapObject(PaletteSize, NULL);

		if (! OutputPalette)
		{
			std::cerr << "Could not create color palette" << std::endl;
			return false;
		}

		if (GifQuantizeBuffer(Size.X, Size.Y, & PaletteSize, & (r[0]), & (g[0]), & (b[0]), & (output.Data[0]), OutputPalette->Colors) == GIF_ERROR)
		{
			std::cerr << "Could not quantize color buffer" << std::endl;
			return false;
		}
	}
	else
	{
		for (u32 i = 0, j=0; i < PixelCount; i++) {
			int minIndex = 0,
				minDist = 3 * 256 * 256;
			GifColorType *c = OutputPalette->Colors;

			/* Find closest color in first color map to this color. */
			for (int k = 0; k < OutputPalette->ColorCount; k++)
			{
				int dr = (int(c[k].Red) - data[j] ) ;
				int dg = (int(c[k].Green) - data[j+1] ) ;
				int db = (int(c[k].Blue) - data[j+2] ) ;

				int dist=dr*dr+dg*dg+db*db;

				if (minDist > dist) {
					minDist  = dist;
					minIndex = k;
				}
			}
			j+=3;
			output.Data[i] = minIndex;
		}

	}

	output.Delay = deltaTime * 100;
	Frames.push_back(output);
	return true;       
}

static bool AddLoop(GifFileType *gf)
{
	int loop_count;
	loop_count=0;
	{
		char nsle[12] = "NETSCAPE2.0";
		char subblock[3];
		EGifPutExtensionLeader(gf, APPLICATION_EXT_FUNC_CODE);
		if (EGifPutExtensionBlock(gf, 11, nsle) == GIF_ERROR)
		{
			std::cerr << "Failed to add extension 11" << std::endl;
			WaitForUser();
			return false;
		}
		subblock[0] = 1;
		subblock[2] = loop_count % 256;
		subblock[1] = loop_count / 256;
		if (EGifPutExtensionBlock(gf, 3, subblock) == GIF_ERROR)
		{
			std::cerr << "Failed to add extension 3" << std::endl;
			WaitForUser();
			return false;
		}
		EGifPutExtensionTrailer(gf);

	}
	return true;
}

bool GifWriter::Save(std::string const & fileName)
{
	if (Frames.size() == 0)
	{
		std::cerr << "No frames in GIF" << std::endl;
		return false;
	}

	GifFileType * GifFile = EGifOpenFileName(fileName.c_str(), FALSE, FALSE);

	if (! GifFile)
	{
		std::cerr << "Could not create file for writing" << std::endl;
		return false;
	}

	if (EGifPutScreenDesc(GifFile, Size.X, Size.Y, 8, 0, OutputPalette) == GIF_ERROR)
	{
		std::cerr << "Failed to write header" << std::endl;
		return false;
	}

	if (! AddLoop(GifFile))
	{
		std::cerr << "Failed to enable looping" << std::endl;
		return false;
	}

	for (u32 ni = 0; ni < Frames.size(); ni++)
	{      
		static unsigned char
			ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };


		ExtStr[0] = (false) ? 0x06 : 0x04;
		ExtStr[1] = Frames[ni].Delay % 256;
		ExtStr[2] = Frames[ni].Delay / 256;

		/* Dump graphics control block. */
		EGifPutExtension(GifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr);


		if (EGifPutImageDesc(GifFile, 0, 0, Size.X, Size.Y, FALSE, NULL) == GIF_ERROR)
		{
			std::cerr << "Failed to write frame info " << ni << std::endl;
			return false;
		}


		for (int y = 0, j=(Size.Y-1)*Size.X; y < Size.Y; y++, j-=Size.X)
		{
			if (EGifPutLine(GifFile, &(Frames[ni].Data[j]), Size.X) == GIF_ERROR)
			{
				std::cerr << "Failed to write frame " << ni << std::endl;
				return false;
			}
		}
	}

	int errorcode;
	if (EGifCloseFile(GifFile, &errorcode) == GIF_ERROR)
	{
		std::cerr << "Failed to close GIF file" << std::endl;
		return false;
	}

	return true;       
}
