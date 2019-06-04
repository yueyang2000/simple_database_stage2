

#include "Database.h"
#ifndef QUARY_H_
#define QUARY_H_
#include <set>
class Quary
{
private:
    static set<string> keywords;
    vector<string> words;
    vector<string> col_name;
    vector<string> as;
    map<string,colbase*> result;
    map<string,Table*> use_table;
    vector<string> group;
    string order_by;
    bool has_function{false};
    int UNION{-1};
    int order{-1};
    bool select_all{false};
    int where_begin{-1};
    int where_end{-1};
    string filename;
    vector<bool> pick;
    int row;
public:
    void debug();
    Quary(string& sql);
    void parser();
    void execute();
    void simple_create_column();
    void simple_where_clause();
    bool simple_judge(string& str,int r);
    void simple_insert();
    
    void output();
    ~Quary();
};


#endif
