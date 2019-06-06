all: main.cpp Table.cpp Database.cpp  whereclause.cpp
	g++ -std=c++11 -o main main.cpp Table.cpp Database.cpp  whereclause.cpp  update.cpp delete.cpp load.cpp Quary.cpp saving.cpp