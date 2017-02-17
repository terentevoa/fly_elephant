// Модуль загрузки слов из файлов
// 


#include <fstream>
//#include <limits>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "messages.h"
#include "LoadFiles.h"
#include "Verify.h"
#include "utf8.h"

// Корректировака (удаление если присутствует) символа CR
void CorrectCR(std::string *ps)
{
	assert(ps);
	if(ps->empty()) return;
	std::string::iterator it = ps->end()-1;	// смотрим только последний символ
	if(*it == '\r') ps->erase(it);			// если == CR, удалим
}

// Функция загрузки начального и конечного слова
void LoadBeginEndWords(const string &sFileName, wstring *psBeginWord, wstring *psEndWord)
{
	assert(psBeginWord && psEndWord);

	std::ifstream ifs(sFileName.c_str());
	if(ifs.fail())
		throw Exception(messErrorReadBeginEndFile);
	
	// Обработаем сигнатуру UTF-8
	//ifs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t,0x10ffff, std::consume_header>));
	
	// Считываем строки.
	// Используем функцию getline вместо конструкции cin >> *psBeginWord из-за того, что
	// cin >> ... пропустит все пустые строки вначале файла, а нужно прочитать содержимое
	// именно первой и второй строк.
	string Str;
	getline(ifs, Str);			// 1-ая строка -- начальное слово
	CorrectCR(&Str);			// корректировка символа CR
	*psBeginWord = UTF8toW(Str);// преобразуем в wstring
	getline(ifs, Str);			// 2-ая строка -- конечное слово
	CorrectCR(&Str);			// корректировка символа CR
	*psEndWord = UTF8toW(Str);	// преобразуем в wstring
	ifs.close();
	
	// Проверка корректности загруженных слов
	if(!VerifyWord(*psBeginWord))
		throw Exception(messErrorBeginWordInvalid);
	if(!VerifyWord(*psEndWord))
		throw Exception(messErrorEndWordInvalid);

	// Проверка длины загруженных слов
	if(psBeginWord->length() != psEndWord->length()) 
		throw Exception(messErrorBeginEndWordLenInvalid);
}
//-------------------------------------------------------------------

// Функция загрузки словаря
void LoadDicWords(const string &sFileName, IDictionary *pDict)
{
	assert(pDict);

	std::ifstream ifs(sFileName.c_str());
	if(ifs.fail())
		throw Exception(messErrorReadDicFile);
	
	// Обработаем сигнатуру UTF-8
	//ifs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t,0x10ffff, std::consume_header>));
	
	// Считываем строки.
	size_t StringNum = 0;		// номер текущей строки
	while(ifs) {
		// Используем функцию getline вместо конструкции cin >> *psBeginWord из-за того, что
		// cin >> ... пропустит все пустые строки вначале файла, а нужно прочитать содержимое
		// именно первой и второй строк.
		string s;
		getline(ifs, s);
		CorrectCR(&s);				// корректировка символа CR
		wstring Str = UTF8toW(s);	// преобразуем в wstring
		++StringNum;

		// пустые строки отбрасываем
		if(Str.empty()) continue;	

		// Проверка на наличие запрещенных символов
		if(!VerifyWord(Str)) {
			/*const size_t BufferSize = 
					sizeof(formatErrorDicWordInvalid) / sizeof(formatErrorDicWordInvalid[0])
					+ std::numeric_limits<size_t>::digits10;
			char buffer[BufferSize];
			snprintf(buffer, BufferSize, formatErrorDicWordInvalid, StringNum);
			throw Exception(buffer);*/
			throw Exception(MakeMessage(formatErrorDicWordInvalid, StringNum));
		}
		
		// Добавим считанное слово в словарь
		pDict->AddWord(Str);
	}
	
	ifs.close();
}
//-------------------------------------------------------------------
