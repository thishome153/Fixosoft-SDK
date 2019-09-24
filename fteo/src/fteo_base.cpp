#include "stdafx.h"
#include <sstream> // stringStream
#include "fteo_base.h"
#include "fteo_geo.h"
//#include "fteo/include/CadWorks.h"

namespace fteo
{
  namespace api
{

	// This is an example of an exported variable
	//BASECLASSES_API int nbaseClasses = 0;

	// This is an example of an exported function.
	//BASECLASSES_API int ExampleFunction(void)
	//{
	//	return 42;
	//}

	// This is the constructor of a class that has been exported.
	// see baseClasses.h for the class definition
	//CbaseClasses::CbaseClasses()
	//{
	//	return;
	//}


//------------------------ Описания (определения) классов  ---------------------------------------

TOrdinate::TOrdinate() {};
TOrdinate::TOrdinate(double valueX, double valueY) {
	this->x = valueX;
	this->y = valueY;
	};

TPoint::TPoint()
{
  
}

TPoint::~TPoint()
{

}


  TMyPoint::TMyPoint()
	{  //Вот в таком виде
		this->Name = "NewTmyPoint";
		//Получаем deprecated conversion from string constant to 'char*' [-Wwrite-strings]|
		this->OldOrd = new TOrdinate();
		this->NewOrd = new TOrdinate();

		this->NewOrd->x = 0;
		this->NewOrd->y = 0;
		this->NewOrd->z = 0;
		this->id = 0;
		this->Parent_id = -1;

		this->Status = 0; // new
        double TextX = NewOrd->x;
	}

  char* TMyPoint::NumGeopoint()
    {
    return this->Name;
    };

	TMyPoint::~TMyPoint()
	{

	}

	Envelope::Envelope()
        {
                MinX = MaxX = MinY = MaxY = 0;
        }
	int  Envelope::IsInit() { 
		return MinX != 0 || MinY != 0 || MaxX != 0 || MaxY != 0
			; 
		}
	

	void Envelope::Merge( Envelope & sOther ) {
        if( IsInit() )
        {
            MinX =             ((MinX<sOther.MinX) ? MinX:sOther.MinX);//CPL_MIN(MinX,sOther.MinX);


            MaxX = ((MaxX>sOther.MaxX) ? MaxX:sOther.MaxX);//CPL_MAX(MaxX,sOther.MaxX);
            MinY = ((MinY<sOther.MinY) ? MinY:sOther.MinY);//CPL_MIN(MinY,sOther.MinY);
            MaxY = ((MaxY>sOther.MaxY) ? MaxY:sOther.MaxY);//CPL_MAX(MaxY,sOther.MaxY);
        }
        else
        {
            MinX = sOther.MinX;
            MaxX = sOther.MaxX;
            MinY = sOther.MinY;
            MaxY = sOther.MaxY;
        }
		}
//--------------------------------------------------------------------------------------
	TMyPoints::TMyPoints()
		{
        this->Items = new  TMyList<TMyPoint>();
		  this->item_id = -1; // new, no owner
		}
  // reload version
	TMyPoints::TMyPoints(int _item_id)
		{
        this->Items = new  TMyList<TMyPoint>();
		this->item_id = _item_id;

		};

	void TMyPoints::AppendPoint(TMyPoint *Value)
		{
		if ( ! Value) return; ///if ((int) Value == 0xCDCDCDCD)  return
         std::list<TMyPoint>::const_iterator  it = this->Items->end();  // at end
         this->Items->insert(it, *Value);

		}

    void TMyPoints::AppendPoints(TMyPoints *items)
    {
	if ( !items ) return;
      for (int i = 0; i <= items->Count()-1; i++)
      {
      // PointList::const_iterator pt =  items->Items->begin();
      TMyList<TMyPoint>::const_iterator pt =  items->Items->begin();
        std::advance(pt,i);
        this->AppendPoint(&pt._Ptr->_Myval);
      }

    }

