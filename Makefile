all: release

remote: server client
release: main.cpp Table.cpp Database.cpp  whereclause.cpp update.cpp delete.cpp load.cpp Quary.cpp saving.cpp calculate.cpp
	g++ -std=c++11  -o main main.cpp Table.cpp Database.cpp  whereclause.cpp  update.cpp delete.cpp load.cpp Quary.cpp saving.cpp calculate.cpp datetime.cpp
local: Table.o Database.o  whereclause.o update.o delete.o load.o Quary.o saving.o calculate.o 
	g++ -std=c++11  -o main -DLOCAL main.cpp Table.o Database.o  whereclause.o update.o delete.o load.o Quary.o saving.o calculate.o datetime.cpp
server:
	g++ -std=c++11 -DREMOTE -o server main.cpp Table.cpp Database.cpp  whereclause.cpp update.cpp delete.cpp load.cpp Quary.cpp saving.cpp calculate.cpp datetime.cpp server.cpp
client: client.cpp
	g++ -std=c++11 -o client client.cpp
Table.o: 
	g++ -std=c++11 -o Table.o -c Table.cpp
Database.o:
	g++ -std=c++11 -o Database.o -c Database.cpp
whereclause.o:
	g++ -std=c++11 -o whereclause.o -c whereclause.cpp
update.o:
	g++ -std=c++11 -o update.o -c update.cpp
delete.o:
	g++ -std=c++11 -o delete.o -c delete.cpp
load.o: 
	g++ -std=c++11 -o load.o -c load.cpp
Quary.o: 
	g++ -std=c++11 -o Quary.o -c Quary.cpp
saving.o: 
	g++ -std=c++11 -o saving.o -c saving.cpp
calculate.o: 
	g++ -std=c++11 -o calculate.o -c calculate.cpp
clean:
	rm *.o server client main