#include "stdafx.h"
#include "CLR_baseClasses.h"

namespace fteo {

  namespace NET {

#pragma region Конвертеры для CLR строк definitions
//ANSI char в управляемый string:	
    System::String ^  CharToString(char *str)
		 {
		 if (!str) return L"";
         if ( (int) str == 0xCDCDCDCD)  return L"0xCDCDCDCD";  // 0xCDCDCDCD	Allocated in heap, but not initialize
		 System::String^ g = gcnew System::String(str);
          return  g;
		 
	}

// Охуеешь с ваших строк....clr в native char
	char*   StringtoChar(System::String ^ s) 		 

	{
		using namespace System::Runtime::InteropServices;
		
		return (char*) Marshal::StringToHGlobalAnsi(s).ToPointer();

	}

	wchar_t StringtoWChar(System::String ^ s) {
		 // Convert to a wchar_t*
	char*	cs =StringtoChar(s);
    size_t origsize = strlen(cs) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, cs, _TRUNCATE);
    wcscat_s(wcstring, L" (wchar_t *)");
	return *wcstring;
		};
		
 wchar_t* charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
     mbstowcs(wa,text,size); // This function or variable may be unsafe. Consider using mbstowcs_s instead. 
    //mbstowcs_s(wa,text,size); 
    return wa;
}

#pragma endregion

#pragma region TMyPoint wrapper definitions
using namespace System;
using namespace System::Runtime::InteropServices;

wr_TMyPoint::wr_TMyPoint()
	{
	//this->Name = "";
    this->API = new fteo::api::TMyPoint();
    
	}

	wr_TMyPoint::~wr_TMyPoint()
	{
	 // Get the GCHandle associated with the pinned object based on its
       // address, and free the GCHandle. At this point, the ->API
       // object is eligible for GC.
       GCHandle h = GCHandle::FromIntPtr(IntPtr(this->API));
       h.Free();
	}

    

#pragma endregion

#pragma region TMyPoints wrapper definitions
     
     wr_TMyPoints::wr_TMyPoints()
       {
        this->API = new api::TMyPoints(); //this->API->item_id = -1; // owner not setuped
       }

    void wr_TMyPoints::AppendPoint(wr_TMyPoint^ Value)
    {
      this->API->AppendPoint(Value->API);
    }

    void wr_TMyPoints::AppendPoint(api::TMyPoint *Value)
      {
        this->API->AppendPoint(Value);
      }


    void wr_TMyPoints::AppendPoints(wr_TMyPoints^ items)
    {
      this->API->AppendPoints(items->API);
    }

    wr_TMyPoint^ wr_TMyPoints::GetPoint(int id)
    {
      wr_TMyPoint^ resPoint = gcnew wr_TMyPoint();
      resPoint->API = this->API->getpoint(id);
      return resPoint;
    }

    wr_TMyPoint^ wr_TMyPoints::GetPointbyIndex(int index)
      {
      wr_TMyPoint^ res = gcnew wr_TMyPoint();
      res->API = this->API->getpointbyindex(index);
      return res;
      }
	void wr_TMyPoints::ClearItems()
		{
 		   if (this->API) 
			   this->API->ClearItems();
		};
#pragma endregion

#pragma region TMyContours wrapper definitions
     wr_TMyContours::wr_TMyContours()
      {
       this->API = new api::TMyContours(); //this->API->item_id = -1; // owner not setuped
      }
#pragma endregion

