#include "stdafx.h"
#include "CadWorks.h"
#include <stdlib.h>  // for: mallloc 
#include <string.h> //for string: strcpy
#include <rpc.h>

namespace fteo
{
 char* CadWorkTypeToName(int cw)
{
  //char *Res;
 switch (cw)
 {
 case cw_Default : return (char*) "Иные виды работ";      //5000
 case cw_Unknown : return (char*) "Неопределен";      //0
 case cw_OnNew_Partition : return (char*) "Выдел";      //114
 case cw_OnNew_Divide    : return (char*) "Раздел";     //214
 case cw_OnNew_DivideGos : return (char*) "Раздел с измененным земельным участком"; //314
 case cw_OnNew_Repart    : return (char*) "Перераспределение"; //414
 case cw_OnNew_          : return (char*) "Образование из земель"; //514
 case cw_OnNew_Unite     : return (char*) "Объединение";      //614
 case cw_OnExist_Slots : return (char*) "Только образование частей";      //115
 case cw_OnExist_Any   : return (char*) "уточнение границ и площади, уточн.(образование) частей"; //215
 case cw_Zone          : return (char*) "Охранная зона ЛЭП"; //533
 case cw_New_Build     : return (char*) "Здание"; //123
 case cw_New_Construct : return (char*) "Сооружение";
 case cw_New_UnComplete: return (char*) "Объект незавершенного строительства (ОНС) (постановка на учет)";
 case cw_New_Float     : return (char*) "Помещение";

  /*
   124:  Result := 'Здание';
   224:  Result := 'Сооружение';
   324:  Result := 'Объект незавершенного строительства (ОНС) (постановка на учет)';
   424:  Result := 'Помещение';
   
   133:  Result := 'Граница муниципального образования';
   233:  Result := 'Граница населенного пункта';
   333:  Result := 'Границы между субъектами Российской Федерации';
   433:  Result := 'Участок государственной границы Российской Федерации';
   533:  Result := 'Охранная зона ЛЭП';   
   */
 }
 //return *Res;
}

 char* CadWorkTypeToChar(int cw)
 {
   if (cw == 0)
   {
        char *cwCh = " like '%%' ";
        return cwCh;
   }
    char *cwCh =(char*) malloc (sizeof(char)*3);
    char *pref =(char*) malloc (sizeof(char));
   itoa(cw, cwCh, 10);
   strcpy(pref, "= ");
   strcat(pref, cwCh);
  return pref;
 }

 




 /*-------------------------------------------------------------------
 char* GUIDGenerator()
   {
   UUID uuid;
    ZeroMemory(&uuid, sizeof(UUID));

    // Create uuid or load from a string by UuidFromString() function
    ::UuidCreate(&uuid);

    // If you want to convert uuid to string, use UuidToString() function
    WCHAR* wszUuid = NULL;
    ::UuidToStringW(&uuid, (RPC_WSTR*)&wszUuid);
    if (wszUuid != NULL)
    {
        ::RpcStringFree((RPC_WSTR*)&wszUuid);
        wszUuid = NULL;
    }

   //  wstring guid; 
   // guid = wszUuid;            // ERROR: no operator "=" matches these operands operand types are: std::string = WCHAR*

    return (char*) wszUuid;
   }
  
*/
/*
 Color^ Status::StatusToColor(int status)
	 {
	 switch (status)
		{
     case 0:   return  System::Drawing::Color::Red;
     case 	 6:   return  System::Drawing::Color::Blue; //Архивный
     case    61:   return  System::Drawing::Color::Blue;//Архивный
	 case 70:   return  System::Drawing::Color::Green; //пункт ОМС, ГГС
	 case 71:   return  System::Drawing::Color::Lime;
	 case 41:   return  System::Drawing::Color::Black; ///учтеный
	 case 4 :   return  System::Drawing::Color::Black; //учтеный
		 default: return  System::Drawing::Color::Black;
		 }
	 }
*/	

//=======================================================================================
///--Описания--Класс  Кадастровые работы---
	//Конструктор
	TWork::TWork(int id, enum CodeTypes CType)
	{
	  
		if (CType == CodeTypes::undef) this->CodeType = CODETYPE_UNDEF;
		if (CType == CodeTypes::Exist) this->CodeType = CODETYPE_EXIST;
		if (CType == CodeTypes::New)   this->CodeType = CODETYPE_NEW;
	//	this->Parent_id = parent_id;
        this->id = id;
		this->fCount = 0;
        this->AllPoints = new fteo::api::TMyPoints (id);
		this->EntryList = new std::list<TWorkEntry>;

	}

    int TWork::EntryCount()
	{
      return this->EntryList->size();
	}
    void TWork::AddWorkEntrys(TWorkEntry *childitem)
      {
        std::list<TWorkEntry>::const_iterator  it = this->EntryList->end();  // at end
        this->EntryList->insert(it, *childitem);
      };

//=======================================================================================
    TWorkEntry* TWork::GetEntrybyIndex (int index)
      {
        std::list<TWorkEntry>::const_iterator pt = this->EntryList->begin();
          std::advance(pt, index);
          return &pt._Ptr->_Myval;
  //        return ;//nullptr;
      }

    TWork::~TWork()
    {
	 free(this->EntryList);
    }


//====================================================================
//
//
    TWorkList::TWorkList()
    {
      this->Works = new TWorks();// std::list<TWork>();

    }

bool   TWorkList::AddWork(TWork *item)
	{
	 std::list<TWork>::const_iterator  it = this->Works->end();  // at end - в конец списка
	 this->Works->insert(it, *item);
	                                   it = this->Works->end();  // at end - в конец списка

                                   //    &it._Ptr->_Myval.GUID = fteo::GUIDGenerator();
	 return false;
	}
int    TWorkList::Count()
{
  return this->Works->size();
}

TWork *TWorkList::GetWork(int work_id)
{
  //std::list<TWork>::const_iterator pt = this->Works->begin();
   TWorks::const_iterator pt = this->Works->begin();
  for (int i = 0; i <= Works->size()-1; i++)
      {  std::advance(pt,i);
        if (pt->id == work_id)   return &pt._Ptr->_Myval;
      }
}

TWork *TWorkList::getworkbyindex(int index)
{
  /* 
  std::list<TWork>::const_iterator pt = this->Works->begin();
   std::advance(pt,index);
     return &pt._Ptr->_Myval;*/
  return  this->Works->GetByIndex(index);
}

} //namespace