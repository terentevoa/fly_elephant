// Модуль проверки корректности слов
// 
/*
	Используется wstring из-за кодировки UTF-8 в Linux и проверки слов с помощью iswalpha.
	Функция isalpha для string проверяет посимвольно, включая дополнительные и для
	символов, отличных от английских выдает ложь.
*/


 #include <wctype.h>
#include "Verify.h"


// Функция проверки корректности слова
bool VerifyWord(const wstring &Str)
{
	// Слово не должно быть нулевой длины
	if(Str.length() == 0) 
		return false;

	// Проверка на наличие запрещенных символов
	for(wstring::const_iterator it = Str.begin(); it != Str.end(); ++it) {
		if(!iswalpha(*it))
			return false;
	}
	
	// Проверки пройдены успешно
	return true;
}
//-------------------------------------------------------------------
