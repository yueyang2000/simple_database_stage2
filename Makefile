all: main.cpp Table.cpp Database.cpp Column.cpp whereclause.cpp
	g++ -std=c++11 -o main main.cpp Table.cpp Database.cpp Column.cpp whereclause.cpp select.cpp update.cpp delete.cpp load.cpp Quary.cpp