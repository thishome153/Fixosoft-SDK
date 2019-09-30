#include "stdafx.h"
#include <iomanip> //std::precission
#include <fstream> //std::ofstream
#include <sstream> //std::istringstream


//если Нужны классы:
//#include "mitab.h";
//#include "mitab_priv.h";
//Если нужны только API:
//#include "mitab_capi.h";

//#include "cpl_string.h"
#include "fteo_io.h"
#include "fteo_cpl.h"


namespace fteo
{
	namespace api
	{



		TextWriter::TextWriter(bool writepointpref, double deltageopoint, char* filename)
		{
			this->writePointPref = writepointpref;
			this->DeltaGeopoint = deltageopoint;
			this->FileName = filename;
		};

		void TextWriter::SaveAsDxf(void* Entity)
		{

		}

		using namespace std;

		// Write any data in vector container onto file
		template<typename T>
		int  SaveVectorToFile(const char* filename, vector<T>& buffer)
		{
			if (buffer.empty())
			{
				cout << "There is nothing to save" << endl;
				return -1;
			}

			FILE* f = fopen(filename, "wb");
			if (!f)
			{
				cout << "Opening file " << filename << " failed" << endl;
				return -1;
			}

			size_t count = fwrite(&buffer[0], sizeof(T), buffer.size(), f);
			if (count != buffer.size())
			{
				cout << "Error occured during saving to file " << filename << endl;
				return -1;
			}

			fclose(f);

			return 0;
		}


		// Write any data in buffer pbData onto file
		// unsigned long -> DWORD
		// unsigned char -> BYTE
		int TextWriter::SaveBodyToFile(unsigned long cbData, unsigned char *pbData)
		{
			vector<unsigned char> message(cbData);
			copy(pbData, pbData + cbData, message.begin());
			return (fteo::api::SaveVectorToFile<unsigned char>(this->FileName, message));
		}

		// Функция чтения данных из файла в vector. Вычитывается весь файл целиком.
		int TextReader::ReadFileToVector(const char* filename, vector<unsigned char>& buffer)
		{
			enum { bytesSize = 512 };

			unsigned long bytesRead(1);
			char buf[bytesSize];

			FILE* f = fopen(filename, "r+b");

			if (!f)
			{
				cout << "Opening file " << filename << " failed" << endl;
				return -1;
			}

			while (!feof(f))
			{
				bytesRead = (unsigned long)fread(buf, 1, bytesSize, f);

				if (bytesSize != bytesRead && ferror(f))
					return -1;

				std::copy(buf, buf + bytesRead, back_inserter(buffer));
			}
			fclose(f);

			return 0;
		}




		void TextWriter::SaveAsFteo15(TMyPoints* Entity)
		{
			if (!Entity) return;
			std::ofstream ofs;
			ofs.open(this->FileName, std::ofstream::out | std::ofstream::app);
			ofs << "#Fixosoft NumXYZD data format V2015" << "\n";
			ofs << "#Producer: fteoApplication 6'+ DateTimeToStr(Now))" << "\n";
			ofs << "#Parent_id = " << Entity->item_id << "\n";
#ifdef VC120
			ofs << "# Файл формата FTEO - Разделители полей tab. PlatformToolSet: C++ Visual 2013 CE \n";
#else ofs << "# Файл формата FTEO - Разделители полей tab. PlatformToolSet: C++ Visual 2008 \n";
#endif

			ofs << "# Поля файла:\n";
			ofs << "# ИмяТочки,X,Y,Z,Описание-Код\n";
			ofs << "Polygon \n"; //<< Entity->Definition<<"\n"; 

			for (int i = 0; i <= Entity->Count() - 1; i++)
			{
				double tst = Entity->getpointbyindex(i)->NewOrd->x;
				tst = Entity->getpointbyindex(i)->NewOrd->y;
				tst = Entity->getpointbyindex(i)->NewOrd->DeltaGeopoint;

				ofs << std::fixed << std::setprecision(this->DeltaGeopoint)
					<< Entity->getpointbyindex(i)->Name
					<< "\t" << Entity->getpointbyindex(i)->NewOrd->x
					<< "\t" << Entity->getpointbyindex(i)->NewOrd->y
					<< "\t" << Entity->getpointbyindex(i)->NewOrd->z
					<< "\t" << Entity->getpointbyindex(i)->Description << "\n";
			}

			ofs << "EndPolygon\n";
			ofs.close();
			/*


			*/
		};




