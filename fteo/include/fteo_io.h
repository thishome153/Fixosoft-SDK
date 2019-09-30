
#include "fteo_base.h"
#include "fteo_mif.h"
#include "dl_dxf.h"
#include "dl_creationadapter.h"

#ifndef  FTEOAPIIO_H
#define FTEOAPIIO_H

namespace fteo
{
  namespace api
{

/*



*/

	 // Класс для экспорта в различные форматы
   //public - is assembly access specifier. Only in code /clr
 class TextWriter
  {
  private:
       int   DeltaGeopoint;
       bool writePointPref;
       char *FileName;
	 public: 
       TextWriter(bool writepointpref, double deltageopoint, char *filename);
                void SaveAsDxf(void *Entity);
				int SaveBodyToFile(unsigned long cbData, unsigned char* pbData);
      public:   void SaveAsFteo15(TMyPoints *Entity);//TMyPoints as  #Fixosoft NumXYZD data format V2015
       //void SaveAsFteo15(void *Entity);  // #Fixosoft NumXYZD data format V2015
  };


//TechnoCAD CSV 
//'Контур;Префикс номера;Номер;Старый X;Старый Y;Новый X;Новый Y;Метод определения;Формула;Радиус;Погрешность;Описание закрепления'
class    TcsvString {
public:  char *src; //временно public
private: char*  fDefinition;
         void ParseItem(int  &index, char* &Dest);
		 char* RemoveBracets(char *Name); // [ and ] 
public:
	int sz; // length of string  
    char*  Pref;
    char*  Number;
    char*  oldx; char* oldy;
    char*  x; char*   y; char*  Method; char*  f; char*  r; char*  Mt; char*  Descr;
    TcsvString(char *InitString);
    void Parse();
	char* ParentDefinition();
    char*       Definition();
    TMyPoint* AsPoint();
	bool isChild();
    int Status();
  };





/*************************TMydxf*******************************
 * This class takes care of the entities read from the file.
 * Usually such a class would probably store the entities.
 * this one just prints some information about them to stdout.
 *
 *
 * @author Andrew Mustun
 **************************************************************/
class TMydxf_DLInterface : public DL_CreationAdapter {
public:
    TMydxf_DLInterface();
	TMyContours* dxfPolyGons;
	TMyPoints* dxfPoints;
    virtual void addLayer(const DL_LayerData& data);
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void add3dFace(const DL_3dFaceData& data);

    void printAttributes();
};




//public - is assembly access specifier. Only in code /clr


  class TextReader
	 {
     private: bool parseCSVContour(std::string FileString, TMyContours* res);
			  bool parseCSVContour(char *FileString, TMyContours* res);
	 public:

		 TextReader();
		 char *ClassName;// = "Fixosoft TextReader";
			  int  ReadFileToVector(const char* filename, std::vector<unsigned char>& buffer);
	  TMyContours *ImportNXYZD2015File(const char *pszFname);
      TMyPoints   *Import_NikonCoord(char *FileName);
	  TMyContours *Read_dxf(const char *pszFname);
	  TMyContours *Read_csv_TechnoCAD(std::string FileName);  //Загрузка csv TecnoCad через ifstream. Проблемы с русскими именами
	  TMyContours *Read_csv_TechnoCADF(const char *pszFname); //Загрузка csv TecnoCad using TCsvFile::TmyFile
	  TMyContours *Read_mif_MITabAPI(char* FileName); //Загрузка mif M a p i n f o через miTab API (mitab.dll / mitab.lib)
	  TMyContours *Read_mif(char* FileName);    //Загрузка self miFiles.h
	  //TMyPoints   *Read_mif(char* FileName);  //Загрузка self miFiles.h. Тоже чтои выше, но в список точек
	 };
	  
	  




	  }//namespace api
	}//namespace fteo
#endif