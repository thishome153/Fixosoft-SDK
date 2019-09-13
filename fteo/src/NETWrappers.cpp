#include <stdafx.h>  // эти pch мен€ добивают
#include "NETWrappers.h"
#include <string.h>
#include <stdlib.h>  // for: mallloc 
namespace fteo

{
	namespace NETWrappers
	{
		// ANSI char в управл€емый string:
		System::String^ CharToString(char* str)
		{
			if (!str) return L"";
			using namespace System::Runtime::InteropServices;
			System::String^ g = Marshal::PtrToStringAnsi((System::IntPtr)str);

			return  g;
		}

		// ANSI char в управл€емый string:	
		//Ёта намного проще, без маршаллов:
		System::String^ CharToString2(char* str)
		{
			if (!str) return L"";
			System::String^ g = gcnew System::String(str);
			return  g;

		}

		char* StringtoChar(System::String^ s) 		 // ќхуеешь с ваших строк....
		{
			using namespace System::Runtime::InteropServices;

			return (char*)Marshal::StringToHGlobalAnsi(s).ToPointer();

		}





	} // namespace 
}