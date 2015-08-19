#include "CSite.h"
#include "CWorldTime.h"

#include "CLocation.h"

#include "rapidjson/document.h"


CSite::CSite(string const & Name)
{
	SingletonPointer<CProgramContext> Context;

	this->Name = Name;
	this->Path = Context->SitePath + "/" + Name + "/";
}

void CSite::ReadConfiguration()
{
	rapidjson::Document d;
	d.Parse(File::ReadAsString(Path + "/Site.json").c_str());
	string Error;

	if (d.HasMember("DataSets"))
	{
		auto & dDataSets = d["DataSets"];
		if (dDataSets.IsArray())
		{
			for (uint i = 0; i < dDataSets.Size(); ++ i)
			{
				auto & dDataSet = dDataSets[i];
				CDataSet * DataSet = new CDataSet(this);
				DataSet->Traits.PositionXField = dDataSet["PositionXField"].GetString();
				DataSet->Traits.PositionYField = dDataSet["PositionYField"].GetString();
				DataSet->Traits.PositionZField = dDataSet["PositionZField"].GetString();
				
				if (dDataSet.HasMember("MapDepth") && dDataSet["MapDepth"].IsDouble())
					DataSet->MapDepth = dDataSet["MapDepth"].GetDouble();
				
				if (dDataSet.HasMember("YExaggeration") && dDataSet["YExaggeration"].IsDouble())
					DataSet->YExaggeration = dDataSet["YExaggeration"].GetDouble();
				
				if (dDataSet.HasMember("GlyphColorMapper") && dDataSet["GlyphColorMapper"].IsString())
					DataSet->GlyphColorMapper = dDataSet["GlyphColorMapper"].GetString();
				
				if (dDataSet.HasMember("VolumeColorMapper") && dDataSet["VolumeColorMapper"].IsString())
					DataSet->VolumeColorMapper = dDataSet["VolumeColorMapper"].GetString();
				
				if (dDataSet.HasMember("ColorField") && dDataSet["ColorField"].IsArray()) {
					auto & fields = dDataSet["ColorField"];
					for (uint j = 0; j < fields.Size(); ++j)
					{
						auto & field = fields[j];
						if (field.HasMember("FieldName") && field["FieldName"].IsString()) {
							DataSet->dataFields.push_back(field["FieldName"].GetString());
						}
					}
					if (DataSet->dataFields.size()) {
						DataSet->ColorField = DataSet->dataFields[0];
					}
				}

				if (dDataSet.HasMember("TField") && dDataSet["TField"].IsString())
					DataSet->Traits.TField = dDataSet["TField"].GetString();

				if (dDataSet.HasMember("DataLonLatCenter") && dDataSet["DataLonLatCenter"].IsObject())
				{
					SLongitudeLatituded DataLonLatCenter;
					DataLonLatCenter.Longitude = SLongitudeLatituded::DMStoDecimal(dDataSet["DataLonLatCenter"]["Longitude"].GetString());
					DataLonLatCenter.Latitude = SLongitudeLatituded::DMStoDecimal(dDataSet["DataLonLatCenter"]["Latitude"].GetString());
					DataSet->DataLonLatCenter = DataLonLatCenter;
					DataSet->ManuallySetDataLongLat = true;
				}

				if (dDataSet.HasMember("Assets"))
				{
					auto & dAssets = dDataSet["Assets"];
					for (uint j = 0; j < dAssets.Size(); ++ j)
					{
						auto & dAsset = dAssets[j];
						CDataSet::SAsset Asset;

						if (dAsset.HasMember("File") && dAsset["File"].IsString())
							Asset.File = dAsset["File"].GetString();
						else
							cerr << "Failed to read site configuration file: " << "File member is not a string or does not exist" << endl;

						if (dAsset.HasMember("Parser") && dAsset["Parser"].IsString())
							Asset.Parser = dAsset["Parser"].GetString();
						else
							cerr << "Failed to read site configuration file: " << "Parser member is not a string or does not exist" << endl;
						
						if (dAsset.HasMember("FieldDelimiter") && dAsset["FieldDelimiter"].IsString())
							Asset.FieldDelimiter = dAsset["FieldDelimiter"].GetString()[0];

						if (dAsset.HasMember("ValueDelimiter") && dAsset["ValueDelimiter"].IsString())
							Asset.ValueDelimiter = dAsset["ValueDelimiter"].GetString()[0];

						DataSet->Assets.push_back(Asset);
					}
				}
				else
				{
					cerr << "Failed to read site configuration file: " << "unable to find Assets member" << endl;
				}

				DataSets.push_back(DataSet);
			}
		}
	}
	else
	{
		cerr << "Failed to read site configuration file: " << "unable to find DataSets member" << endl;
	}

	if (d.HasMember("Locations"))
	{
		auto & dLocations = d["Locations"];
		if (dLocations.IsArray())
		{
			for (uint i = 0; i < dLocations.Size(); ++ i)
			{
				auto & dLocation = dLocations[i];
				CLocation * Location = new CLocation(this);
				Location->ColorFile = dLocation["ColorFile"].GetString();
				Location->BathymetryFile = dLocation["BathymetryFile"].GetString();
				Location->HeightFile = dLocation["HeightFile"].GetString();
				if (dLocation.HasMember("NormalFile") && dLocation["NormalFile"].IsString())
					Location->NormalFile = dLocation["NormalFile"].GetString();

				auto & dLowerBound = dLocation["LowerBound"];
				Location->LowerBound.Longitude = SLongitudeLatituded::DMStoDecimal(dLowerBound["Longitude"].GetString());
				Location->LowerBound.Latitude = SLongitudeLatituded::DMStoDecimal(dLowerBound["Latitude"].GetString());

				auto & dUpperBound = dLocation["UpperBound"];
				Location->UpperBound.Longitude = SLongitudeLatituded::DMStoDecimal(dUpperBound["Longitude"].GetString());
				Location->UpperBound.Latitude = SLongitudeLatituded::DMStoDecimal(dUpperBound["Latitude"].GetString());

				Locations.push_back(Location);
			}
		}
	}
	else
	{
		cerr << "Failed to read site configuration file: " << "unable to find Locations member" << endl;
	}

    if (d.HasMember("Tracks")) 
    {
        auto & dTracks = d["Tracks"];
        if (dTracks.IsArray())
        {
            for (uint i = 0; i < dTracks.Size(); ++i) {
                auto & dTrack = dTracks[i];

                if (dTrack.HasMember("TrackFiles"))
                {
                    auto & dTrackFiles = dTrack["TrackFiles"];
                    for (uint j = 0; j < dTrackFiles.Size(); ++j) 
                    {
                        auto & dTrackFile = dTrackFiles[j];

                        CSplinePath* splinePath = new CSplinePath();

                        if (dTrackFile.HasMember("File") && dTrackFile["File"].IsString())
                        {
                            cout << "Creating SplinePath using file: " << Path + dTrackFile["File"].GetString() << endl;
                            splinePath->setFileName(Path + dTrackFile["File"].GetString());
                        }
                        else
                        {
                            cerr << "Error parsing file name of shark file " << j << endl;
                        }

                        Tracks.push_back(splinePath);
                    }
                }
            }
        }
    }
    else 
    {
        cerr << "Site directory does not contain any tracking files, or the files have not been formatted into site.json" << endl;
    }
}