    bool TMyPoints::RemovePoint(int point_id)
      {
       std::list<TMyPoint>::const_iterator pt = this->Items->begin();
      for (int i = 0; i <= Items->size()-1; i++)
      {
        std::advance(pt,i);
        if(pt->id == point_id) 
          { 
             //this->Items->remove(pt);
            return false;
          }
      }
       //Более компактный перебор:
      /*
       for (std::list<TMyPoint>::iterator it=Items->begin(); it!=Items->end(); ++it)
        {
          
        if (*it->id == point_id) 
          { 
             //this->Items->remove(pt);
            return false;
          }         
         
         }
      */
      }

	void TMyPoints::ClearItems()
		{
		this->Items->clear();
		this->item_id = -1; // free, no owner
		};

    TMyPoint *TMyPoints::getpoint(int point_id)
    {
      std::list<TMyPoint>::const_iterator pt = this->Items->begin();
      for (int i = 0; i <= Items->size()-1; i++)
      {
        std::advance(pt, i);
        if(pt->id == point_id) return &pt._Ptr->_Myval;
      }
    }

    TMyPoint *TMyPoints::getpointbyindex(int index)
      {
         /* std::list<TMyPoint>::const_iterator pt = this->Items->begin();
          std::advance(pt, index);
          return &pt._Ptr->_Myval;*/
	  if (index > this->Count()-1)  return NULL;
      return this->Items->GetByIndex(index);
      }
    
    TMyPoint *TMyPoints::getpointbyName(char *RequestPointName) {
      TMyPoint* res;
      for (int i = 0; i <= Items->size()-1; i++) 
        {
        res = Items->GetByIndex(i);
        if (strcmp(res->Name, RequestPointName) == 0)
          return res;
        }
      return NULL;
      };

	int  TMyPoints::Count()
		{
		if (! this->Items->empty()) 
			return this->Items->size(); 
		  else return 0; // undefined
		}
	    
	bool TMyPoints::Empty()
		{
   		 if (this->Count() ==0) return true;
		 else return false;
		};
	TMyPoints::~TMyPoints()
		{
          free(this->Items);

		};



