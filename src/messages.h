// Файл с текстовыми сообщениями программы
// 

#pragma once

#include <exception>
#include <stdexcept>
using std::string;


// Приветственное сообщение
const string messAppName = 
			"fly_elephant - Как из мухи сделать слона\n";
// Описание применения приложения
const string messAppUsing = 
			"Использование:\n"
			"fly_elephant имя_файла1 имя_файла2\n"
			"имя_файла1 - путь к текстовому файлу, в котором указано начальное и конечное слова.\n"
			"имя_файла2 - путь к файлу, который содержит словарь.\n";

// Информационные сообщения программы
const string messBeginEndFileName = 
			"Файл начального и конечного слов:\n";
const string messDicFileName = 
			"Файл словаря:\n";

// Сообщение об отсутствии пути
const string messWordsPathNotExists = 
			"Для заданных начального и конечного слов пути не существует.";
//-------------------------------------------------------------------

// Исключение, генерируемое при ошибках ввода
class Exception : public std::runtime_error {
public:
	explicit Exception(const std::string& what_arg) : runtime_error(what_arg) {}
};
//-------------------------------------------------------------------

// Сообщения об ошибках
const string messError = "Ошибка: ";

const string messErrorReadBeginEndFile	= "ошибка чтения файла начального и конечного слов";
const string messErrorReadDicFile		= "ошибка чтения файла словаря";

const string messErrorBeginWordInvalid	= "начальное слово некорректно";
const string messErrorEndWordInvalid	= "конечное слова некорректно";
const string messErrorBeginEndWordLenInvalid = "начальное и конечное слова разной длины";
const char formatErrorDicWordInvalid[] 	= "строка словаря %ld - слово содержит запрещенный символ";

const string messErrorAddWordInvalid 	= "попытка добавить в словарь некорректное слово";

const string messErrorFindPathBeginWordMissing	= "начальное слово отсутствует в словаре";
const string messErrorFindPathEndWordMissing	= "конечное слово отсутствует в словаре";


// Размер буфера для функции генерации. Должен вмещать любую строку формата
const size_t MakeMessageBufferSize = 256;
// Функция генерации сообщения
const string MakeMessage(const char *format, ...);

