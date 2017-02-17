// Модуль поиска кратчайшего маршрута от начального слова до конечного слова.
// 
//

#include <assert.h>
#include <stdlib.h>
#include <utility>
#include "FindPath.h"
#include "messages.h"
#include "Verify.h"
#include "utf8.h"


// Перегрузка оператора вывода wstring в поток string
string w2s(const std::wstring& wstr)
{  
	const size_t BufferSize = 256;
	char buffer[BufferSize];
	if(wcstombs(buffer, wstr.c_str(), BufferSize) < 0)
		assert(false);		// защита от ошибки
	return string(buffer);
}
//-------------------------------------------------------------------

// Перегрузка оператора вывода результата в поток
std::ostream& operator<<(std::ostream& os, const WordsPath& wp)  
{  
	for(WordsPath::const_iterator it = wp.begin(); it != wp.end(); ++it)
		//os << w2s(*it) << std::endl;	// испльзуется в Windows
		os << *it << std::endl;			// работает в Linux и без преобразования wstring -> string
	return os;  
}
//-------------------------------------------------------------------

// Конструктор
CFindPath::CFindPath(void)
{
}
//-------------------------------------------------------------------

// Деструктор
CFindPath::~CFindPath(void)
{
}
//-------------------------------------------------------------------

// Добавление слова в словарь
void CFindPath::AddWord(const wstring &addWord)
{
	// Проверим слово на корректность
	if(!VerifyWord(addWord))
		throw Exception(messErrorAddWordInvalid);
	
	// Проверка пройдена -- добавим слово в словарь	

	// Определим количество символов в слове -- от этого зависит выбор словаря
	const size_t WordLength = addWord.length();
	// Выберем словарь по размеру слова
	WordsDict &wordsDict = m_WordsDictArray[WordLength];
	// Добавим новое слово в словарь
	std::pair<WordsDict::iterator, bool> pr = wordsDict.insert(WordsDict::value_type(addWord, WordLinks()));
	// Только если слово действительно добавлено
	if(pr.second) {
		WordLinks &addLinks = (*pr.first).second;	// Таблица связей добавленного слова
		// Проверим, что таблица связей пуста
		assert(addLinks.size() == 0);

		// Просканируем словарь в поисках слов, отличающихся на один символ и обновим таблицы связей
		for(WordsDict::iterator word_it = wordsDict.begin(); word_it != wordsDict.end(); ++word_it) {
			// Для всех, кроме только что добавленного
			if(pr.first == word_it) continue;

			// Проверим наличие связи данного узла с уже имеющимися
			const wstring &iterWord = (*word_it).first;	// слово
			WordLinks &iterLinks = (*word_it).second;	// таблица связей

			// Поиск различий
			int differences = 0;	// счетчик различий
			for(size_t position = 0; differences < 2 && position < WordLength; ++position) {
				if(addWord[position] != iterWord[position]) 
					++differences;
			}
			assert(differences > 0);	// хотя бы одно различие быть должно. Иначе -- ошибка в программе

			// Если различие только в одном символе -- добавим связи
			if(differences == 1) {
				// запись "&*obj" означает "адрес объекта, на который указывает итератор"
				iterLinks.push_back(&*pr.first);
				addLinks.push_back(&*word_it);
			}
		}
	}
}
//-------------------------------------------------------------------