	TLine::TLine(double beginX, double beginY, double EndX, double EndY) {
		TOrdinate* begin = new TOrdinate();
		TOrdinate* end = new TOrdinate();
		begin->x = beginX;
		begin->y = beginY;
		end->x = EndX;
		end->y = EndY;
		this->Begin = begin;
		this->End = end;
		};
//--------------------------------------------------------------------------------------
              TLine::TLine(TOrdinate* begin, TOrdinate* end) {
                this->Begin = begin;
                this->End = end;
      };

//--------------------------------------------------------------------------------------
    double    TLine::Length() {
	 Geodethic* geodethic = new Geodethic();
     return geodethic->lent(this->Begin->x, this->Begin->y, this->End->x, this->End->y);
      };

//--------------------------------------------------------------------------------------
    TMyPoint* TLine::FindIntersect(char* MGTName, TOrdinate* b1, TOrdinate* b2) {

			  	Geodethic* geodethic = new Geodethic();

                if (! geodethic->LinesIntersect(Begin->x,Begin->y,End->x,End->y,b1->x,b1->y,b2->x,b2->y))
			       return NULL ; // не пересекаются
			  
   double  v1,v2,v3,v4;
  
   const  double   eps = 0.0000000000000000000000000000000000000000000000001;
   double   k1,k2,q1,q2;  double Denominator1, Denominator2,Denominator3;

  //проверим, что она не параллельна оси и знаменатели не будут  равны 0
   if (End->x - Begin->x ==0 )  Denominator1 = eps; else Denominator1 = End->x -  Begin->x;
  if (b2->x- b1->x ==0 )  Denominator2 = eps; else Denominator2 = b2->x- b1->x;

  k1 = (End->y - Begin->y)/(Denominator1);
  k2 = (b2->y- b1->y)/(Denominator2);
   if (k1 ==0 & k2==0)   return NULL; //параллелльные

   q1 = Begin->y - Begin->x*k1;
   q2 = b1->y    - b1->x*k2;
   //еще проверим, что деления на ноль не будет:
  if (k2-k1 == 0)  Denominator3 = eps; else Denominator3 = k2-k1;

    TMyPoint* FP = new  TMyPoint();
	          FP->Name = MGTName;
	          FP->NewOrd->x = (q1-q2)/(Denominator3);
              FP->NewOrd->y = (k2*q1-k1*q2)/(Denominator3);
              FP->NewOrd->z = 0 ;
	          FP->Status =0;
              FP->Description = "Аналитич. решение";
    return  FP;
      };

//--------------------------------------------------------------------------------------
	double     TSpatialElement::Area() {
      double rr = 0;
                double x1_, y3_, y1_, y2_;
				int PtsCnt = this->Count();
                int CurPts = 0;
                if (PtsCnt > 2)
                {
                    PtsCnt--; // декремент сразу чтобы не возиться в массиве с выражениями типа array_[PtsCnt-1].x;
					y3_ = this->Items->GetByIndex(CurPts)->NewOrd->y; // Вначало !!!
                    CurPts++; // Вперёд !!!
                    y2_ = this->Items->GetByIndex(CurPts)->NewOrd->y;
					x1_ = this->Items->GetByIndex(PtsCnt)->NewOrd->x; // В конец
                    y1_ = this->Items->GetByIndex(PtsCnt)->NewOrd->y; // В конец
                    rr = x1_ * (y2_ - y1_);
					y2_ = this->Items->GetByIndex(PtsCnt - 1)->NewOrd->y; // Назад !!!
                    rr = rr + x1_ * (y3_ - y2_);
                    CurPts = 0;
                    for (int i = 2; i <= PtsCnt; i++)
                    {
                        y1_ = this->Items->GetByIndex(CurPts)->NewOrd->y;
                        CurPts++; // Вперёд !!!
                        x1_ = this->Items->GetByIndex(CurPts)->NewOrd->x;
                        CurPts++; // Вперёд !!!
                        y2_ = this->Items->GetByIndex(CurPts)->NewOrd->y;
                        rr = rr + x1_ * (y2_ - y1_);
                        CurPts--;
                    }
                    rr = abs(rr / 2);
                    return rr;
                }
                return rr;
	};

	double     TSpatialElement::Perymethr()
		{

		if (this->Items->Count() == 0) return -1;

            double Peryd = 0;

			Geodethic* geodethic = new Geodethic();
			
            for (int i = 0; i <= this->Items->Count() - 2; i++)
            {

  	          double Test = geodethic->lent( this->Items->GetByIndex(i)->NewOrd->x, 
				                             this->Items->GetByIndex(i)->NewOrd->y, 
											 this->Items->GetByIndex(i+1)->NewOrd->x, 
											 this->Items->GetByIndex(i+1)->NewOrd->y);
      //        if (Test != ) //нужна проверка
                    Peryd += Test;
            }
			Peryd += geodethic->lent( this->Items->GetByIndex(this->Count()-1)->NewOrd->x, 
				                      this->Items->GetByIndex(this->Count()-1)->NewOrd->y, 
								      this->Items->GetByIndex(0)->NewOrd->x, 
								      this->Items->GetByIndex(0)->NewOrd->y);
            return Peryd;
		};


//--------------------------------------------------------------------------------------
	TMyPoints* TSpatialElement::CheckIntersect(TMyPoint* b1, TMyPoint* b2) {
		if ((!b1) || (!b2)) return NULL;

		TMyPoints* res = new TMyPoints();
		int NextPointIndex =0;
        for (int i = 0; i <= this->Items->Count() - 1; i++)
	     { 
		  if (i == this->Count()-1) 
			  NextPointIndex = 0; 
		        else NextPointIndex = i+1; // чтобы не вылетать на последней границе
               TLine* CurrentLine = new TLine(this->getpointbyindex(i)->NewOrd,	this->getpointbyindex(NextPointIndex)->NewOrd);
               TMyPoint* resP = CurrentLine->FindIntersect("-", b1->NewOrd, b2->NewOrd);
                   if (resP)
                     res->AppendPoint(resP);
			}
			
		return res;
		};

//--------------------------------------------------------------------------------------
    TEntitySpatial::TEntitySpatial() {
      TSpatialElement *OutBorder = new TSpatialElement();
      //this->fSpatialElement = new std::list<TSpatialElement>();
      this->fMyEs = new TMyList<TSpatialElement>();
      OutBorder->LayerName = "OUT";
      AddElement(OutBorder); // внешний есть всегда, создаем его по дефолту
      };

