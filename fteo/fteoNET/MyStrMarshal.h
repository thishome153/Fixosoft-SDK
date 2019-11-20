//2014 Fixosoft String routines
// ����������� �����

#include <windows.h>  //���� ��������
#include <string>
using namespace System; // ���� .NET
	

#ifndef MarshUtils_h_INCLUDED // ���� ������ �� �������������� ���������
#define MarshUtils_h_INCLUDED




// �������������� ������ .NET � ��������� �� ������ WINAPI/
 char* StringtoChar(System::String ^ s); 		 // ������� � ����� �����....
 System::String ^ LPTSTRToString(LPTSTR str); 		 // ��� Unicode:
 System::String ^ LPSTRToString(LPSTR  str); 		 // ��� Unicode:
 bool LPWSTRTostd_string(std::string& s, const LPWSTR pw, UINT codepage = CP_ACP);
 System::String^ LPWSTRToString(const LPWSTR pw, UINT codepage = CP_ACP);

 System::String ^ CharToString(char *str); //��� ANSI
 System::String ^ PBYTEToStr(PBYTE x, const int cb);
#endif /* _SignerUtils_h_INCLUDED */