		//------------------------------TcsvString:: ----------------------------------

		TcsvString::TcsvString(char* InitString) {
			this->src = InitString;
			this->sz = strlen(src);
			this->fDefinition = " ";
		};

		//разбор элемента до следующего элемента ;
		void TcsvString::ParseItem(int& index, char*& Dest) {
			int toIndex = 0;
			int sz = strlen(src);
			if (index == sz) return; // end of string
			if (src[0] == ';') return; // wrong string/comment
			Dest = (char*)malloc(sizeof(char) * sz); // на всю длину !!!на всякой случай
			while (src[index] != ';')
			{
				Dest[toIndex++] = src[index++];
				if (index == sz) break; // если ёто последний
			};
			Dest[toIndex] = '\0'; // обрежем строку
			index++;
		};


		void TcsvString::Parse() {
			if (src[0] == ';') return;   // comment string
			int i = 0;
			while (i < sz)
			{
				ParseItem(i, this->fDefinition);
				ParseItem(i, this->Pref);
				ParseItem(i, this->Number);
				ParseItem(i, this->oldx);
				ParseItem(i, this->oldy);
				ParseItem(i, this->x);
				ParseItem(i, this->y);
				ParseItem(i, this->Method);
				ParseItem(i, this->f);
				ParseItem(i, this->r);
				ParseItem(i, this->Mt);
				ParseItem(i, this->Descr);
			}
		};


		/*
		Value	Usage
		0xCDCDCDCD	Allocated in heap, but not initialized
		0xDDDDDDDD	Released heap memory.
		0xFDFDFDFD	"NoMansLand" fences automatically placed at boundary of heap memory. Should never be overwritten. If you do overwrite one, you're probably walking off the end of an array.
		0xCCCCCCCC	Allocated on stack, but not initialized
		*/

		char* TcsvString::RemoveBracets(char* Name) {

			if ((int)Name == 0xCDCDCDCD)  return "0xCDCDCDCD";  // 0xCDCDCDCD	Allocated in heap, but not initialize

			int toIndex = 0;
			int index = 1; //skip left [
			char* res = (char*)malloc(sizeof(char) * sz);
			while (Name[index] != ']')
			{
				res[toIndex++] = Name[index++];
				if (index == sz) break; // если ёто последний
			}
			res[toIndex] = '\0'; // обрежем строку    
			return res;
		};

		char* TcsvString::Definition() {
			if (this->fDefinition)
				return
				this->RemoveBracets(this->fDefinition);
			else return "";
		};

		char* TcsvString::ParentDefinition() {
			int toIndex = 0;
			int index = 1; //skip left [
			char* res = (char*)malloc(sizeof(char) * sz);

			while (fDefinition[index] != '.')
			{
				res[toIndex++] = fDefinition[index++]; // [1975.03];
				if (index == sz) break; // если ёто последний
				if (fDefinition[index] == ']') break; // если это не child
			}
			res[toIndex] = '\0'; // обрежем строку    
			return res;
		};

		// Весь ма небезопасный метод: надо бы try
		TMyPoint* TcsvString::AsPoint() {

			TMyPoint* res = new TMyPoint();
			res->Name = this->Number;
			res->OldOrd->x = atof(this->oldx);
			res->OldOrd->y = atof(this->oldy);
			res->NewOrd->x = atof(this->x);
			res->NewOrd->y = atof(this->y);
			res->DeltaGeopoint = atof(this->Mt);
			res->GeopointZacrep = this->Descr;
			return res;
		};

		bool TcsvString::isChild() {
			const char* src2 = fDefinition;
			int szd = strlen(src2);
			for (int i = 0; i <= szd - 1; i++)
			{
				if (src2[i] == '.')
				{
					return true;
				};
			};
			return false;
		};

		int TcsvString::Status() {
			if (this->Pref)   return 0;
			else return 4;
		};