         void TEntitySpatial::AddElement(fteo::api::TSpatialElement *Value)
           {
           // std::list<TSpatialElement>::const_iterator  it = this->fSpatialElement->end();  // at end
           // this->fSpatialElement->insert(it, *Value);
            this->fMyEs->Add(Value);
           };

         void TEntitySpatial::AddElementPoint(fteo::api::TMyPoint *point, int TargetIndex) {
           TSpatialElement *TargetES = this->SpatialElement(TargetIndex);
           if (TargetES)
             {
              TargetES->AppendPoint(point);
             };
           };

         TSpatialElement* TEntitySpatial::AddChild(char* layername) {
           //if (this->Count() > 0)  {  //// внешний есть всегда, создается по дефолту 
             
              TSpatialElement* NewChild = new TSpatialElement();
               NewChild->LayerName = layername;
               this->AddElement(NewChild);
              return NewChild;
              /*
             }
              else return NULL;
              */
           };
//.....................................................................................................
         bool TEntitySpatial::HasChilds()
           {
           if (this->fMyEs->size() > 1) return true; else return false;
           };
//.....................................................................................................
         int TEntitySpatial::Count()
          {
          return this->fMyEs->size();
           };
//.....................................................................................................
         int TEntitySpatial::PointCount() {
           int res = 0;
            for (int i = 0; i <= this->Count() - 1; i++)
              res += this->SpatialElement(i)->Count();
            return res;
           }
//.....................................................................................................
         TSpatialElement* TEntitySpatial::SpatialElement(int index) {
               return this->fMyEs->GetByIndex(index);
           };

//.....................................................................................................
		 TMyPoints* TEntitySpatial::CheckIntersctL(fteo::api::TMyPoint *b1, fteo::api::TMyPoint *b2) {
		  TMyPoints* ResLayer = new TMyPoints();
			 for (int i = 0; i <= this->Count() - 1; i++)

 				ResLayer->AppendPoints(this->SpatialElement(i)->CheckIntersect(b1,b2));

		 return ResLayer;
			 };

//.....................................................................................................
		 TMyPoints* TEntitySpatial::CheckIntersctC(fteo::api::TEntitySpatial *checkin) {
			TMyPoints* ResLayer = new TMyPoints();

			 for (int i = 0; i <= this->Count() - 1; i++)
			  for (int ci = 0; ci <= checkin->Count() - 1; ci++)
				  {
                // не заскочить бы за границы
				int NextP_id =0;
				for (int cip = 0; cip <= checkin->SpatialElement(ci)->Count() - 1; cip++) 
					{
					if (cip == checkin->SpatialElement(ci)->Count() - 1) NextP_id =0; else NextP_id = cip+1;
						
						TMyPoint* testP1 = checkin->SpatialElement(ci)->getpointbyindex(cip);
						TMyPoint* testP2 = checkin->SpatialElement(ci)->getpointbyindex(NextP_id);
						if ((testP1) || (testP2))
						 ResLayer->AppendPoints( this->CheckIntersctL(testP1,testP2));
					}
				  }
				  
		    return ResLayer;

			 };


         /// Внимание, выбирает по имени лажу....заменим на strcmp
         TSpatialElement* TEntitySpatial::SpatialElement(char *layername) {
           TMyList<TSpatialElement>::const_iterator pt = this->fMyEs->begin();
           
           for ( pt = this->fMyEs->begin(); pt != this->fMyEs->end(); pt++)
              {
              if (strcmp(pt->LayerName,layername) == 0)  // strcmp красафчег, чотко проверяет
                   return &pt._Ptr->_Myval;
              }
           };

