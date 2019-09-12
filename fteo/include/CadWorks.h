
#include "fteo_base.h"

#ifndef  CADWORKS_H
#define  CADWORKS_H

namespace fteo {

  




#define CODETYPE_UNDEF "000"
#define CODETYPE_NEW   "014"
#define CODETYPE_EXIST "015"

enum CodeTypes {undef,New,Exist};

// CadWorkTypes - регламенты кадастровых работ
// 014
#define cw_OnNew_Partition 114   //Выдел 
#define cw_OnNew_Divide    214   //Раздел
#define cw_OnNew_DivideGos 314   //Раздел с изменным
#define cw_OnNew_Repart    414   //Перераспределение
#define cw_OnNew_          514   //'Образование из земель';
#define cw_OnNew_Unite     614   //Объединение
// 015
#define cw_OnExist_Slots   115   //Только части
#define cw_OnExist_Any     215   //Уточнение


#define cw_New_Build       123  // Здание
#define cw_New_Construct   223  // Сооружение
#define cw_New_UnComplete  323  // Объект незавершенного строительства (ОНС) (постановка на учет)
#define cw_New_Float       423  // Помещение


#define cw_Zone            533   //Охранная зона ЛЭП;   

#define cw_Default 5000     //Иные виды работ
#define cw_Unknown 0     //Неопределенный тип




 char* CadWorkTypeToName(int cw); 
 char* CadWorkTypeToChar(int cw); 
/* int str2int (const std::string &str); */
 char* GUIDGenerator();



//Классы для обработки записей БД. Портированы из Fteo5
 struct DBLayerItem
   {
      
     int Layer_ID;                  // PK таблицы LAYERS
     int ParentLayer_ID;            // Родительский Контур для   Layer_Type= IN и Level=2
     int Itemid; // Slot id
     int Lot_ID;                    // PK ЗУ
     int ParentLot_ID;
     int Order;                     // порядок слоев в MyObj
     int Level;                     // уровень: 1= OUT, 2= IN
     char *Definition;
     char *Layer_Type;                 //  IN, OUT, ANOTHER
   } ;

 struct  DBAreaItem {
     int Lot_ID;
     int Layer_ID;
     int Area_ID;
     int Area_order;
     int Point_ID;
     char *Def; //описание отрезка границы
   };



 //Error	17	error C3193: '^' : requires '/clr' command line option
/*
  ref class  Status
	  {
   public: static Color^ StatusToColor(int status);
	   };
 
/*

  */

 //====================================================================
  class TWorkEntry {
    public:
      fteo::api::TNewParcel *NewParcel;
        int id;
	    int Parent_id;    //parent_id LOTtable для детской  записи (child = 1)
        int EntryType;
        char *Name; // archnum
        char *LotName; // archnum
	  };

//----Класс  Кадастровые работы (МП. ТП . и т.п.)---
  class TWork {
private: int fCount;
public:
	int CadWorkType;
    int id;           //id LOTTABLE для записи типа child=0
    int EntryCount();
	int Status;
	char *CodeType;
    char *ArchNum; //Номер дела (работы, МП, ТП)
	//void *Items;   // Список земельных участков, ОКС и т.п. Обобщение в данный момент . [0] - Родительский элемент
	std::list<TWorkEntry> *EntryList; // Список земельных участков, ОКС и т.п. Обобщение в данный момент . [0] - Родительский элемент
    char *Conclusion;
    char *GUID;
    fteo::api::TMyPoints *AllPoints;
    void AddWorkEntrys(TWorkEntry *childitem);
    TWorkEntry* GetEntrybyIndex (int index);
	TWork(int id, enum CodeTypes CType);
   ~TWork();
//      void AddObj(ItemId,CadWoktype:Integer; CadNumber, LotName, AreaPrec, AreaPrecMK: AnsiString):TMyObj;
    };

    //class TWorks: public std::list<TWork> {}; // Список кадастровых работ
    class TWorks: public fteo::api::TMyList<TWork> {}; // Список кадастровых работ

class TWorkList
// Класс Список кадастровых работ для Определенного
// регламента  CadWorkType.
{
public:
    TWorkList();     //Конструктор
  	int CadWorkType; // Тип кадастровых работ (регламент)
	int id;          // pk
	int Count();
    TWorks  *Works;
	bool AddWork(TWork *item);
    TWork *GetWork(int work_id);
    TWork *getworkbyindex(int index);


};

 }
#endif  //CADWORKS_H