		/*------------------------------TCsvFile:: ----------------------------------
		int  TCsvFile::OpenCSV (const char *pszFname) {

				 m_eAccessMode = TABRead; //распишем по дефолту
			 char *pszTmpFname = NULL;
			m_pszFname = CPLStrdup(pszFname);
			pszTmpFname = CPLStrdup(m_pszFname);
			this->pszFeatureClassName =  GetBasename(m_pszFname);

			if	(this->Open(pszFname, "r") != 0) // Теперь для файла вызываем его родный метод Open,
												 // который далее обычный системный
												 // fopen юзает
			  {
			   Close();
				return -1;
			  };

			return 0;
			};
		*/



		/**
		 * Default constructor.
		 */
		TMydxf_DLInterface::TMydxf_DLInterface() {
			this->dxfPolyGons = new TMyContours();
			this->dxfPoints = new TMyPoints();
		}


		/**
		 * Sample implementation of the method which handles layers.
		 */
		void TMydxf_DLInterface::addLayer(const DL_LayerData& data) {
			printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
			printAttributes();
		}

		/**
		 * Sample implementation of the method which handles point entities.
		 */
		void TMydxf_DLInterface::addPoint(const DL_PointData& data) {
			printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
				data.x, data.y, data.z);
			TMyPoint* p = new TMyPoint();
			p->NewOrd->x = data.x;
			p->NewOrd->y = data.y;
			p->NewOrd->z = data.z;
			this->dxfPoints->AppendPoint(p);
			printAttributes();
		}

		/**
		 * Sample implementation of the method which handles line entities.
		 */
		void TMydxf_DLInterface::addLine(const DL_LineData& data) {
			printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
				data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
			printAttributes();
		}

		/**
		 * Sample implementation of the method which handles arc entities.
		 */
		void TMydxf_DLInterface::addArc(const DL_ArcData& data) {
			printf("ARC      (%6.3f, %6.3f, %6.3f) %6.3f, %6.3f, %6.3f\n",
				data.cx, data.cy, data.cz,
				data.radius, data.angle1, data.angle2);
			printAttributes();
		}

