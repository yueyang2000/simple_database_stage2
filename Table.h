

#ifndef CODE_TABLE_H
#define CODE_TABLE_H

#include "Column.h"
bool Sign();
class Table{
private:
    string tname; 
    //表格名称
    string primarykey;
    //表格的主键名
    int size=0;
    //表格所含列数  //是行数好吗？？i fule you
    vector <string> order;
    //记录表格中各列的排序顺序
    map<string, colbase*> columns;
    //存放指向各列的指针
    map<string,string> typemap;
    //快速查找type
    map<int,bool>primary_int;
    map<double,bool>primary_double;
    map<string,bool> primary_char;
    //将主键是否存在作为映射，避免主键重复的情况，根据不同类型设了三个map
    int rnum;
public:
    Table(string x);//构造函数
    void createcolumn(string k);
    //创建一个新的列
    void inserttocolumn();
    //插入列的接口
    void loadtocolumn(string& filename);
    bool compare(string x){//访问表格名称并比较与字符串是否一样
        return x==tname;
    }//new
    int getsize(){//访问size的接口
        return size;
    }
    string showtype(string& x){//访问type的接口
        return typemap[x];
    }
    void showinfo();//打印出表格的表头
    void showname();//打印列名
    ~Table();
    template <class T>
    void cmpval(T val,string& x,int* p, int k){//访问column中比较函数的接口
        Column<T>* ptr= dynamic_cast<Column<T>*>(columns[x]);
        for(int i=0;i<size;i++){
            if(ptr->cmp(i,k,val)){
                continue;
            }
            else{
                p[i]=0;
            }
        }
    }
    void select();//
    void deletedata();//访问Column中删除数据点的接口，同时更改主键信息
    void update();//根据主键大小将被操作的行换至改变后应当在的位置，然后更改这一行的值（调用Column中的相应操作）
    void select(vector<string> name,int total);
    void swap(Table*,int i1,int i2);//用于交换vector中的两行的函数

    friend class Quary;
    template<class T>
    bool compare(int pos,string& x,int k,T value){
        Column<T>* ptr= dynamic_cast<Column<T>*>(columns[x]);
        return ptr->cmp(pos,k,value);
    }
};

int* whereclause(Table* table);//处理whereclause语句，返回符合条件的行号
#endif //CODE_TABLE_H

