#ifndef _SCI_DATA_PARSER_H_INCLUDED_
#define _SCI_DATA_PARSER_H_INCLUDED_

#include <vector>
#include <ionScene.h>
#include <ionCore.h>

#include "SciDataSet.h"
#include "SciDataTree.h"

#include "IColorMapper.h"


class SciDataParser
{

public:

	SciDataParser()
		: VolumeHandle(0)
	{}

	SciDataSet RawValues;
	SciDataSet GridValues;

	unsigned int VolumeHandle;
	ISciTreeNode * DataTree;

	int * GridDimensions;

	void createVolumeFromGridValues(IColorMapper * ColorMapper);
	void createDataTreeFromRawValues();
	void createGridDataFromRawValues(Range AcceptedValues = FullRange, double Deviations = 5.0, std::string const & Field = "o2");
	void createPointCloudObjects(bool FromRaw, ISceneObject * RootParent, SVector3f const DataScale, IColorMapper * ColorMapper);

	virtual void load(std::string const & FileName) =0;

};

class SciDataParserCTD : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

class SciDataParserSimpleTXT : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

class SciDataParserGrid1 : public SciDataParser
{

public:

	void load(std::string const & FileName);

};

#endif