       TMyPoints*       TEntitySpatial::AsPoints() // сохраняет все точки в один список. Вот , понадобилось однако
       {
           TMyList<TSpatialElement>::const_iterator pt = this->fMyEs->begin();
           TMyPoints* res = new TMyPoints(this->id);
           for ( pt = this->fMyEs->begin(); pt != this->fMyEs->end(); pt++)
              {
                res->AppendPoints(&pt._Ptr->_Myval);
              }
             return res;
       }
//=======================================================================================
         TMyContour::TMyContour() {
           this->EntitySpatial = new TEntitySpatial();
           };

         TMyContours::TMyContours() {
           this->Contour = new TMyList<TMyContour>();
           };

         TMyContour* TMyContours::Add(char *Value) {
           TMyContour* newCnt = new TMyContour();
           newCnt->Definition = Value;
//           newCnt->EntitySpatial 
           std::list<TMyContour>::const_iterator  it = this->Contour->end();  // at end
           this->Contour->insert(it, *newCnt);
           return newCnt;
           };

         TMyContour* TMyContours::Append(fteo::api::TMyContour *Value)
           {
           std::list<TMyContour>::const_iterator  it = this->Contour->end();  // at end
           this->Contour->insert(it, *Value);
           return Value;
           };

         void TMyContours::Appends(fteo::api::TMyContours *Items) {
           if ( ! Items) return;
           for (int i = 0; i <= Items->Contour->Count() - 1; i++)
             this->Append(Items->Contour->GetByIndex(i));
           };

		 TMyPoints* TMyContours::CheckIntersections(fteo::api::TEntitySpatial *checkin) {
		   if ( ! checkin) return NULL;
		  TMyPoints* res = new TMyPoints();
			 for (int i = 0; i <= this->Contour->Count() - 1; i++)
				 {
				 res->AppendPoints( this->Contour->GetByIndex(i)->EntitySpatial->CheckIntersctC(checkin));
				 
				 }
				return res;	
			 };

         TMyContour* TMyContours::GetContourbyDef(char* ContourDefinition) {
           if (this->Contour->empty()) return NULL;
             std::list<TMyContour>::const_iterator pt = this->Contour->begin();
           
           for ( pt = Contour->begin(); pt != Contour->end(); pt++)
              {
              //      std::advance(pt, 1);  // Increments given iterator it by n elements.
                if ( strcmp(pt->Definition,ContourDefinition) == 0) 
                   return &pt._Ptr->_Myval;

              }

           };
         int         TMyContours::PointCount() {
           int res = 0;
           for (int i = 0; i <= this->Contour->Count() - 1; i++)
             {
             res += this->Contour->GetByIndex(i)->EntitySpatial->PointCount();
             };
           return res;
           }
         TMyPoints*  TMyContours::AsPoints() // сохраняет все точки в один список. Вот , понадобилось однако
         {
           TMyPoints* res= new TMyPoints();
              std::list<TMyContour>::const_iterator pt = this->Contour->begin();
              
              for ( pt = Contour->begin(); pt != Contour->end(); pt++)
              {
                res->AppendPoints((&pt._Ptr->_Myval)->EntitySpatial->AsPoints());
              }
              return res;
         }
         
//=======================================================================================
   TNewParcel::TNewParcel()
     {
      //this->Definition
     };


//  STl строку в integer
int str2int (const std::string &str) 
  {
   std::stringstream ss(str);
  int num;
  if((ss >> num).fail())
  { 
    return -1;  //ERROR 
  }
  return num;
	}


//  STl строку в native
 char *str2char(std::string src)
	 {
      char * writable = new char[src.size() + 1];
        std::copy(src.begin(), src.end(), writable);
        writable[src.size()] = '\0'; // don't forget the terminating 0
		return writable;
	 }

}
}
