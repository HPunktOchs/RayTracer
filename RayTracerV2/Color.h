#ifndef __COLOR_H__
#define __COLOR_H__

/**
 * "NaN To Zero"
 * Converts a NaN value to zero. Otherwise adding values to NaN results in NaN.
 * When adding colors together we usually want to ignore it and use 0 instead.
 */
#define NTZ(X) (isnan((X)) ? 0.0 : (X))

#include <math.h>

class Color {
public:
   double r;
   double g;
   double b;
   double f; // "filter" or "alpha"

   Color() : r(0.0), g(0.0), b(0.0), f(1.0) {}
   Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_), f(1.0) {}
   Color(double r_, double g_, double b_, double f_) : r(r_), g(g_), b(b_), f(f_) {}

   Color Color::operator+ (Color const &c) const {
	   Color other;

	   other.r = NTZ(c.r) + NTZ(r);
	   other.g = NTZ(c.g) + NTZ(g);
	   other.b = NTZ(c.b) + NTZ(b);

	   return other;
   }

   Color Color::operator* (double amount) const {
	   Color other;

	   other.r = r * amount;
	   other.g = g * amount;
	   other.b = b * amount;

	   return other;
   }

   Color Color::operator/ (double amount) const {
	   Color other;
	   other.r = r / amount;
	   other.g = g / amount;
	   other.b = b / amount;
	   return other;
   }
};

#endif