		/**
		 * Sample implementation of the method which handles circle entities.
		 */
		void TMydxf_DLInterface::addCircle(const DL_CircleData& data) {
			printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f\n",
				data.cx, data.cy, data.cz,
				data.radius);
			printAttributes();
		}


		/**
		 * Sample implementation of the method which handles polyline entities.
		 */
		void TMydxf_DLInterface::addPolyline(const DL_PolylineData& data) {
			TMyContour* c = new TMyContour();
			c->Definition = "dxfPolyLIne";

			this->dxfPolyGons->Append(c);
			//there was defaults: 
		   //   printf("POLYLINE \n");
		   //   printf("flags: %d\n", (int)data.flags);
		   //   printAttributes();
		}


		/**
		 * Sample implementation of the method which handles vertices.
		 */
		void TMydxf_DLInterface::addVertex(const DL_VertexData& data) {

			TMyPoint* Vertx = new  TMyPoint();
			Vertx->NewOrd->x = data.y;
			Vertx->NewOrd->y = data.x;
			Vertx->NewOrd->z = data.z;
			// в последний контур? типа текущий. Тогда чисто непонятно, почему одни бабки впереди, а другие сзади!
			this->dxfPolyGons->Contour->GetByIndex(this->dxfPolyGons->Contour->Count() - 1)->EntitySpatial->AddElementPoint(Vertx, 0);

			printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
				data.x, data.y, data.z,
				data.bulge);
			printAttributes();
		}


		void TMydxf_DLInterface::add3dFace(const DL_3dFaceData& data) {
			printf("3DFACE\n");
			for (int i = 0; i < 4; i++) {
				printf("   corner %d: %6.3f %6.3f %6.3f\n",
					i, data.x[i], data.y[i], data.z[i]);
			}
			printAttributes();
		}


		void TMydxf_DLInterface::printAttributes() {
			printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
			printf(" Color: ");
			if (attributes.getColor() == 256) {
				printf("BYLAYER");
			}
			else if (attributes.getColor() == 0) {
				printf("BYBLOCK");
			}
			else {
				printf("%d", attributes.getColor());
			}
			printf(" Width: ");
			if (attributes.getWidth() == -1) {
				printf("BYLAYER");
			}
			else if (attributes.getWidth() == -2) {
				printf("BYBLOCK");
			}
			else if (attributes.getWidth() == -3) {
				printf("DEFAULT");
			}
			else {
				printf("%d", attributes.getWidth());
			}
			printf(" Type: %s\n", attributes.getLinetype().c_str());
		}



		//------------------------------TextReader:: ----------------------------------
		TextReader::TextReader()
		{
			this->ClassName = "Fixosoft format reader";
		}

		//"#Fixosoft NumXYZD data format V2015"
		TMyContours* TextReader::ImportNXYZD2015File(const char* pszFname)
		{

			TMyContours* Contours = new TMyContours(); // результат сюда собирать будем
			TMyFile* csvFile = new TMyFile();
			const char* pszLine;
			char** papszToken;
			if (csvFile->Open(pszFname, "r") == 0)
			{
				// Проверяем файл на формат по первой строчке:
				if (((pszLine = csvFile->GetLine()) != NULL) && !(EQUALN(pszLine, "#Fixosoft NumXYZD data format V2015", 7))) //Если это не наш файл? то нах
				{
					csvFile->Close();
					return NULL;
				};
				// сканируем файл по-строчно до конца:
				while (((pszLine = csvFile->GetLine()) != NULL) && !(csvFile->GetEof_() == true))
				{
					if (!(EQUALN(pszLine, "#", 1))) //если это не комментарий
					{
						//Разберем строку на токены по признаку "tab #09 \t"  
						papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine, "\t", true, false);
						//if ((fteo::cpl::CSLCount(papszToken) == 8) && EQUALN(pszLine,"[",1))
						// this->parseCSVContour(pszLine, Contours);				   
						fteo::cpl::CSLDestroy(papszToken);
					};
				};
			};
			return Contours;


		}

		bool TextReader::parseCSVContour(char* FileString, TMyContours* res) {

			TcsvString* CSVString = new TcsvString(FileString);
			CSVString->Parse();
			char* test = CSVString->src;
			if (!CSVString->isChild())
			{
				TMyContour* CurCnt = res->GetContourbyDef(CSVString->Definition());
				if (!CurCnt)
					CurCnt = res->Add(CSVString->Definition());
				CurCnt->EntitySpatial->AddElementPoint(CSVString->AsPoint(), 0); // добавим во внешний - 0 
			}
			else  //child:
			{
				TMyContour* ParentL = res->GetContourbyDef(CSVString->ParentDefinition());
				if (ParentL)
				{
					// chld
					TSpatialElement* chld = ParentL->EntitySpatial->SpatialElement(CSVString->Definition());
					if (!chld)  chld = ParentL->EntitySpatial->AddChild(CSVString->Definition()); // если нет то добавить слой для child
					chld->AppendPoint(CSVString->AsPoint());
				};

			};
			return false;
		};

		bool TextReader::parseCSVContour(std::string FileString, fteo::api::TMyContours* res)
		{
			this->parseCSVContour(fteo::api::str2char(FileString), res);
			return false;
		};


		/**
		* Чтение полигонов из dxf. (Замкнутые полилинии)
		*/
		TMyContours* TextReader::Read_dxf(const char* pszFname) {
			TMydxf_DLInterface* dxfInterface = new TMydxf_DLInterface();
			DL_Dxf* dxf = new DL_Dxf();
			if (!dxf->in(pszFname, dxfInterface)) { // if file open failed
				return NULL;
			};

			//	 dxf->in
			return dxfInterface->dxfPolyGons;
			//return NULL; // определен stdio.h

		};

		/*.......................................................................................
		 *        Чтение CSV через мои классы, заимствованные в miTab
		 *  Разбор csv TechnoCADа через CSVFile::TmyFile (надерганный из CPL/MiTab)
		 *  По отношению к русским именам/путям файлов работает корректно
		 *
		 */
		TMyContours* TextReader::Read_csv_TechnoCADF(const char* pszFname) {
			TMyContours* Contours = new TMyContours(); // результат сюда собирать будем
			TMyFile* csvFile = new TMyFile();
			const char* pszLine;
			char** papszToken;
			if (csvFile->Open(pszFname, "r") == 0)
			{
				// Проверяем файл на формат по первой строчке:
				if (((pszLine = csvFile->GetLine()) != NULL) && !(EQUALN(pszLine, "Контур;", 7))) //Если это не наш файл? то нах
				{
					csvFile->Close();
					return NULL;
				};
				// сканируем файл по-строчно до конца:
				while (((pszLine = csvFile->GetLine()) != NULL) && !(csvFile->GetEof_() == true))
				{
					if (!(EQUALN(pszLine, ";;;;;;;;;;;", 11))) //если это не комментарий
					{
						//Разберем строку на токены по признаку ";"  
						papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine, "[;]", true, false);
						if ((fteo::cpl::CSLCount(papszToken) == 8) && EQUALN(pszLine, "[", 1))
							this->parseCSVContour(pszLine, Contours);
						fteo::cpl::CSLDestroy(papszToken);
					};
				};
			};
			return Contours;
		};

		//.......................................................................................
		//     Разбор csv TechnoCADа через ifstream. Оказался дефектным по отношению к русским 
		//    именам/путям файлов 
		TMyContours* TextReader::Read_csv_TechnoCAD(std::string FileName) {
			std::string FileStringW;
			std::ifstream inFile(FileName.c_str());

			if (!inFile.is_open()) return NULL;

			std::getline(inFile, FileStringW);

			if (FileStringW != "Контур;Префикс номера;Номер;Старый X;Старый Y;Новый X;Новый Y;Метод определения;Формула;Радиус;Погрешность;Описание закрепления")
				return NULL; // это не наш файлик

			TMyContours* Contours = new TMyContours(); // результат сюда собирать будем
			/*
			// istringstream запросто все разбирает:
			 while ( std::getline(inFile,FileStringW))
		  {
			 // std::istringstream linestream(FileStringW);
			 // std::string item;
			 // int itemnum = 0;
			 // while (getline (linestream, item, ';'))   itemnum++; // вот так, в одну строку
			 }
			*/
			//но мы пойдем другим путем (долгим и трудным!):
			while (std::getline(inFile, FileStringW))
			{
				while (FileStringW == ";;;;;;;;;;;")   std::getline(inFile, FileStringW); // пропустить комментарии
				parseCSVContour(FileStringW, Contours);
			};

			inFile.close();
			return Contours;
		};
		//..........mitab API  ..........................................................................
		TMyContours* TextReader::Read_mif_MITabAPI(char* FileName) {
			/*

			  TMyContours  *Contours = new TMyContours(); // результат сюда собирать будем
		 //     if (FileStringW != "Version 450")     return NULL; // это не наш файлик

			// API :
			  mitab_handle h =  mitab_c_open(FileName);
				int FeaturesCount = mitab_c_get_feature_count(h);

				 // Перечисление не с нуля !?
				for (int i =0; i <= FeaturesCount-1; i++)
					{
					  mitab_feature Feature = mitab_c_read_feature (h, i);
					  if ( Feature)
						  {
						  char* Field0Value = (char*) mitab_c_get_field_as_string(Feature,0);
						  TMyContour* newRegion = new TMyContour();
						  newRegion->Definition = Field0Value;

					  if (mitab_c_get_type(Feature) == TABFC_Region)
						  {

						  //Elements
						 int ElementCount =  mitab_c_get_parts(Feature);
						 for (int pi = 0; pi <= ElementCount-1; pi++)
							 {
							  int PointCount =  mitab_c_get_vertex_count(Feature, pi);
							  for (int vi = 0; vi <= PointCount-1; vi++)
								  {
									TMyPoint* pt= new TMyPoint();
									pt->NewOrd->x = mitab_c_get_vertex_y(Feature, pi,vi);
									pt->NewOrd->y = mitab_c_get_vertex_x(Feature, pi,vi);
									newRegion->EntitySpatial->AddElementPoint(pt, pi);
								  };
							 }
						  }
					  Contours->Append(newRegion);
						  }
					}

		   return Contours;*/
			return NULL;
		};


		//..........Fixosoft classes ....................................................................
		TMyContours* TextReader::Read_mif(char* FileName) {
			TMyContours* res = new TMyContours();

			TMifFile* mif = new TMifFile();
			if (mif->Open(FileName) == 0)
			{
				int PolygonCount = mif->m_nRegions;
				for (int i = 1; i <= mif->m_nRegions; i++)
				{
					if (mif->GotoFeature(i) == 0)
					{
						TMapInfoRegion* Pr = mif->GetFeatureRef(i);
						res->Append(Pr);
					}
				};
			}
			return res;
		};

	}//namespace api
}//name