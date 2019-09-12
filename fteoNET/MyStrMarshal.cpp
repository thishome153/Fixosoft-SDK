/***********************************************************************************
* Fixosoft 2014
*-----------������� ��������� �����------------------------
* // �//����� � ����� �����....
//***********************************************************************************/

#include "stdafx.h"
#include "MyStrMarshal.h"
#include "string"

//using System;



// �������������� ������ .NET � ��������� �� ������ WINAPI/
// �����������, ������ ��� ������ ������� ������� � 
char* StringtoChar(System::String ^ s)
		 {
			 using namespace Runtime::InteropServices;
    		   return (char*)Marshal::StringToHGlobalAnsi(s).ToPointer();
		 }

		 // ��� nonUnicode:
// � ��� ����� ������������? ����� �� ���� WINAPI �������� ������ .NET:
System::String ^  LPTSTRToString(LPTSTR str)
		 {
			 using namespace Runtime::InteropServices;
			 String^ g = Marshal::PtrToStringUni((IntPtr)str);
          return  g;
		 }

// ANSI char � ����������� string:
System::String ^  CharToString(char *str)
		 {
			 if (!str) return L"";
			 using namespace Runtime::InteropServices;
			 String^ g = Marshal::PtrToStringAnsi((IntPtr)str);
          return  g;
		 }

//���� ������ ��� ����� �� �����:
System::String ^  LPSTRToString(LPSTR  str) 		 // ��� nonUnicode:
{
			 using namespace Runtime::InteropServices;
			 String^ g = Marshal::PtrToStringUni((IntPtr)str);
          return  g;
}
	
//�������� ����� hex � ������
//������ ���������� ��� ��������� ������ � HEX
System::String ^ PBYTEToStr(PBYTE x,const int cb)
{
   char *str= NULL;
   str = new char[2*cb+1]; // ������ ����� � ��� ���� �������? ���� ���� � HEX -��� FF, � ������� - ��� �������.
   str[cb-1] = '\0';
    int cntr = cb;
   for (int i=0; i<=cb-1; i++) {
	 cntr --;
      sprintf( str+i*2, "%02x", x[cntr]);
	  
   }
   return CharToString(str);
}