void CSite::Load(IProgressBar * Progress)
{
	Progress->BeginProgress();

	f64 Total = Locations.size() + DataSets.size();
    for (auto Location : Locations) {
        Location->Load(Progress->NewTask(1 / Total));
    }
    for (auto DataSet : DataSets) {
        DataSet->Load(Progress->NewTask(1 / Total));
    }
    for (CSplinePath* Track : Tracks) {
        Track->setDataSet(DataSets[0]);
        Track->setLocation(Locations[0]);
        CProgramContext & Context = CProgramContext::Get();
        Track->setContext(&Context);
        this->GetCurrentLocation();
        Track->initSpline();
    }
	Progress->EndProgress();
}

void CSite::ConcurrentLoad()
{
	for (auto Location : Locations)
		Location->ConcurrentLoad();
	for (auto DataSet : DataSets)
		DataSet->ConcurrentLoad();
}

void CSite::InitSceneElements(CProgramContext::SScene & Scene)
{
	if (Locations.size())
		Locations[SelectedLocation]->InitSceneElements(Scene);
	if (DataSets.size()) {
		DataSets[SelectedDataSet]->InitSceneElements(Scene);

		SRange<f64> TRange = DataSets[SelectedDataSet]->Points.GetFieldRange(DataSets[SelectedDataSet]->Traits.TField, 15.0);
		CProgramContext & Context = CProgramContext::Get();
		Context.WorldTime->SetTime(TRange.Minimum);
	}
}

void CSite::AddDataSet(CDataSet * DataSet)
{
	DataSets.push_back(DataSet);
}

CDataSet * CSite::GetCurrentDataSet()
{
	if (DataSets.size())
		return DataSets[SelectedDataSet];
	else
		return nullptr;
}

CLocation * CSite::GetCurrentLocation()
{
	if (Locations.size())
		return Locations[SelectedLocation];
	else
		return nullptr;
}

CSplinePath * CSite::GetCurrentSplinePath()
{
	if (Tracks.size())
		return Tracks[SelectedSplinePath];
	else
		return nullptr;
}

string const & CSite::GetName() const
{
	return Name;
}

string const & CSite::GetPath() const
{
	return Path;
}

std::vector<CLocation *> const & CSite::GetLocations() const
{
	return Locations;
}

std::vector<CDataSet *> const & CSite::GetDataSets() const
{
	return DataSets;
}

void CSite::SetSelectedLocation(int const SelectedLocation)
{
	this->SelectedLocation = Clamp<int>(SelectedLocation, 0, (int) Locations.size() - 1);
}
