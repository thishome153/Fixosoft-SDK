//2014 Fixosoft String routines
// ����������� �����

#include <windows.h>  //���� ��������

using namespace System; // ���� .NET
	

#ifndef MarshUtils_h_INCLUDED // ���� ������ �� �������������� ���������
#define MarshUtils_h_INCLUDED


//  ���������� ���������� ������� - ������������ �����
 char* StringtoChar(System::String ^ s); 		 // ������� � ����� �����....
 System::String ^ LPTSTRToString(LPTSTR str); 		 // ��� Unicode:
 System::String ^ LPSTRToString(LPSTR  str); 		 // ��� Unicode:
 System::String ^ CharToString(char *str); //��� ANSI
 System::String ^ PBYTEToStr(PBYTE x, const int cb);
#endif /* _SignerUtils_h_INCLUDED */