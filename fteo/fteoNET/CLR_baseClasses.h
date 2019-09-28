//////////////////////////////////////////////////////////////////




////                NET WRAPPERS for fteo baseClasses       //////



//   @2015 Fixosoft
//////////////////////////////////////////////////////////////////

#include "fteo_core.h"

namespace fteo {
	namespace NET {

#ifndef  CLR_BASECLASSES_H
#define  CLR_BASECLASSES_H

#pragma region  ** Конвертеры для CLR строк **
		// 
		System::String^ CharToString(char* str);
		char* StringtoChar(System::String^ s);
		wchar_t StringtoWChar(System::String^ s);
		wchar_t* charToWChar(const char* text);
		//РАзработать!!!:  LPCWSTR StringtoLPCWSTR(System::String ^ s); //  __nullterminated

#pragma endregion

#pragma region ** Wrappers для native "BaseClasses" **
//////////////////////////////////////////////////////////////////////////////////////
//TMyPoint wrapper
		public ref class wr_TMyPoint
		{
		public:
			fteo::api::TMyPoint* API;
			wr_TMyPoint();
			~wr_TMyPoint();
			property int id
			{
				int get() { return  this->API->id; }
				void set(int value) { this->API->id = value; this->API->changed = true; }
			}
			property double x
			{
				double get() { return  this->API->NewOrd->x; }
				void set(double value) { this->API->NewOrd->x = value; this->API->NewOrd->changed = true; }
			}
			property double y
			{
				double get() { return  this->API->NewOrd->y; }
				void set(double value) { this->API->NewOrd->y = value; this->API->NewOrd->changed = true; }
			}
			property double z
			{
				double get() { return  this->API->NewOrd->z; }
				void set(double value) { this->API->NewOrd->z = value; this->API->NewOrd->changed = true; }
			}

			property System::String^ Name
			{
				System::String^ get() { return  CharToString(this->API->Name); }
				void set(System::String^ value) { this->API->Name = StringtoChar(value); this->API->changed = true; }
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////
		
		/*public class MyClass
		{
		public:
			MyClass();
			~MyClass();

		private:

		};

		MyClass::MyClass()
		{
		}

		MyClass::~MyClass()
		{
		}
	
		*/
		
		
		//TMyPoints wrapper
		public ref class wr_TMyPoints
		{

		public: wr_TMyPoints();
		public:  fteo::api::TMyPoints* API;
		public: void AppendPoint(wr_TMyPoint^ Value);
		public: void AppendPoint(api::TMyPoint* Value);
				void AppendPoints(wr_TMyPoints^ items);
				wr_TMyPoint^ GetPoint(int id);
				wr_TMyPoint^ GetPointbyIndex(int index);
				static int check = 1975;
				System::String^ Comment;
				char* comment;
				void ClearItems();
				property int id
				{
					int get() { return  this->API->item_id; }
					void set(int value) { this->API->item_id = value; }
				}

				property int Count
				{
					int get() { return  this->API->Count(); }
				}
		};

		//////////////////////////////////////////////////////////////////////////////////////
		//Wrapper NET для Контуров
		public ref class wr_TMyContours
		{

		public: wr_TMyContours();
		public:  fteo::api::TMyContours* API;
		};

		//////////////////////////////////////////////////////////////////////////////////////
		//Wrapper NET для работы
		public ref class wr_TWork
		{
		public:  fteo::TWork* API;
				 wr_TWork();
				 void AppendPoints(wr_TMyPoints^ items); //Добавить в AllPoints точки
				 System::String^ ArchNum();
		};

		//////////////////////////////////////////////////////////////////////////////////////
		//Wrapper NET для списка работ одного типа
		public ref class wr_TWorks
		{
		public:  TWorkList* API;
				 wr_TWorks(int cwType);
				 int Count();
				 void AddItem(wr_TWork^ item);
				 TWork* getitembyIndex(int index); //native
				 wr_TWork^ GetItembyIndex(int index); //cli

		};

#pragma endregion

		//////////////////////////////////////////////////////////////////////////////////////
		// Утилиты для Viewera. Массив fteo::api в массив CLI::Drawing
		cli::array<System::Drawing::Point>^ fteoPoints2Drawing(fteo::api::TMyPoints points);


#pragma region Утилиты для CLR Forms. Классы для отображения объектов fteo::api
		public ref class Lister {
			void  ListSpatialElement(System::Windows::Forms::TreeNode^ PointsNode, fteo::api::TMyPoints* Layer);
		public: void  ListPointsList(System::Windows::Forms::TreeView^ TV, fteo::api::TMyPoints* Layer);
		public: void  ListContours(System::Windows::Forms::TreeView^ TV, fteo::api::TMyContours* cs);
		};
#pragma endregion

#endif // CLR_BASECLASSES_H
	} //namespace NET
} //namespace fteo 

