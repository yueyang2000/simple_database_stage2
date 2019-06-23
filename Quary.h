


#ifndef QUARY_H_
#define QUARY_H_
#include "Database.h"
#include <set>
class Quary
{
private:
	static set<string> keywords;//关键词
	vector<string> words;//存储SQL语句
	vector<string> col_name;//存储该查询涉及的所有列名和算式
	vector<bool> col_output;//存储对应col_name是否需要输出
	vector<string> as;//存储类型别名
	map<string, colbase*> result;//
	vector<string> group;
	vector<int> group_id;
	int max_id{ 0 };//比最大id大1
	string order_by;
	bool has_function{ false };
	bool select_all{ false };
	int where_begin{ -1 };
	int where_end{ -1 };
	bool simple_mode{ false };
	string filename;
	vector<bool> pick;
	int row{ 0 };//行数
	bool ASC{true};
	//本查询所对应表格的指针、行数
	Table* local;
	int rnum;
public:
	void debug();
	Quary(string& sql);
	void parser();
	void execute();
	void create_column();
	void where_clause();
	bool judge(string& str, int r);
	void get_result();//根据groupby_id生成计算结果
	void get_groupby_id();
	void sort();
	void swap_row(int, int);
	void output();
	~Quary();
	friend class Union;
};


#endif
