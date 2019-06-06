
#ifndef CODE_DATABASE_H
#define CODE_DATABASE_H

#include "Table.h"


class Database{
private:
    string dname;
    //数据库名称
    map<string,Table*> tables;
    //数据库中的所有表格
public:
    Database(string& x);
    bool compare(string x);
    friend ostream& operator <<(ostream&,Database&);
    //重载database的输出流
    ~Database(){//析构函数，将所有table指针删除
        map<string,Table*>::iterator it;
        it=tables.begin();
        while(it!=tables.end()){
            delete it->second;
            it++;
        }
        tables.clear();
    };
    void createtable(string x);//创建表格的接口
    void inserttable();//插入数据的接口
    void droptable();//删除表格的接口
    void showname();//显示表格名的接口
    void showdb();//访问数据库名的接口
    void showtable(string& x);//打印表格的接口
    Table* gettable(string& x){//找到所需表格的接口
        return tables[x];
    }
    bool tableexist(string& x){//判断叫某名的数据库是否存在
        if(tables.count(x))return true;
        return false;
    }
    void deletedata();//删除数据的接口
    void update();//更新数据的接口
    void load();
    friend void save_database();
    friend void load_database();
};

bool Process_operation(string& operation);//处理规范字符串的接口
void createdatabase();//创建数据库的接口
void dropdatabase();//删除数据库
void usedatabase();//选择数据库
void stringtoUpper(string& op);//大写规范化函数


#endif //CODE_DATABASE_H

