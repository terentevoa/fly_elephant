// Модуль служебных функций и типов для тестирования
// 



#include <assert.h>
#include <string>
#include <vector>
using std::wstring;

// размер буфера для преобразования строк
const size_t BufferSize = 256;

// Перегрузка оператора вывода wstring в поток string
std::ostream& operator<<(std::ostream& os, const std::wstring& wstr)
{  
	char buffer[BufferSize];
	if(wcstombs(buffer, wstr.c_str(), BufferSize) < 0)
		assert(false);		// защита от ошибки
	os << buffer;
    return os;  
}
//-------------------------------------------------------------------

// Массив хранения строк словаря
typedef std::vector<wstring> StringArray;
/* Определим способ добавления слов в массив.
   При выполнении последовательности операторов << будет создано много
   временных переменных типа StringArray. Данное поведение очень не эффективно,
   но для целей тестирования применимо.
   Если использовать вызов
		StringArray& operator<<(StringArray& v, const wstring &s);
   не будет компилироваться конструкция 
		StringArray() << L"КОТ" << L"ТОН"
   в вызове функций.
   Используется только при тестировании.
*/
StringArray operator<<(StringArray v, const wstring &s) { v.push_back(s); return v; }

//-------------------------------------------------------------------