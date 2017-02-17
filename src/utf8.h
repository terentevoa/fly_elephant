// Модуль поддержки UTF-8
// 

#pragma once

#include <string>
//-------------------------------------------------------------------


// Функция преобразования UTF-8 string в wstring
std::wstring UTF8toW(const std::string &utf8);
// Функция преобразования wstring в UTF-8 string
std::string WtoUTF8(const std::wstring &wstr);

// Перегрузка оператора вывода wstring в поток string
std::ostream& operator<<(std::ostream& os, const std::wstring& wstr);