// Поиск пути
WordsPath CFindPath::Find(const wstring &sBeginWord, const wstring &sEndWord) const
{
	// Проверка длин слов
	if(sBeginWord.length() != sEndWord.length())
		throw Exception(messErrorBeginEndWordLenInvalid);

	// количество букв в словах
	const size_t WordLength = sBeginWord.length();		
	// проверим наличие словаря
	WordsDictionaryArray::const_iterator itDict = m_WordsDictArray.find(WordLength);

	// Если соответствующего словаря нет -- значит слова такой длины добавлено не было.
	// Считаем, что ошибка отсутствия начального слова
	if(itDict == m_WordsDictArray.end())
		throw Exception(messErrorFindPathBeginWordMissing);

	// Ссылка на используемый словарь
	const WordsDict &wordsDict = (*itDict).second;

	// поиск начального слова в словаре
	if(wordsDict.find(sBeginWord) == wordsDict.end())
		throw Exception(messErrorFindPathBeginWordMissing);
	// поиск конечного слова в словаре
	if(wordsDict.find(sEndWord) == wordsDict.end())
		throw Exception(messErrorFindPathEndWordMissing);

	
	// Ищем кратчайший путь по алгоритму Дейкстры
	//map<wstring, WordData> words;	// ассоциативный массив данных узлов
	/*for(NodeList::iterator nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it) {
		wstring word = (*nodes_it).first;
		words.insert(map<wstring, WordData>::value_type(word, WordData(word)));
	}*/
	// Очередь рассматриваемых слов
	//priority_queue<WordData*, vector<WordData*>, CompareWordData> queWords;
	
	MapFindData mapFindData;
	priority_queue<FindData*, vector<FindData*>, CompareWordData> queWords;
	
	WordsDict::const_iterator it = wordsDict.find(sBeginWord);
	assert(it != wordsDict.end());

	// ссылка на элемент словаря с начальным словом
	WordsDict::const_reference refWordDictItem = *it;

	// Добавим начальное слово в список вспомогательных объектов
	//FindData &fdBeginWord = mapFindData[&refWordDictItem];
	std::pair<MapFindData::iterator, bool> pr = mapFindData.insert(std::make_pair(&refWordDictItem, FindData(&refWordDictItem, 0)));
	assert(pr.second);
	// ссылка на вспомогательную информацию поиска начального слова
	FindData &fdBeginWord = (*pr.first).second;
	
	// Добавим ссылку на вспомогательный объект поиска в очередь
	queWords.push(&fdBeginWord);

	while(!queWords.empty()) {
		// Извлечем из очереди элемент
		FindData &fdCurWord = *(queWords.top());
		wstring curr_word = fdCurWord.pWordInfo->first;
//		fdCurWord.visited = true;
		queWords.pop();

		// Текущая дистанция
		size_t &curr_dest = fdCurWord.destination;

		// Дистанция далее относительно выбранного слова
		size_t next_destination = fdCurWord.destination + 1;

		// Таблица связей выбранного слова
		const WordLinks &linksCurWord = fdCurWord.pWordInfo->second;

		// Просканируем слова
		for(WordLinks::const_iterator it = linksCurWord.begin(); it != linksCurWord.end(); ++it) {
			wstring wordNext = (*it)->first;	// связанное слово

			WordsDict::const_iterator itNext = wordsDict.find(wordNext);
			assert(itNext != wordsDict.end());

			// ссылка на элемент словаря с начальным словом
			WordsDict::const_reference refWordDictItemNext = *itNext;

			// Добавим слово в список вспомогательных объектов
			FindData &fdNextWord = mapFindData[&refWordDictItemNext];
			if(fdNextWord.pWordInfo == NULL)
				fdNextWord.pWordInfo = &refWordDictItemNext;
			else
				// Узел графа уже обработан -- можно переходить к следующему.
				// А так как у ребер вес одинаков, дистанцию можно не проверять,
				// она сразу выставляется минимальной
				continue;

//			if(fdNextWord.visited) continue;
			
			// Проверим и скорректируем дистанцию и направление
			size_t &temp_dest = fdNextWord.destination;
			if(fdNextWord.destination > next_destination) {
				fdNextWord.destination = next_destination;
				fdNextWord.from = &fdCurWord;
			}

			// Так как у ребер вес одинаков, можно сразу исключить узел из рассмотрения
//			fdNextWord.visited = true;

			// Так как у ребер вес одинаков и очередной узел всегда выбирается 
			// с наименьшим путем, то можно завершить поиск, 
			// если достигнуто конечное слово
			if(fdNextWord.pWordInfo->first == sEndWord) break;

			// Добавим ссылку на вспомогательный объект поиска в очередь
			queWords.push(&fdNextWord);
		}
	}

	// Формируем результат
	//WordData &wdEnd = words[sEndWord];
	WordsDict::const_iterator itEnd = wordsDict.find(sEndWord);
	assert(itEnd != wordsDict.end());

	// ссылка на элемент словаря с конечным словом
	WordsDict::const_reference refWordDictItemEnd = *itEnd;

	FindData &fdEndWord = mapFindData[&refWordDictItemEnd];

	// Если путь не найден, вернем пустой объект
	if(fdEndWord.destination == DistanceInfinite)
		return WordsPath(0);
	
	// Заполним последовательность
	WordsPath wp(fdEndWord.destination + 1);
	FindData *pfd = &fdEndWord;
	for(size_t n = fdEndWord.destination; n+1 > 0; --n) {
		assert(pfd);
		assert(pfd->pWordInfo);
		assert(fdEndWord.destination != DistanceInfinite);

		// запишем слово
		wp[n] = pfd->pWordInfo->first;
		// перейдем к следующему
		pfd = pfd->from;
	}
	assert(pfd == NULL);

	// Возвращаем результат
	return wp;
}
//-------------------------------------------------------------------
