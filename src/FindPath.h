// Модуль поиска кратчайшего маршрута от начального слова до конечного слова.
// Используется алгоритм Дейкстры.
//

#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <set>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include "Dictionary.h"

using std::wstring;
using std::list;
using std::vector;
using std::map;
using std::priority_queue;
//-------------------------------------------------------------------


// Типы и классы хранения служебных данных словаря


// Результат представим в виде массива слов
class WordsPath : public vector<wstring>
{
public:
	WordsPath(size_t size) : vector<wstring>(size) {}
	virtual ~WordsPath() {}

	// Определим операцию вывода в поток
	friend std::ostream& operator<<(std::ostream& os, const WordsPath& wp);
};


// Обозначение максимума расстояния до узла. Считаем это значение как бесконечное расстояние.
const size_t DistanceInfinite = std::numeric_limits<size_t>::max();


/*
	Для хранения связей и оперирования словарем используем адреса объектов, 
	расположенных в контейнерах map и list.
	Это допустимо, так как операции добавления объектов в эти конейнеры не
	разрушают итераторы и ссылки на уже имеющиеся в контейнере объекты.
	Т.е., подразумеваем, что адреса в памяти объектов контейнера не 
	меняются при добавлении новых объектов.
*/

// Таблица связей с другими словами
class WordLinks : public list< std::pair<const wstring, WordLinks> * >
{
public:
	WordLinks() {}
	virtual ~WordLinks() {}
};

// Собственно класс словаря
typedef map<wstring, WordLinks> WordsDict;

// Служебные данные алгоритма Дейкстры
class FindData {
public:
	WordsDict::const_pointer pWordInfo;	// указатель на элемент словаря
	size_t destination;					// дистанция
	FindData * from;					// направление
	//bool visited;						// признак посещения узла - не требуется, в качестве флага используется pWordInfo
										// также можно использовать само наличие узла графа в массиве MapFindData
	

	FindData() : pWordInfo(), destination(DistanceInfinite), /*visited(false), */from(NULL) {}
	//FindData(std::pair<const wstring, WordLinks> *p) : pWordInfo(p), destination(DistanceInfinite), visited(false), from(NULL) {}
	//FindData(std::pair<const wstring, WordLinks> *p, size_t dest) : pWordInfo(p), destination(dest), visited(false), from(NULL) {}
	FindData(WordsDict::const_pointer p, size_t dest) : pWordInfo(p), destination(dest), /*visited(false), */from(NULL) {}
};

// Ассоциативный массив для хранения узлов графа
typedef map<WordsDict::const_pointer, FindData> MapFindData;

// Класс сравнения экземпляров WordData
class CompareWordData {
public:
	bool operator()(const FindData *_Left, const FindData *_Right) const { return _Left->destination > _Right->destination; }
};
//-------------------------------------------------------------------



// Определение типа ассоциативного массивв словарей -- соответствие словаря длине слова
typedef map<size_t, WordsDict> WordsDictionaryArray;


// Словарь
class CFindPath : public IDictionary
{
public:
	CFindPath();
	virtual ~CFindPath();

	// Добавить слово
	virtual void AddWord(const wstring &Str);

	// Очистка
	void Clear() { /*m_WordsDict.clear();*/ m_WordsDictArray.clear(); }

	// Функция поиска кратчайшего маршрута
	WordsPath Find(const wstring &sBeginWord, const wstring &sEndWord) const;


#ifdef _UTEST	// следующие методы используются только для тестирования
	WordsDict _get_dict(size_t nDict) { return m_WordsDictArray[nDict]; }
	WordsDict _all_words_count(size_t nDict) { return m_WordsDictArray[nDict]; }
#endif

private:
	// внутреннее представление данных
	WordsDictionaryArray m_WordsDictArray;	// массив словарей (по словарю на каждый размер слова)

private:
	// Запрещаем конструктор копирования и оператор присваивания, чтобы
	// избежать неявного создания компилятором копий объекта (для объектов
	// с большим словарем это можеть сильно сказаться на производительности).
	// Если когда-нибудь действительно появится необходимость создания копии 
	// объекта, напишем соответствующую функцию копирования.
	CFindPath(const CFindPath &);
	CFindPath &operator=(const CFindPath &);

};
//-------------------------------------------------------------------
