
#include "fteo_base.h"

#ifndef  CADWORKS_H
#define  CADWORKS_H

namespace fteo {

  




#define CODETYPE_UNDEF "000"
#define CODETYPE_NEW   "014"
#define CODETYPE_EXIST "015"

enum CodeTypes {undef,New,Exist};

// CadWorkTypes - ���������� ����������� �����
// 014
#define cw_OnNew_Partition 114   //����� 
#define cw_OnNew_Divide    214   //������
#define cw_OnNew_DivideGos 314   //������ � ��������
#define cw_OnNew_Repart    414   //�����������������
#define cw_OnNew_          514   //'����������� �� ������';
#define cw_OnNew_Unite     614   //�����������
// 015
#define cw_OnExist_Slots   115   //������ �����
#define cw_OnExist_Any     215   //���������


#define cw_New_Build       123  // ������
#define cw_New_Construct   223  // ����������
#define cw_New_UnComplete  323  // ������ �������������� ������������� (���) (���������� �� ����)
#define cw_New_Float       423  // ���������


#define cw_Zone            533   //�������� ���� ���;   

#define cw_Default 5000     //���� ���� �����
#define cw_Unknown 0     //�������������� ���




 char* CadWorkTypeToName(int cw); 
 char* CadWorkTypeToChar(int cw); 
/* int str2int (const std::string &str); */
 char* GUIDGenerator();



//������ ��� ��������� ������� ��. ����������� �� Fteo5
 struct DBLayerItem
   {
      
     int Layer_ID;                  // PK ������� LAYERS
     int ParentLayer_ID;            // ������������ ������ ���   Layer_Type= IN � Level=2
     int Itemid; // Slot id
     int Lot_ID;                    // PK ��
     int ParentLot_ID;
     int Order;                     // ������� ����� � MyObj
     int Level;                     // �������: 1= OUT, 2= IN
     char *Definition;
     char *Layer_Type;                 //  IN, OUT, ANOTHER
   } ;

 struct  DBAreaItem {
     int Lot_ID;
     int Layer_ID;
     int Area_ID;
     int Area_order;
     int Point_ID;
     char *Def; //�������� ������� �������
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
	    int Parent_id;    //parent_id LOTtable ��� �������  ������ (child = 1)
        int EntryType;
        char *Name; // archnum
        char *LotName; // archnum
	  };

//----�����  ����������� ������ (��. �� . � �.�.)---
  class TWork {
private: int fCount;
public:
	int CadWorkType;
    int id;           //id LOTTABLE ��� ������ ���� child=0
    int EntryCount();
	int Status;
	char *CodeType;
    char *ArchNum; //����� ���� (������, ��, ��)
	//void *Items;   // ������ ��������� ��������, ��� � �.�. ��������� � ������ ������ . [0] - ������������ �������
	std::list<TWorkEntry> *EntryList; // ������ ��������� ��������, ��� � �.�. ��������� � ������ ������ . [0] - ������������ �������
    char *Conclusion;
    char *GUID;
    fteo::api::TMyPoints *AllPoints;
    void AddWorkEntrys(TWorkEntry *childitem);
    TWorkEntry* GetEntrybyIndex (int index);
	TWork(int id, enum CodeTypes CType);
   ~TWork();
//      void AddObj(ItemId,CadWoktype:Integer; CadNumber, LotName, AreaPrec, AreaPrecMK: AnsiString):TMyObj;
    };

    //class TWorks: public std::list<TWork> {}; // ������ ����������� �����
    class TWorks: public fteo::api::TMyList<TWork> {}; // ������ ����������� �����

class TWorkList
// ����� ������ ����������� ����� ��� �������������
// ����������  CadWorkType.
{
public:
    TWorkList();     //�����������
  	int CadWorkType; // ��� ����������� ����� (���������)
	int id;          // pk
	int Count();
    TWorks  *Works;
	bool AddWork(TWork *item);
    TWork *GetWork(int work_id);
    TWork *getworkbyindex(int index);


};

 }
#endif  //CADWORKS_H