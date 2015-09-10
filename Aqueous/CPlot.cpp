#include "CPlot.h"

#include <iostream>
#include <sstream>
#include <string>
#include "CSpectrumColorMapper.h"

CPlot::CPlot(std::vector<mreal> data) {
	_data = mglData(data.size(), &data[0]);
}

void CPlot::graph(Mode m) {
	switch (m) {
	case CPlot::Histogram:
		graphHist();
		break;
	case CPlot::Line:
		graphLine();
		break; 
	case CPlot::Points:
		graphPoints();
		break;
	}
}

void CPlot::graphHist() {
	_graph.Clf("{xFFFFFF00}");
	_graph.SubPlot(1, 1, 0, "");
	_graph.InPlot(0, 1, 0, 1);
	_counts = graphHist(&_graph, _data, _divisions, _colors);
	_invalid = false;
	lastUsed = Mode::Histogram;
}

void CPlot::graphLine() {
	_graph.Clf("{xFFFFFF00}");
	_graph.SubPlot(1, 1, 0, "");
	graphLine(&_graph, _data, false); // _colors ignored - does not work well
	_invalid = false;
	lastUsed = Mode::Line;
}

void CPlot::graphPoints() {
	_graph.Clf("{xFFFFFF00}");
	_graph.SubPlot(1, 1, 0, "");
	graphPoints(&_graph, _data, _times, _colors);
	_invalid = false;
	lastUsed = Mode::Points;
}

void CPlot::setData(std::vector<mreal> data) {
	_data = mglData(data.size(), &data[0]);
	Invalidate();
}

void CPlot::setTimes(std::vector<mreal> times) {
	_times = mglData(times.size(), &times[0]);
	Invalidate();
}

SVector3f CPlot::getPosXYZ(double x, double y) {
	mglPoint p = _graph.CalcXYZ(x, y);
	return SVector3f(p.x, p.y, p.z);
}

double CPlot::getValAt(double x) {
	switch (lastUsed) {
	case Mode::Histogram:
		if (x > 0 && x < _counts.GetNx()) {
			return _counts.a[(int)floor(x)];
		}
		else {
			return 0;
		}
		break;

	case Mode::Line:
		return _data.Spline(x);
		break;

	case Mode::Points:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

/* Static Usage */
mglData CPlot::graphHist(std::vector<mreal> &data, const int divisions, bool colors) {
	mglGraph graph;
	graph.SubPlot(1, 1, 0, "");

	return CPlot::graphHist(&graph, mglData(data.size(), &data[0]), divisions, colors);
}

void CPlot::graphLine(std::vector<mreal> &data, bool colors) {
	mglGraph graph;
	graph.SubPlot(1, 1, 0, "");

	CPlot::graphLine(&graph, mglData(data.size(), &data[0]), colors);
}

void CPlot::graphPoints(std::vector<mreal> &data, std::vector<mreal> &times, bool colors) {
	mglGraph graph;
	graph.SubPlot(1, 1, 0, "");

	CPlot::graphPoints(&graph, mglData(data.size(), &data[0]), mglData(times.size(), &times[0]), colors);
}

mglData CPlot::graphHist(mglGraph *gr, mglData &data, const int divisions, bool colors) {
	const int min = floor(data.Minimal());
	const int max = ceil(data.Maximal());

	mglData counts = data.Hist(divisions, min, max);

#ifdef DEBUG
	std::cout << "Raw Data" << std:endl;
	printData(data);
	std::cout << "Histogram Counts" << std:endl;
	printData(counts);
#endif

	int subdivisions = divisions / (max - min);
	int paddingTicks = (divisions % (max - min));
	int vertIncrement = ceil(counts.Maximal() / 10);

	gr->SetRange('x', min, max + (double)paddingTicks / subdivisions + 0.000001);
	gr->SetTicks('x', 1.0, subdivisions - 1);
	gr->SetRange('y', 0, vertIncrement * ceil(counts.Maximal() / vertIncrement));
	gr->Axis();
	gr->SetRange('x', 0, divisions);

	std::string pen = "";
	if (colors) {
		std::stringstream str;
		str << "!";
		for (int i = 0; i < divisions; i++) {
			SColorAf col = CSpectrumColorMapper::MapColor((i + 0.5) / divisions);
			str << colorRGBA(col);
		}
		pen = str.str();
	}

	gr->Bars(counts, pen.c_str());
	gr->WriteFrame("Media/graph.png");

	return counts;
}

void CPlot::graphLine(mglGraph *gr, mglData &data, bool colors) {
	const int min = floor(data.Minimal());
	const int max = ceil(data.Maximal());

	int vertIncrement = ceil(data.Maximal() / 10);

#ifdef DEBUG
	std::cout << "Raw Data" << std::endl;
	printData(data);
	std::cout << "Times" << std::endl;
	printData(times);
#endif

	gr->SetRange('x', 0, data.GetNx());
	gr->SetTicks('x', 5, 0);
	gr->SetRange('y', vertIncrement * floor(min / vertIncrement), vertIncrement * ceil(max / vertIncrement));
	gr->Axis("~x");
	gr->Axis("y");

	string pen = "";
	if (colors) {
		std::stringstream str;
		str << "!";
		for (int i = 0; i < data.GetNx(); i++) {
			SColorAf col = CSpectrumColorMapper::MapColor((data.a[i] - data.Minimal()) / (data.Maximal() - data.Minimal()));
			str << colorRGBA(col);
		}
		pen = str.str();
	}

	gr->Plot(data, pen.c_str());
	gr->WriteFrame("Media/graph.png");
}

void CPlot::graphPoints(mglGraph *gr, mglData &data, mglData &times, bool colors) {
	const int min = floor(data.Minimal());
	const int max = ceil(data.Maximal());

	int vertIncrement = ceil(data.Maximal() / 10);

#ifdef DEBUG
	std::cout << "Raw Data" << std::endl;
	printData(data);
	std::cout << "Times" << std::endl;
	printData(times);
#endif

	gr->InPlot(0, 1, 0.1, 1);
	gr->SetRange('x', times.Minimal(), times.Maximal());
	gr->SetTicksTime('x', 0.0, "%I:%M");
	gr->SetTickRotate(true);
	gr->SetRange('y', vertIncrement * floor(min / vertIncrement), vertIncrement * ceil(max / vertIncrement));
	gr->Axis();

	string pen = " #s";
	if (colors) {
		std::stringstream str;
		str << "! #s";
		for (int i = 0; i < data.GetNx(); i++) {
			SColorAf col = CSpectrumColorMapper::MapColor((data.a[i] - data.Minimal()) / (data.Maximal() - data.Minimal()));
			str << colorRGBA(col);
		}
		pen = str.str();
	}

	gr->Plot(times, data, pen.c_str());
	gr->WriteFrame("Media/graph.png");
}

void CPlot::printData(mglData &dat) {
	for (int i = 0; i < dat.GetNx(); i++) {
		std::cout << i << ": " << dat.a[i] << std::endl;
	}
}

string CPlot::colorRGBA(SColorAf col) {
	std::stringstream str;

	str << "{x";
	str << std::setfill('0') << std::setw(2) << std::hex << (int)(255 * col.Red);
	str << std::setfill('0') << std::setw(2) << std::hex << (int)(255 * col.Green);
	str << std::setfill('0') << std::setw(2) << std::hex << (int)(255 * col.Blue);
	str << std::setfill('0') << std::setw(2) << std::hex << (int)(255);
	str << "}";

	return str.str();
}