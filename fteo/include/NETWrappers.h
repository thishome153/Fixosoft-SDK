


#ifndef NETWRAPPERS_H
#define NETWRAPPERS_H



namespace fteo
{
	namespace NETWrappers
	{
    // ANSI char � ����������� string:
   System::String ^  CharToString(char *str);
   System::String ^  CharToString2(char *str);
             char*   StringtoChar(System::String ^ s); 		 // ������� � ����� �����....
	}
}





#endif //NETWRAPPERS_H