#pragma region TWork:: wrapper definitions
   	
	wr_TWork::wr_TWork()
	{
      this->API = new TWork(-1, CodeTypes::undef);
	}

   System::String^ wr_TWork::ArchNum()
   {
     if (this->API)
     return CharToString(this->API->ArchNum);
       else return "";
   }	

   void wr_TWork::AppendPoints(fteo::NET::wr_TMyPoints ^items)
     {
     this->API->AllPoints->AppendPoints(items->API);
     };
   //------------------------------TWorks:: ----------------------------------
	wr_TWorks::wr_TWorks(int cwType)
	{
    this->API = new  TWorkList();
      this->API->CadWorkType = cwType;
    }

     int    wr_TWorks::Count()
    {
      return this->API->Count();
    }

     void wr_TWorks::AddItem(wr_TWork^ item)
   {
     this->API->AddWork(item->API);
   }

  
    //native
   TWork *wr_TWorks::getitembyIndex(int index)
   {
     TWork *CW = this->API->getworkbyindex(index);
     return CW;
   }

   //wrapped
   wr_TWork ^wr_TWorks::GetItembyIndex(int index)
   {
     wr_TWork^ it = gcnew wr_TWork();
     it->API = this->getitembyIndex(index);
     return it;
     }



   cli::array<System::Drawing::Point>^  fteoPoints2Drawing(fteo::api::TMyPoints* points) {
	   cli::array<System::Drawing::Point>^  PointList = gcnew cli::array<System::Drawing::Point>(points->Count());

	   for (int ip = 0; ip <= points->Count()-1 ; ip++)

		   PointList[ip] =  System::Drawing::Point(points->Items->GetByIndex(ip)->NewOrd->x,
												   points->Items->GetByIndex(ip)->NewOrd->y);


	   return PointList;
	   };

#pragma endregion

#pragma region Утилиты для CLR Forms

   

   void  Lister::ListSpatialElement(System::Windows::Forms::TreeNode^ PointsNode, fteo::api::TMyPoints *Layer) {
	   if ( (!Layer) || (Layer->Items->size() == 0)) return; 
     for (int ip = 0; ip <= Layer->Items->size()-1 ; ip++)
                   {
                   System::Windows::Forms::TreeNode^ ordNode = PointsNode->Nodes->Add(CharToString(Layer->Items->GetByIndex(ip)->Name)+
					   " ("+
                    Layer->Items->GetByIndex(ip)->NewOrd->x.ToString()+"; "+
                    Layer->Items->GetByIndex(ip)->NewOrd->y.ToString()+")");
				   if (Layer->Items->GetByIndex(ip)->OldOrd->x > 0)
                    ordNode->Nodes->Add(Layer->Items->GetByIndex(ip)->OldOrd->x.ToString());
				   if (Layer->Items->GetByIndex(ip)->OldOrd->y > 0)
                    ordNode->Nodes->Add(Layer->Items->GetByIndex(ip)->OldOrd->y.ToString());
                   }
     };

   void  Lister::ListPointsList(System::Windows::Forms::TreeView^ TV, fteo::api::TMyPoints *Layer) {
      TV->Nodes->Clear();
	  if (!Layer) return ; 
	  System::Windows::Forms::TreeNode^ PointsNode = TV->Nodes->Add(Layer->Count().ToString());
	  ListSpatialElement(PointsNode, Layer);

	   };

   void  Lister::ListContours(System::Windows::Forms::TreeView^ TV, fteo::api::TMyContours* cs) {
        if (!cs) return ; 

           TV->BeginUpdate();
           TV->Nodes->Clear();
           fteo::api::TMyContour* c;
		   for (int i = 0; i <= cs->Contour->Count()-1; i++)
             {
               c   = cs->Contour->GetByIndex(i);
               System::Windows::Forms::TreeNode^ root= TV->Nodes->Add(CharToString( c->Definition));
                 System::Windows::Forms::TreeNode^ OUTNode = root->Nodes->Add(CharToString( c->EntitySpatial->SpatialElement(0)->LayerName));
                   ListSpatialElement(OUTNode, c->EntitySpatial->SpatialElement(0));

               for (int is = 1; is <= c->EntitySpatial->Count()-1; is++) {
				   System::Windows::Forms::TreeNode^ PointsNode2 = OUTNode->Nodes->Add(CharToString( c->EntitySpatial->SpatialElement(is)->LayerName));
                   ListSpatialElement(PointsNode2, c->EntitySpatial->SpatialElement(is));
                 };

               root->Expand();
             }
           TV->EndUpdate();
           };




#pragma endregion
    
     } //namespace NET
} //namespace fteo 

