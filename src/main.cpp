// Как из мухи сделать слона
// 

// _UTEST -- макрос включающий модульное тестирование
// для компиляции боевого кода его необходимо закомментировать

//#define _UTEST


#include <iostream>
#include <string>
#include <assert.h>
#include "messages.h"
#include "LoadFiles.h"
#include "FindPath.h"

#ifdef _UTEST

// Модульное тестирование
#include "_test_LoadFile.h"
#include "_test_FindPath.h"

int main(int argc, char* argv[])
{
	// Установим кодовую страницу, заданную по умолчанию для пользователя 
	setlocale(LC_ALL, "");

	// Тестирование загрузки исходных данных
	test_LoadFiles();

	// Тестирование поиска кратчайшего маршрута
	test_FindPath();

	std::cout << "Тесты пройдены.\n===============\n";
}
//-------------------------------------------------------------------


#else
// Боевая программа

int main(int argc, char* argv[])
{
	// Установим кодовую страницу, заданную по умолчанию для пользователя 
	setlocale(LC_ALL, "");

	if(argc != 3) {
		// Параметров не хватает -- выведем информацию об использовании
		std::cout << messAppName << std::endl;
		std::cout << messAppUsing << std::endl;
	}
	else try {
		/*
		// Вывод краткого описания программы и параметров
		// (закомментировано -- по ТЗ требуется только вывод результата )
		std::cout << messAppName << std::endl;
		std::cout << messBeginEndFileName << argv[1] << std::endl << std::endl;
		std::cout << messDicFileName << argv[2] << std::endl << std::endl;
		*/
		// Загрузка начального и конечного слов
		std::wstring sBeginWord, sEndWord;
		LoadBeginEndWords(argv[1], &sBeginWord, &sEndWord);

		// Функция загрузки словаря
		CFindPath wordFindPath;
		LoadDicWords(argv[2], &wordFindPath);

		// Поиск кратчайшего пути
		WordsPath Result = wordFindPath.Find(sBeginWord, sEndWord);

		// Вывод результата
		if(Result.empty())
			std::cout << messWordsPathNotExists << std::endl;
		else
			std::cout << Result;
	}
	catch(Exception &e) {
		// Ошибка во время выполнения -- выведем сообщение
		std::cerr << messError << e.what() << "." << std::endl;
	}
	
	return 0;
}
//-------------------------------------------------------------------

#endif
