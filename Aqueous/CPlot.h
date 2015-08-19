#pragma once
#ifndef CPLOT_H
#define CPLOT_H

#include <mgl2/mgl.h>
#include <ionScience.h>

class CPlot {
public:
	CPlot() {}
	CPlot(std::vector<mreal> data);

	enum Mode {
		Histogram,
		Line,
		NumModes
	};

	static std::string GetModeName(Mode m) {
		switch (m) {
		case Histogram:
			return "Histogram";
			break;
		case Line:
			return "Line Graph";
			break;
		}
	}

	void graph(Mode m);
	void graphHist();
	void graphLine();

	void setData(std::vector<mreal> data);

	void setDivisions(const int divs) { _divisions = divs; Invalidate(); }
	const int getDivisions() { return _divisions; }
	void useColors(const bool cols) { _colors = cols; Invalidate(); }
	const bool usingColors() { return _colors; }

	SVector3f getPosXYZ(SVector2f screenPos) { return getPosXYZ(screenPos.X, screenPos.Y); }
	SVector3f getPosXYZ(double x, double y);

	double getValAt(double x);

	bool isInvalid() { return _invalid; }
	void Invalidate() { _invalid = true; }

private:
	mglGraph _graph;
	mglData _data;
	mglData _counts;

	int _divisions = 10;
	bool _colors = true;

	bool _invalid = true;

	Mode lastUsed = Histogram;

	/* Static Usage */
public:
	static mglData graphHist(std::vector<mreal> &data, const int divisions, bool colors);
	static void graphLine(std::vector<mreal> &data, bool colors);

private:
	static mglData graphHist(mglGraph *gr, mglData &data, const int divisions, bool colors);
	static void graphLine(mglGraph *gr, mglData &data, bool colors);

	static void printData(mglData &dat);

	static string colorRGBA(SColorAf col);
};

#endif