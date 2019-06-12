

#ifndef CODE_DATASTRUCT_H
#define CODE_DATASTRUCT_H

#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <stdlib.h>
using namespace std;
class Database;
extern Database* whichdb;
//指向当前被使用的数据库
extern string operation;
//承接当前输入的字符串
extern map<string, Database*> database;
//所有数据库的集合，可以直接用数据库的名称访问
extern stringstream ss;
//字符串流的对象
template <typename A>
struct datapoint {
	A val;
	//根据实际类型存储数据
	bool isnull;
	//判断该点位置是否已输入数据
public:
	datapoint<A>(A x, bool y) : val(x), isnull(y) {};
	//构造函数，根据实际类型构造对象
	void output()
		//根据此处是否有值决定输出的是NULL还是实际值
	{
		if (isnull)
			cout << "NULL";
		else
			cout << val;
		return;
	}
};



#endif //CODE_DATASTRUCT_H

