fly_elephant: main.o Verify.o LoadFiles.o FindPath.o messages.o utf8.o
	gcc main.o Verify.o LoadFiles.o FindPath.o messages.o utf8.o -o fly_elephant -lstdc++
main.o: src/main.cpp
	gcc -c src/main.cpp -std=c++11
Verify.o: src/Verify.cpp
	gcc -c src/Verify.cpp -std=c++11
LoadFiles.o: src/LoadFiles.cpp
	gcc -c src/LoadFiles.cpp -std=c++11
FindPath.o: src/FindPath.cpp
	gcc -c src/FindPath.cpp -std=c++11
messages.o: src/messages.cpp
	gcc -c src/messages.cpp -std=c++11
utf8.o: src/utf8.cpp
	gcc -c src/utf8.cpp -std=c++11
clean:
	rm -f main.o Verify.o LoadFiles.o FindPath.o messages.o utf8.o
