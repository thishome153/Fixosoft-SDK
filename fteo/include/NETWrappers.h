


#ifndef NETWRAPPERS_H
#define NETWRAPPERS_H



namespace fteo
{
	namespace NETWrappers
	{
    // ANSI char в управляемый string:
   System::String ^  CharToString(char *str);
   System::String ^  CharToString2(char *str);
             char*   StringtoChar(System::String ^ s); 		 // Охуеешь с ваших строк....
	}
}





#endif //NETWRAPPERS_H