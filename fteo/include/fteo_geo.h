
//===========================================================================
//@2017 Fixosoft    FIXOSOFT FTEO LIBRARY
//===========================================================================
/*//////////////////////////////////////////////////////////////////////////*
 
     fteo_geo.h - заголовник corefteo.lib
   Содержит классы геодезии
                      
     
     !!! Менять что-то здесь без перестройки библиотеки corefteo.lib
                      не имеет смысла.
 
 
///////////////////////////////////////////////////////////////////////////*/



#ifndef  GEODETHICS_H
#define GEODETHICS_H

namespace fteo
{
  namespace api
{
class Geodethic
    {
	public: 
		double lent(double xn1,double yn1,double xn2, double yn2);
		bool   LinesIntersect(double ax1, double ay1, double ax2, double ay2, double bx1, double by1, double bx2, double by2);
	  };



    }
  }

#endif // GEODETHICS_H