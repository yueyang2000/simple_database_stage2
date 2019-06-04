
#ifndef CODE_COLUMN_H
#define CODE_COLUMN_H

#include "datastruct.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;

class colbase{
protected:
    string cname;
    //列的名称
    string type;
    //列中数据的类型
    bool notnull;//是否是NOT NULL
    //该列的数据是否不能为空
    bool primarykey{false};
    
public:
    virtual ~colbase(){};
    //虚析构函数
    bool ifnotnull(){//访问nutnull的接口
        return notnull;
    }
    void showname(){
        cout<<cname;
        if(type=="INT")cout<<'\t'<<"int(11)";//输出类型
        else if(type=="CHAR")cout<<'\t'<<"char(1)";
        else cout<<'\t'<<"double";
        if(notnull)cout<<'\t'<<"NO";//输出notnull
        else cout<<'\t'<<"YES";
        if(primarykey) cout<<'\t'<<"PRI";//输出是否j主键
        else cout<<'\t';
        cout<<'\t'<<"NULL"<<endl;
    }
    virtual void setkey()=0;
    //纯虚函数，在派生类中实现
    colbase(string& x,bool notnull, const string& type):cname(x), notnull(notnull), type(type){}//cname(x),notnull标记是否为null
    //构造函数
    virtual bool getnull(int i)=0;
    string gettype(){return type;}
    string getname(){return cname;}
};


template <typename T>
class Column: public colbase{
private:
    vector<datapoint<T>> data;
    //用vector容器存储本列中的数据，每列中的数据类型根据其实际类型特化
public:
    ~Column();
    //析构函数
    Column(string& x,bool notnull, const string& type):colbase(x,notnull,type){}
    //构造函数
    void insertdata(T x, int place){//插入数据的接口，需要将isnull改为false
        data[place].isnull=false;
        data[place].val=x;
    }
    void initialdata(T x,int place){//在每次插入时需要一个占位（初始化）的过程（以避免有些数据为空导致行对不齐）
        datapoint<T> y=datapoint<T>(x,1);
        //此时标记isnull为true
        data.insert(data.begin()+place,y);
        //根据主键比大小确定的插入位置将数据插入vector之中
    }//a for value x for null or not
    void push_back(T x){
        datapoint<T> y=datapoint<T>(x,0);
        data.push_back(y);
        }
    void setkey(){
        primarykey=true;
        //更改primarykey的接口
    }
    T getvalue(int i){//访问数据点的值的接口
        return data[i].val;
    }
    datapoint<T> getdata(int i){
        return data[i];
    }
    bool getnull(int i){return data[i].isnull;}
    bool cmp(int pos, int k, T val){//用于比较（主键）值的大小的函数，注意当其中被比较对象是空（is null）时，返回false（即排在该点之前）
        //k的值还可以用来灵活改变比大还是比小或比等，一个函数多用
        if(k==1){
            if(data[pos].isnull){
                return false;
            }
            return data[pos].val>val;
        }
        else if(k==2){
            if(data[pos].isnull){
                return false;
            }
            return data[pos].val<val;
        }
        else if(k==3){
            if(data[pos].isnull){
                return false;
            }
            return data[pos].val==val;
        }
        return true;
    }
    void select(int i)
	{//输出数据点值的接口
        data[i].output();
	};
    void deletedata(int i)
	{//删除数据点的接口
		data.erase(data.begin()+i);
		return;
	};
//	template<class A>
    void update(int i,T changed)
	{//更新数据点，注意修改isnull的值
		data[i].val=changed;
		data[i].isnull=0;
		return;
	};
	template<class A>
	void swap(int i1,int i2)
	{//交换data的两个位置
	    datapoint<A> temp=data[i1];
	    data[i1]=data[i2];
	    data[i2]=temp;
		return;
	}
    int size(){return (int)data.size();}
    friend class Quary;
    friend class Group;
};

template <typename T>
Column<T>::~Column(){
    data.clear();
}
//析构函数类外实现，用于释放datal内存
#endif //CODE_COLUMN_H

