// Модуль работы с сообщениями об ошибках
// 


#include <string>
#include <cstdio>
#include <stdarg.h>
#include <assert.h>
#include "messages.h"
#include "utf8.h"


// Макрос для определения размера массива
#ifndef _countof
#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

using std::string;

// Функция генерации сообщения
const string MakeMessage(const char *format, ...)
{
	char buffer[MakeMessageBufferSize];
	va_list args;
	va_start(args, format);
	if(vsnprintf(buffer, MakeMessageBufferSize, format, args) < 0) {
		assert(MakeMessageBufferSize > 4);		// защита на случай изменения MakeMessageBufferSize (при отладке, например)

		char *p = &buffer[_countof(buffer)];	// ардес, расположенный сразу за последним символом (C++ такое допускает)
		*--p = 0;			// обрежем строку
		*--p = '.';			// поставим троеточие
		*--p = '.';
		*--p = '.';
		//assert(false); // ошибка -- строка не поместилась в буфер
	}
	va_end(args);

	return string(buffer);
}
//-------------------------------------------------------------------

