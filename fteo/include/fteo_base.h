//===========================================================================
//@2017 Fixosoft    FIXOSOFT FTEO LIBRARY
//===========================================================================
/*//////////////////////////////////////////////////////////////////////////*
 
     fteo_base.h - заголовник corefteo.lib
    Содержит базовые классы пространства fteo, fteo::api
                      
     
     !!! Менять что-то здесь без перестройки библиотеки corefteo.lib
                      не имеет смысла.
 
 
///////////////////////////////////////////////////////////////////////////*/




#include <list>
#include <xstring>

#ifndef  BASECLASSES_H
#define  BASECLASSES_H


// from common portability library:: cpl_port.h
// тут оно недавно перестало компилиться....пришлось вписывать без define
/*
#ifndef CPL_MAX
  #define CPL_MIN(a,b)      ((a<b) ? a : b)
  #define CPL_MAX(a,b)      ((a>b) ? a : b)
#endif


#ifndef CPL_ABS
  #define CPL_ABS(x)        ((x<0) ? (-1*(x)) : x)
#endif
*/
namespace fteo
{

  namespace api
{




#ifdef BASECLASSES_EXPORTS
#define BASECLASSES_API __declspec(dllexport)
#else
#define BASECLASSES_API __declspec(dllimport)
#endif






// This class is exported from the baseClasses.dll
//class BASECLASSES_API CbaseClasses {
//public:
//	CbaseClasses(void);
	// TODO: add your methods here.
//};
//extern BASECLASSES_API int nbaseClasses;
//BASECLASSES_API int ExampleFunction(void);






//-------------------Декларации (объявления) классов  ---------------------------------------

//====================================================================
//
//
//
//Координата
class TOrdinate
{
public:
	TOrdinate();
	TOrdinate(double valueX, double valueY);
    double x;
	double y;
	double z;
	double DeltaGeopoint;
    bool changed;
  };

class TPoint
{
public: TPoint();
       ~TPoint();
      int id;
	  int Parent_id;
};

//====================================================================
//
//Kласс (Точка). (экспортируеся из baseClasses.dll)
//
 class TMyPoint : public TPoint
{
public:
     TMyPoint();
	~TMyPoint();
	  char *Name;
	  char *Description;
      char *NumGeopoint();
      char *GeopointZacrep;
	int Status;
	//double x;
	//double y;
	//double z;
	double DeltaGeopoint;
    bool changed;
    TOrdinate *NewOrd;
    TOrdinate *OldOrd;
private:
};




 //Simple container for a bounding region.
class  Envelope //Simple container for a bounding region.
{
  public:
        Envelope();

    double      MinX;
    double      MaxX;
    double      MinY;
    double      MaxY;

    int  IsInit();
    void Merge( Envelope & sOther );
};



//====================================================================
//
//------------Шаблон класса !!! -------------
      template <typename T>
      class TMyList : public std::list<T>
        {
        public:
        void Add(T *Item) {
          std::list<T>::const_iterator  it = std::list<T>::end();  // at end
          std::list<T>::insert(it, *Item);
          };

        T* GetByIndex(int index){
          std::list<T>::const_iterator pt = std::list<T>::begin();
          std::advance(pt, index); // в данном случае верно, от нуля на index
            return &pt._Ptr->_Myval;
          };

        void Delete(int id) {    };
        void DeleteByIndex(int index) {    };
		int Count() {return std::list<T>::size(); };

        };
      //Example for class Template:
      /*

      template <typename T>
       class Storage8
        {
        private:
          T m_tType[8];
          public:
            void Set(int nIndex, const T &tType)
              {
                m_tType[nIndex] = tType;
              }
             const T& Get(int nIndex)
            {
              return m_tType[nIndex];
            }
        };
*/

//====================================================================
//
// ----Класс  Список точек---
 	class TMyPoints
	{
    //friend class TMyPoint; // Это необязательно, для отработки
	public:
		int item_id; // id владельца списка точек
		//PointList *Items;
        TMyList<TMyPoint> *Items;
		void ClearItems();
		void AppendPoint(TMyPoint *Value);
        void AppendPoints(TMyPoints *items);
		bool Empty();
        bool RemovePoint(int point_id);
        TMyPoint *getpoint(int point_id);
        TMyPoint *getpointbyName(char* RequestPointName);
        TMyPoint *getpointbyindex(int index);
		int Count();

		 TMyPoints();
		 TMyPoints(int _item_id);
        ~TMyPoints();
//	private:
	};

