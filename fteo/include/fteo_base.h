//===========================================================================
//@2017 Fixosoft    FIXOSOFT FTEO LIBRARY
//===========================================================================
/*//////////////////////////////////////////////////////////////////////////*
 
     fteo_base.h - ���������� corefteo.lib
    �������� ������� ������ ������������ fteo, fteo::api
                      
     
     !!! ������ ���-�� ����� ��� ����������� ���������� corefteo.lib
                      �� ����� ������.
 
 
///////////////////////////////////////////////////////////////////////////*/




#include <list>
#include <xstring>

#ifndef  BASECLASSES_H
#define  BASECLASSES_H


// from common portability library:: cpl_port.h
// ��� ��� ������� ��������� �����������....�������� ��������� ��� define
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






//-------------------���������� (����������) �������  ---------------------------------------

//====================================================================
//
//
//
//����������
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
//K���� (�����). (������������� �� baseClasses.dll)
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
//------------������ ������ !!! -------------
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
          std::advance(pt, index); // � ������ ������ �����, �� ���� �� index
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
// ----�����  ������ �����---
 	class TMyPoints
	{
    //friend class TMyPoint; // ��� �������������, ��� ���������
	public:
		int item_id; // id ��������� ������ �����
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
        TOrdinate* Begin; //������ ������
        TOrdinate* End;   //����� ������
        double    Length();
        TLine(double beginX, double beginY, double EndX, double EndY);
        TLine(TOrdinate* begin, TOrdinate* end);
	    TMyPoint* FindIntersect(char* MGTName, TOrdinate* b1, TOrdinate* b2); // ����� ����������� � ������ b1-b2

      };


    class TPolyLine
      {
      public:
        int        id;
        TMyPoints* Vertexs;
      };


    // K����� - ���������� ��� �������. ����� �������, �������� � �������� ���������
    class TSpatialElement  : public TMyPoints
      {
      public:
       char*      LayerName; // ��� ���� , ���� OUT.... ����� �� ������ � Definition
       double     Area();
	   double     Perymethr(); //�������� �������
       TMyPoints* CheckIntersect(TMyPoint* b1, TMyPoint* b2); // ����� ����������� ������� � ������ b1-b2
       bool       Overlapped(TSpatialElement* check); //�������� �� ������ ��������
	   void		  Draw(); // nocomments
      };



    //������� (������ �������� - ������� (0), � ����������)
    class TEntitySpatial
      { private:
        TMyList<TSpatialElement>   *fMyEs; //�� ������� TMyList ���������� ���������� �����
         public:
                              TEntitySpatial();
             char*            EntSys;         //������ �� ���-���� ���������
             void             AddElement(TSpatialElement *Value);
             void             AddElementPoint(TMyPoint *point, int TargetIndex); // 0 -> to 'OUT';
             TSpatialElement* AddChild(char* layername);
             TMyPoints*       CheckIntersctL(TMyPoint* b1, TMyPoint* b2); // ����� ����������� �������� � ������ b1-b2
			 TMyPoints*       CheckIntersctC(TEntitySpatial* checkin);    // ����� ����������� �������� � ��������
			 int              Count(); // ���������� ��������
             int              PointCount();
             bool             HasChilds(); // ���� �� �����. �������
             int              id;
             TSpatialElement* SpatialElement(int index); // �������� ��������� ������, 0 - �������
             TSpatialElement* SpatialElement(char* layername); // �������� ��������� ������, 0 - �������
             TMyPoints*       AsPoints(); // ��������� ��� ����� � ���� ������. ��� , ������������ ������
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
       TMyList<TMyContour> *Contour; //������� ���-����
       TMyContour* Add(char *Value);
       TMyContour* Append (TMyContour *Value);
       void        Appends(TMyContours* Items);
	   TMyPoints*  CheckIntersections(TEntitySpatial* checkin);    // ����� ����������� �������� � ��������
       TMyContour* GetContourbyDef(char* ContourDefinition);
       int         PointCount();
       TMyPoints*  AsPoints(); // ��������� ��� ����� � ���� ������. ��� , ������������ ������
     };

//====================================================================
//
 class TAreaNew
   {
    double Area;
    char *Unit;
    double Inaccuracy;
   };
//����� ���������� �������
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
    double Area; // ����� ����� TAreaNew
     // LotName   - � ������ MP05 ��� �������, �.�. ���� ES ���� Contours
    TMyContours *Contours;
    TEntitySpatial *EntitySpatial;
    char *Definition;
    };

  //���������������� ��� ��
  class TExistParcel {
    char *CadastralBlock;
    TAreaNew  *Area;
    TEntitySpatial *EntitySpatial;
       TMyContours *Contours;

    double AreaInGKN;
    double DeltaArea;
    double MinArea;
    double MaxArea;
    char *Note;  //  ���� ��������
    char *CadastralNumber;
    };
 // ���
  class TExistEZParcel {
    char *CadastralBlock;
    TAreaNew  *Area;
    char *CadastralNumber;
    };

  //���������� ��
  class TSpecifyParcel{
      TExistParcel *ExistParcel;
    TExistEZParcel *ExistEZ;
    };








int   str2int (const std::string  &str);  //  STl ������ � integer
char *str2char(std::string src); //  STl ������ � native char*

 }
}
#endif // BASECLASSES_H
