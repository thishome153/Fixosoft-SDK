/***********************************************************************************
* Fixosoft 2014
/***********************************************************************
*-----------������� ��������� �����------------------------
*   �//����� � ����� �����....
*
***********************************************************************/


#include "stdafx.h"
#include "MyStrMarshal.h"
#include "string"

//using System;



// �������������� ������ .NET � ��������� �� ������ WINAPI/
// �����������, ������ ��� ������ ������� ������� � 
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
System::String^ LPSTRToString(LPSTR  str) 		 // ��� nonUnicode:
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


//�������� ����� hex � ������
//������ ���������� ��� ��������� ������ � HEX
System::String^ PBYTEToStr(PBYTE x, const int cb)
{
	char* str = NULL;
	str = new char[2 * cb + 1]; // ������ ����� � ��� ���� �������? ���� ���� � HEX -��� FF, � ������� - ��� �������.
	str[cb - 1] = '\0';
	int cntr = cb;
	for (int i = 0; i <= cb - 1; i++) {
		cntr--;
		sprintf(str + i * 2, "%02x", x[cntr]);

	}
	return CharToString(str);
}