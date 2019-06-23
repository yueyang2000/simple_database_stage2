#define _CRT_SECURE_NO_WARNINGS
#include "Database.h"
//#define LOCAL
//#define REMOTE
Database* whichdb;
string operation;
stringstream ss;
map <string, Database*> database;
void save_database();
void load_database();
int server();
bool get_operation()
{
	operation = "";
	string temp;
	if (!getline(cin, temp))
	{
		return false;
	}
	operation += temp;
	while (temp[temp.size() - 1] != ';')
	{
		getline(cin, temp);
		operation += " ";
		operation += temp;
	}
	return true;
}
int main() {
	load_database();
#ifdef REMOTE
	server();
#endif
#ifdef LOCAL
	//FILE *stream;
	freopen("/Users/yueyang/yiqunyang/大一下/面向对象的程序设计基础/database/database/input.txt", "r", stdin);
#endif
	while (get_operation()) {
		//在未遇到EOF之前逐行读入字符串，每行是一个指令
		if (Process_operation(operation))break;
		//只有遇到QUIT指令时会直接跳出循环，结束进程
		save_database();
	}
#ifdef LOCAL
	fclose(stdin);
#endif
	save_database();
	return 0;
}
