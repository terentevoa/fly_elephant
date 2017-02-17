// Модуль поддержки UTF-8
// 

#include <iostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "utf8.h"


// Функция преобразования UTF-8 string в wstring
std::wstring UTF8toW(const std::string &utf8)
{
	// the UTF-8 / wstring standard conversion facet
	std::wstring wStr;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
	wStr = utf8conv.from_bytes(utf8);
	return wStr;
}
//-------------------------------------------------------------------

// Функция преобразования wstring в UTF-8 string
std::string WtoUTF8(const std::wstring &wstr)
{
	// the UTF-8 / UCS2 standard conversion facet
	std::string Str;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
	Str = utf8conv.to_bytes(wstr);
	return Str;
}
//-------------------------------------------------------------------

// Перегрузка оператора вывода wstring в поток string
std::ostream& operator<<(std::ostream& os, const std::wstring& wstr)
{  
	os << WtoUTF8(wstr);
    return os;  
}
//-------------------------------------------------------------------
