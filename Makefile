all: main.cpp Table.cpp Database.cpp  whereclause.cpp update.cpp delete.cpp load.cpp Quary.cpp saving.cpp server.cpp calculate.cpp
	g++ -std=c++11 -o main main.cpp Table.cpp Database.cpp  whereclause.cpp  update.cpp delete.cpp load.cpp Quary.cpp saving.cpp server.cpp calculate.cpp

Table.o: Table.cpp
	g++ -std=c++11 -o Table.o -c Table.cpp

Database.o: Database.cpp
	g++ -std=c++11 -o Database.o -c Database.cpp

whereclause.o: whereclause.cpp
	g++ -std=c++11 -o whereclause.o -c whereclause.cpp

update.o: update.cpp
	g++ -std=c++11 -o update.o -c update.cpp 

delete.o: delete.cpp
	g++ -std=c++11 -o delete.o -c delete.cpp
load.o: load.cpp
	g++ -std=c++11 -o load.o -c load.cpp