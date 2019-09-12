//2014 Fixosoft String routines
// Выстраданая хуйня

#include <windows.h>  //типы основные

using namespace System; // типы .NET
	

#ifndef MarshUtils_h_INCLUDED // типа защита от множественного включения
#define MarshUtils_h_INCLUDED


//  Декларации заголовков функций - интерфейсная часть
 char* StringtoChar(System::String ^ s); 		 // Охуеешь с ваших строк....
 System::String ^ LPTSTRToString(LPTSTR str); 		 // Для Unicode:
 System::String ^ LPSTRToString(LPSTR  str); 		 // Для Unicode:
 System::String ^ CharToString(char *str); //Для ANSI
 System::String ^ PBYTEToStr(PBYTE x, const int cb);
#endif /* _SignerUtils_h_INCLUDED */