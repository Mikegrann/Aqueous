#pragma once

#include <string>
#include "RBFInterpolator/RBFInterpolator.h"

struct Interp {
	enum Mode {
		Radial,
		Connor,
		NumModes
	};

	static std::string GetInterpName(Mode mode) {
		switch (mode) {
		case Radial:
			return "Radial Basis";
			break;

		case Connor:
			return "Depth & Inverse Dist";
			break;

		default:
			return "";
			break;
		}
	}

	enum RadialFunc {
		Log_Shift,
		Thin_Spline,
		NumFuncs
	};

	static std::string GetFuncName(RadialFunc func) {
		switch (func) {
		case Log_Shift:
			return "Logarithmic Shift";
			break;

		case Thin_Spline:
			return "Thin Plate Splines";
			break;

		default:
			return "";
			break;
		}
	}

	static RBFunc GetFuncPtr(RadialFunc func) {
		switch (func) {
		case Log_Shift:
			return RBFInterpolator::log_shift;
			break;

		case Thin_Spline:
			return RBFInterpolator::thin_spline;
			break;

		default:
			return NULL;
			break;
		}
	}

	Interp(Mode m, RadialFunc f, int x, bool l) : mode(m), func(f), exponent(x), useLog(l)
	{}

	// Member vars
	Mode mode;
	RadialFunc func;
	int exponent;
	bool useLog;
};