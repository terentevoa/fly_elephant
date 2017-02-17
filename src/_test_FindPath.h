// Модульное тестирование поиска маршрута
// 
// 
//

#pragma once

#include <assert.h>
#include <string>
#include <algorithm>
#include "FindPath.h"

/*  
namespace помогает обойтись меньшим количеством файлов тестирования
и легче их отключать.
*/
namespace testFindPath {

#include "_test_Types.h"
#include "messages.h"

using std::cerr;
using std::endl;
using std::ofstream;
using std::vector;
using std::string;
using std::wstring;


// Функция проверки добавления слова в словарь
void test_AddWord(const wstring &Str, const string &sError = "")
{
	cerr << "test_AddWord(" << Str << ", " << sError.c_str() << ") \n";

	try {
		// Формируем словарь
		CFindPath fp;
		fp.AddWord(Str);

		// Если проверяется ошибка (sError не пуста), до этого места не должны были дойти
		assert(sError.empty());
		if(sError.empty()) {
			// Ошибка не проверяется (sError пуста) -- проверим состояние словаря
			WordsDict dict = fp._get_dict(Str.length());
			assert(dict.size() == 1 && dict.find(Str) != dict.end());
		}
	}
	catch(Exception &e) {
		assert(sError == e.what());
	}

	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

// Функция проверки добавления слов в словарь
void test_AddWords(const StringArray &saContent, const StringArray &saVerify, 
				   const string &sError = "")
{
	// Технологический вывод, описывает текущий тест
	wstring sContent, sVerify;
	for(StringArray::const_iterator it = saContent.begin(); it != saContent.end(); ++it)
		sContent += *it + L" ";
	for(StringArray::const_iterator it = saVerify.begin(); it != saVerify.end(); ++it)
		sVerify += *it + L" ";
	cerr << "test_AddWords(" << sContent << ", " << sVerify << ", " << sError.c_str() << ") \n";

	size_t nMaxWordLen = 0;
	try {
		// Формируем словарь
		CFindPath fp;
		for(StringArray::const_iterator it = saContent.begin(); it != saContent.end(); ++it) {
			size_t nCurWordLen = (*it).length();
			if(nCurWordLen > nMaxWordLen) nMaxWordLen = nCurWordLen;
			fp.AddWord(*it);
		}

		// Если проверяется ошибка (sError не пуста), до этого места не должны были дойти
		assert(sError.empty());
		if(sError.empty()) {
			// Ошибка не проверяется (sError пуста) -- проверим состояние словаря
			StringArray::size_type nWordsCount = 0;
			for(size_t n = 1; n <= nMaxWordLen; ++n) {
				WordsDict dict = fp._get_dict(n);
				for(WordsDict::iterator it = dict.begin(); it != dict.end(); ++it) {
					++nWordsCount;
					assert(std::find(saVerify.begin(), saVerify.end(), (*it).first) != saVerify.end());
				}
			}
			assert(nWordsCount == saVerify.size());
		}
	}
	catch(Exception &e) {
		assert(sError == e.what());
	}

	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

//
void test_FindPath(const StringArray &saContent, 
				   const wstring &sBeginWord, const wstring &sEndWord, 
				   const StringArray &saVerify, 
				   const string &sError = "")
{
	// Технологический вывод, описывает текущий тест
	wstring sContent, sVerify;
	for(StringArray::const_iterator it = saContent.begin(); it != saContent.end(); ++it)
		sContent += *it + L" ";
	for(StringArray::const_iterator it = saVerify.begin(); it != saVerify.end(); ++it)
		sVerify += *it + L" ";
	cerr	<< "test_FindPath(" << sContent << ", " << sBeginWord << ", " 
			<< sEndWord << ", " << sVerify << ", " << sError.c_str() << ") \n";

	// Подготовка словаря
	CFindPath fp;
	for(StringArray::const_iterator it = saContent.begin(); it != saContent.end(); ++it)
		fp.AddWord(*it);

	try {
		WordsPath wp = fp.Find(sBeginWord, sEndWord);

		// Если проверяется ошибка (sError не пуста), до этого места не должны были дойти
		assert(sError.empty());

		if(sError.empty()) {
			// Ошибка не проверяется (sError пуста) -- проверим результат
			assert(wp.size() == saVerify.size());
			StringArray saResult;
			saResult.assign(wp.begin(), wp.end());
			assert(saResult == saVerify);
		}
	}
	catch(Exception &e) {
		assert(sError == e.what());
	}

	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

} // of namespace testFindPath {


// Тестирование поиска кратчайшего маршрута
void test_FindPath()
{
	using namespace testFindPath;

	// Формирование словаря
	test_AddWord(L"", messErrorAddWordInvalid);		// ошибка -- пустое слово
	test_AddWord(L"1111", messErrorAddWordInvalid);	// ошибка -- слово содержит запрещенные символы
	test_AddWord(L"КОТ");							// успешное добавление слова

	// Сортировка слов, удаление повторений
	test_AddWords(StringArray(), StringArray());	// успешно -- пустой словарь
	test_AddWords(									// удаление повторений
		StringArray() << L"КОТ" << L"КОТ" << L"НОТА" << L"КОТ",
		StringArray() << L"КОТ" << L"НОТА" ); 
	test_AddWords(									// Сортировка слов
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ",
		StringArray() << L"КОТ" << L"КОТЫ" << L"НОТА" << L"РОТ" << L"РОТА" << L"ТОН" << L"ТОТ" ); 

	// Проверка контроля входных параметров
	test_FindPath(									// Начальное слово отсутствует
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ",
		L"МОТ", L"ТОТ", StringArray(), messErrorFindPathBeginWordMissing);
	test_FindPath(									// Конечное слово отсутствует
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ",
		L"КОТ", L"ВОТ", StringArray(), messErrorFindPathEndWordMissing);
	test_FindPath(									// Начальное и конечное слово разной длины
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ",
		L"КОТЫ", L"ТОТ", StringArray(), messErrorBeginEndWordLenInvalid);

	// Проверка собственно работы алгоритма пути
	test_FindPath(									// Пути нет
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ" << L"МАК",
		L"КОТ", L"МАК", StringArray());
	test_FindPath(									// Путь найден
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ",
		L"КОТ", L"ТОН", StringArray() << L"КОТ" << L"ТОТ" << L"ТОН");
		
}
//-------------------------------------------------------------------
