/***********************************************************************************
* Fixosoft 2014
/***********************************************************************
*-----------Утилиты обработки строк------------------------
*   О//уеешь с ваших строк....
*
***********************************************************************/


#include "stdafx.h"
#include "MyStrMarshal.h"
#include "string"

//using System;



// Преобразование строки .NET в указатель на строку WINAPI/
// Понадобился, однако для вызова функций чистого С 
char* StringtoChar(System::String^ s)
{
	using namespace Runtime::InteropServices;
	return (char*)Marshal::StringToHGlobalAnsi(s).ToPointer();
}

// Multibyte string to managed string:
System::String^ LPTSTRToString(LPTSTR str)
{
	using namespace Runtime::InteropServices;
	String^ g = Marshal::PtrToStringUni((IntPtr)str);
	return  g;
}

// native c++ string to managed string:
System::String^ LPSTRToString(LPSTR  str) 		 // Для nonUnicode:
{
	using namespace Runtime::InteropServices;
	String^ g = Marshal::PtrToStringUni((IntPtr)str);
	return  g;
}


// native c++ string to managed string:
System::String^ CharToString(char* str)
{
	if (!str) return L"";
	using namespace Runtime::InteropServices;
	String^ g = Marshal::PtrToStringAnsi((IntPtr)str);
	return  g;
}

// Multibyte string to std C++ string:
bool LPWSTRTostd_string(std::string& s, const LPWSTR pw,	UINT codepage )
{
	bool res = false;
	char* p = 0;
	int bsz;
	bsz = WideCharToMultiByte(codepage,
		0,
		pw, -1,
		0, 0,
		0, 0);
	if (bsz > 0) {
		p = new char[bsz];
		int rc = WideCharToMultiByte(codepage,
			0,
			pw, -1,
			p, bsz,
			0, 0);
		if (rc != 0) {
			p[bsz - 1] = 0;
			s = p;
			res = true;
		}
	}
	delete[] p;
	return res;
}

// Multibyte string to managed string:
System::String^ LPWSTRToString(const LPWSTR pw, UINT codepage)
{
	std::string converted;
	if (LPWSTRTostd_string(converted, pw, codepage))
	{
		return	gcnew String(converted.c_str());
	}
	return nullptr;
}


//Серийный номер hex в строку
//Полное извращение для серийного номера в HEX
System::String^ PBYTEToStr(PBYTE x, const int cb)
{
	char* str = NULL;
	str = new char[2 * cb + 1]; // массив нужен в два раза длиньше? один байт в HEX -это FF, к примеру - два символа.
	str[cb - 1] = '\0';
	int cntr = cb;
	for (int i = 0; i <= cb - 1; i++) {
		cntr--;
		sprintf(str + i * 2, "%02x", x[cntr]);

	}
	return CharToString(str);
}