   class TLine
      {
      public:
        int       id;
        char*     Name;
        TOrdinate* Begin; //Начало прямой
        TOrdinate* End;   //Конец прямой
        double    Length();
        TLine(double beginX, double beginY, double EndX, double EndY);
        TLine(TOrdinate* begin, TOrdinate* end);
	    TMyPoint* FindIntersect(char* MGTName, TOrdinate* b1, TOrdinate* b2); // Точка пересечения с прямой b1-b2

      };


    class TPolyLine
      {
      public:
        int        id;
        TMyPoints* Vertexs;
      };


    // Kонтур - внутренний или внешний. Имеет площадь, периметр и проверку геометрий
    class TSpatialElement  : public TMyPoints
      {
      public:
       char*      LayerName; // имя слоя , типа OUT.... чтобы не путать с Definition
       double     Area();
	   double     Perymethr(); //Периметр контура
       TMyPoints* CheckIntersect(TMyPoint* b1, TMyPoint* b2); // точки пересечения контура и прямой b1-b2
       bool       Overlapped(TSpatialElement* check); //проверка на полное накрытие
	   void		  Draw(); // nocomments
      };



    //Полигон (Список контуров - внешний (0), и внутренние)
    class TEntitySpatial
      { private:
        TMyList<TSpatialElement>   *fMyEs; //Из шаблона TMyList производим конкретный класс
         public:
                              TEntitySpatial();
             char*            EntSys;         //Ссылка на сис-тему координат
             void             AddElement(TSpatialElement *Value);
             void             AddElementPoint(TMyPoint *point, int TargetIndex); // 0 -> to 'OUT';
             TSpatialElement* AddChild(char* layername);
             TMyPoints*       CheckIntersctL(TMyPoint* b1, TMyPoint* b2); // точки пересечения полигона и прямой b1-b2
			 TMyPoints*       CheckIntersctC(TEntitySpatial* checkin);    // точки пересечения полигона и полигона
			 int              Count(); // количество контуров
             int              PointCount();
             bool             HasChilds(); // Есть ли внутр. границы
             int              id;
             TSpatialElement* SpatialElement(int index); // Получить указанный контур, 0 - внешний
             TSpatialElement* SpatialElement(char* layername); // Получить указанный контур, 0 - внешний
             TMyPoints*       AsPoints(); // сохраняет все точки в один список. Вот , понадобилось однако
     };


//====================================================================
  class TMyContour
    {
    public:
      TMyContour();
      double Area();
       TEntitySpatial *EntitySpatial;
      char *Definition;
    };

   class TMyContours
     {
     public:
       TMyContours();
       TMyList<TMyContour> *Contour; //Элемент кон-тура
       TMyContour* Add(char *Value);
       TMyContour* Append (TMyContour *Value);
       void        Appends(TMyContours* Items);
	   TMyPoints*  CheckIntersections(TEntitySpatial* checkin);    // точки пересечения полигона и полигона
       TMyContour* GetContourbyDef(char* ContourDefinition);
       int         PointCount();
       TMyPoints*  AsPoints(); // сохраняет все точки в один список. Вот , понадобилось однако
     };

//====================================================================
//
 class TAreaNew
   {
    double Area;
    char *Unit;
    double Inaccuracy;
   };
//Часть земельного участка
  class TSubParcel
    {
     double Area;
     //Encumbrance;
     TMyContours *Contours;
     TEntitySpatial *EntitySpatial;
    };

  class TNewSubParcel : TSubParcel
    {
     char *Definition;
    };

  class TNewParcel    {
  public:
    TNewParcel();
    char *CadastralBlock;
    char *PrevCadastralNumbers;
    double Area; // здесь нужен TAreaNew
     // LotName   - в версии MP05 оно исчезло, т.е. либо ES либо Contours
    TMyContours *Contours;
    TEntitySpatial *EntitySpatial;
    char *Definition;
    };

  //Землепользование или МК
  class TExistParcel {
    char *CadastralBlock;
    TAreaNew  *Area;
    TEntitySpatial *EntitySpatial;
       TMyContours *Contours;

    double AreaInGKN;
    double DeltaArea;
    double MinArea;
    double MaxArea;
    char *Note;  //  Иные сведения
    char *CadastralNumber;
    };
 // ЕЗП
  class TExistEZParcel {
    char *CadastralBlock;
    TAreaNew  *Area;
    char *CadastralNumber;
    };

  //Уточняемый ЗУ
  class TSpecifyParcel{
      TExistParcel *ExistParcel;
    TExistEZParcel *ExistEZ;
    };








int   str2int (const std::string  &str);  //  STl строку в integer
char *str2char(std::string src); //  STl строку в native char*

 }
}
#endif // BASECLASSES_H
