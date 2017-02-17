// Интерфейс класса словаря
//
// определяем только функцию добавления слова в словарь -- AddWord(const string &Str)

#pragma once

#include <string>
using std::wstring;

class IDictionary
{
public:
	virtual void AddWord(const wstring &Str)=0;
};
//-------------------------------------------------------------------
