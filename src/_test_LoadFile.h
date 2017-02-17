// Модульное тестирование чтения из файлов
// 

#pragma once

#include <iostream>
#include <fstream>
#include <assert.h>
#include <locale>
#include <codecvt>
#include <vector>
#include <string>

#include "messages.h"
#include "LoadFiles.h"
#include "Verify.h"
#include "utf8.h"

/*  
namespace помогает обойтись меньшим количеством файлов тестирования
и легче их отключать.
*/
namespace testLoadFile {

#include "_test_Types.h"
#include <stdarg.h>

using std::cerr;
using std::endl;
using std::ofstream;
using std::vector;
using std::string;
using std::wstring;


// Тестовый класс для хранения слов словаря
class CTestDictionary : public IDictionary
{
public:
	CTestDictionary() {}
	virtual ~CTestDictionary() {}
	void AddWord(const wstring &Str) { sa.push_back(Str); }

	// данные класса объявлены как public, 
	// чтобы иметь возможность проверить их корректность
	// (только для тестирования)
	StringArray sa;
};
//-------------------------------------------------------------------

// Проверка реакции на отстутствие файла начального и конечного слов
void test_LoadMissingBeginEndFile(const string &sError = "")
{
	// Технологический вывод, описывает текущий тест
	wstring ws(sError.begin(), sError.end());
	std::cerr << "test_LoadMissingBeginEndFile(" << sError << ") \n";

	// Переменные под считанные слова
	wstring sReadBeginWord, sReadEndWord;

	// Чтение файла
	try {
		LoadBeginEndWords("~~~", &sReadBeginWord, &sReadEndWord);

		// до этого места дойти не должны
		assert(false);
	}
	catch(Exception &e) {
		// Проверка соответствия сообщения об ошибке
		assert(sError == e.what());
	}
	
	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

// Проверка реакции на отстутствие файла словаря
void test_LoadMissingDicFile(const string &sError = "")
{
	// Технологический вывод, описывает текущий тест
	cerr << "test_LoadMissingDicFile(" << sError << ") \n";

	// Переменные под считанные слова
	wstring sReadBeginWord, sReadEndWord;

	// Чтение файла
	try {
		CTestDictionary testDict;
		LoadDicWords("~~~", &testDict);

		// до этого места дойти не должны
		assert(false);
	}
	catch(Exception &e) {
		// Проверка соответствия сообщения об ошибке
		assert(sError == e.what());
	}
	
	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

// Тестирование подпрограммы загрузки файлов
void test_LoadBeginEndWordFile(
	const wstring &sBeginWord, const wstring &sEndWord, 
	const wstring &sFileContent, const string &sError = "")
{
	// Технологический вывод, описывает текущий тест
	wstring str = sFileContent;
	std::size_t found = str.find(L"\n");	// Замена символа конца строки '\n' на строку "\\n"
	while(found != std::string::npos) {
		str.replace(found, 1, L"\\n");
		found = str.find(L"\n", found + 1);
	}
	cerr << "test_LoadBeginEndWordFile(" << sBeginWord << ", " << sEndWord 
		<< ", " << str << ", " << sError << ") \n";
		
	// Создадим временный файл с заданным текстом
	const string sFileName = "~1.txt";
	ofstream ofs(sFileName.c_str());

	// Сгенерируем сигнатуру UTF-8
	//ofs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t,0x10ffff,std::generate_header>));
	
	// Сохраним данные в файл
	ofs << WtoUTF8(sFileContent);
	ofs.close();
	
	// Переменные под считанные слова
	wstring sReadBeginWord, sReadEndWord;

	// Чтение файла
	try {
		LoadBeginEndWords(sFileName, &sReadBeginWord, &sReadEndWord);

		// если функция должна реагировать на ошибку, то до этого 
		// места дойти не должны
		assert(sError.empty());

		// Если ошибка не задана, сравним заданные слова со считанными
		if(sError.empty()) 
			assert(sReadBeginWord == sBeginWord && sReadEndWord == sEndWord);
	}
	catch(Exception &e) {
		// Проверка соответствия сообщения об ошибке
		assert(sError == e.what());
	}
	
	// Удаление временного файла
	remove(sFileName.c_str());

	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

// Тестирование подпрограммы загрузки словаря
void test_LoadDicFile(const StringArray &saContent, const StringArray &saVerify, 
					  const string &sError = "")
{
	// Технологический вывод, описывает текущий тест
	wstring sContent, sVerify;
	for(StringArray::const_iterator it = saContent.begin(); it != saContent.end(); ++it)
		sContent += *it + L" ";
	for(StringArray::const_iterator it = saVerify.begin(); it != saVerify.end(); ++it)
		sVerify += *it + L" ";
	cerr << "test_LoadDicFile(" << sContent << ", " << sVerify << ", " << sError.c_str() << ") \n";
	
	// Создадим временный файл с заданным текстом
	const string sFileName = "~1.txt";
	ofstream ofs(sFileName.c_str());
	
	// Сгенерируем сигнатуру UTF-8
	//ofs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t,0x10ffff,std::generate_header>));
	
	// Сохраним данные в файл
	for(StringArray::const_iterator it = saContent.begin(); it != saContent.end(); ++it)
		ofs << WtoUTF8(*it) + "\n";
	ofs.close();
	
	// Чтение файла
	try {
		CTestDictionary testDict;
		LoadDicWords(sFileName, &testDict);

		// если функция должна реагировать на ошибку, то до этого 
		// места дойти не должны
		assert(sError.empty());

		// Если ошибка не задана, проверим результат
		if(sError.empty()) 
			assert(testDict.sa == saVerify);
	}
	catch(Exception &e) {
		// проверка соответствия сообщения об ошибке
		assert(sError == e.what());
	}

	// Удаление временного файла
	remove(sFileName.c_str());

	cerr << "PASS\n\n";
}
//-------------------------------------------------------------------

} // of namespace testLoadFile {


// Тестирование загрузки входных файлов
void test_LoadFiles()
{
	using namespace testLoadFile;

	// Исходный файл отстутствует
	test_LoadMissingBeginEndFile(messErrorReadBeginEndFile);
	test_LoadMissingDicFile(messErrorReadDicFile);
	
	// Тест загрузки файла начального и конечного слов
	test_LoadBeginEndWordFile(L"КОТ", L"ТОН", L"КОТ\nТОН\n");
	test_LoadBeginEndWordFile(L"КОТ", L"ТОН", L"КОТ\nТОН");	// нет второго перевода строки

	// Тесты контроля ошибок в файле начального и конечного слов
	test_LoadBeginEndWordFile(L"", L"ТОН", L"\nТОН\n", messErrorBeginWordInvalid);		// пустое начальное слово
	test_LoadBeginEndWordFile(L"КОТ", L"", L"КОТ\n\n", messErrorEndWordInvalid);		// пустое конечное слово
	test_LoadBeginEndWordFile(L"КОТ", L"", L"КОТ\n", messErrorEndWordInvalid);			// пустое конечное слово (в файле только одна строка)
	test_LoadBeginEndWordFile(L"", L"", L"", messErrorBeginWordInvalid);					// пустой файл (программа прерывается при обнаружении пустого начального слова)
	test_LoadBeginEndWordFile(L"К0Т", L"ТОН", L"К0Т\nТОН", messErrorBeginWordInvalid);	// начальное слово содержит запрещенный символ 
	test_LoadBeginEndWordFile(L"К Т", L"ТОН", L"К Т\nТОН", messErrorBeginWordInvalid);	// начальное слово содержит пробел 
	test_LoadBeginEndWordFile(L"КОТ", L"Т0Н", L"КОТ\nТ0Н", messErrorEndWordInvalid);		// конечное слово содержит запрещенный символ (0 - ноль)
	test_LoadBeginEndWordFile(L"КОТ", L"Т Н", L"КОТ\nТ Н", messErrorEndWordInvalid);		// конечное слово содержит пробел 
	test_LoadBeginEndWordFile(L"КОТТ", L"ТОН", L"КОТТ\nТОН", messErrorBeginEndWordLenInvalid);	// начальное и конечное слова разной длины

	// Тест загрузки файла словаря
	test_LoadDicFile(
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ",
		StringArray() << L"КОТ" << L"ТОН" << L"НОТА" << L"КОТЫ" << L"РОТ" << L"РОТА" << L"ТОТ" );
	test_LoadDicFile(	// пустой словарь
		StringArray(), StringArray() );
	test_LoadDicFile(	// пустой словарь с переводами строк
		StringArray() << L"" << L"", StringArray() );
	test_LoadDicFile(	// ошибка в слове -- содержит запрещенный символ (0 - ноль)
		StringArray() << L"КОТ" << L"Т0Н" << L"НОТА", StringArray(), MakeMessage(formatErrorDicWordInvalid, 2));
	test_LoadDicFile(	// ошибка в слове -- содержит пробел
		StringArray() << L"КОТ" << L"КОТЫ" << L"Т Н" << L"НОТА", StringArray(), MakeMessage(formatErrorDicWordInvalid, 3));
/**/
}
//-------------------------------------------------------------------
