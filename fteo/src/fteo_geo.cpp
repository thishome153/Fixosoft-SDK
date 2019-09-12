#include "stdafx.h"
#include <math.h>
#include "fteo_geo.h"

namespace fteo
{
  namespace api
  {

double Geodethic::lent(double xn1, double yn1, double xn2, double yn2) {
		return sqrt(pow(xn2-xn1,2)+pow(yn2-yn1,2));
		};

 bool Geodethic::LinesIntersect(double ax1, double ay1, double ax2, double ay2, double bx1, double by1, double bx2, double by2)
        {
           double v1 = 0; double v2 = 0; double v3 = 0; double v4 = 0;
           v1 = (bx2-bx1)*(ay1-by1)-(by2-by1)*(ax1-bx1);
           v2 = (bx2-bx1)*(ay2-by1)-(by2-by1)*(ax2-bx1);
           v3 = (ax2-ax1)*(by1-ay1)-(ay2-ay1)*(bx1-ax1);
           v4 = (ax2-ax1)*(by2-ay1)-(ay2-ay1)*(bx2-ax1);

        return (v1*v2<0) & (v3*v4<0);
        }
    } // namespace api
  } // namespace fteo