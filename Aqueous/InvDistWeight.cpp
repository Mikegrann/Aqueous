#include "InvDistWeight.h"

// Inverse Distance Weighting based on following discussion:
// https://en.wikipedia.org/wiki/Inverse_distance_weighting
// interpolated value: u(x) = (exists x_i | d(x, x_i) == 0) ? u_i : sum(w_i(x) * u_i) / sum(w_i(x))
// known values: u_i = u(x_i) for known x_i
// known locations: x_i
// weights: w_i(x) computed with weight()
float InvDistWeight::interpolate(std::map<vec3f, float> const &known, vec3f const &position, int power) {
	auto search = known.find(position);
	if (search == known.end()) { // Not Found
		float numerator = 0, denominator = 0;
		for (auto it = known.begin(); it != known.end(); ++it) {
			float w_i = weight(it->first, position, power);
			numerator += w_i * it->second;
			denominator += w_i;
		}

		return numerator / denominator;
	}
	else {
		return search->second;
	}
}

// weight: w(x) = 1 / d(x, x_i) ^ power
// x: point at which to interpolate
// x_i: known point for which we have a value
float InvDistWeight::weight(vec3f const &x, vec3f const &x_i, int power) {
	return 1 / pow(Distance(x, x_i), power);
}