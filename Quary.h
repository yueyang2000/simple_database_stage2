


#ifndef QUARY_H_
#define QUARY_H_
#include "Database.h"
#include <set>
class Quary
{
private:
    static set<string> keywords;
    vector<string> words;
    vector<string> col_name;
    vector<bool> col_output;
    vector<string> as;
    map<string,colbase*> result;
    map<string,Table*> use_table;
    vector<string> group;
    vector<int> group_id;
    int max_id{0};//比最大id大1
    string order_by;
    bool has_function{false};
    bool select_all{false};
    int where_begin{-1};
    int where_end{-1};
    bool simple_mode{false};
    string filename;
    vector<bool> pick;
    int row{0};//行数

    int UNION{-1};
public:
    void debug();
    Quary(string& sql);
    void parser();
    void execute();
    void simple_create_column();
    void simple_where_clause();
    bool simple_judge(string& str,int r);
    void simple_calculate();//纯计算器
    void get_result();//根据groupby_id生成计算结果
    void get_groupby_id();
    void sort();
    void swap_row(int,int);
    void output();
    ~Quary();
};


#endif
