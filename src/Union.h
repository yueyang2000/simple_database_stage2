

#ifndef UNION_H_
#define UNION_H_
#include <string>
#include "Quary.h"
using namespace std;

class Union
{
    Quary* quary1;
    Quary* quary2;
    string order_by;
    string exp;
public:
    Union(string&expression):exp(expression){parser();}
    void parser(){//SELECT ... FROM UNION SELECT .... FROM  ORDER BY ...
        int pos=exp.find("UNION");
        int pos1=exp.find("ORDER");
        string exp1(exp.begin(),exp.begin()+pos-1);
        string exp2(exp.begin()+pos+6,exp.begin()+pos1-1);
        auto it=exp.end()-1;
        while(*it!=' '){
            it--;
        }
        order_by=string(it+1,exp.end());
        cout<<exp1<<' '<<exp2<<' '<<order_by<<endl;
        quary1=new Quary(exp1);
        quary2=new Quary(exp2);
    }
    void execute(){
        quary1->execute();
        quary2->execute();//分别执行查询
        transform_data();//将quary2的所有结果对接到quary1中
        erase_duplicate();//在quary1中去重
        sort();//调用quary1排序
        output();//调用quary1输出结果
    }
    void erase_duplicate(){
        quary1->group.clear();
        for(int j=0;j!=quary1->col_name.size()&&quary1->col_output[j];j++){
            quary1->group.push_back(quary1->col_name[j]);
        }
        quary1->group_id.clear();
        quary1->get_groupby_id();//为新的元素分发组号
        for(int i=0;i<quary1->max_id;i++){//去重
            
        }
    }
    void transform_data(){
        for(int j=0;j!=quary1->col_name.size()&&quary1->col_output[j];j++){//select的列名
            string cname=quary1->col_name[j];
            handle_col column(quary2->result[cname],quary1->result[cname]);//从quary2转移到quary1
            for(int i=0;i<quary2->row;i++){
                column.transform_data(i);
            }
        }
    }
    void sort(){
        quary1->order_by=order_by;
        quary1->sort();
    }
    void output();
    ~Union(){
        delete quary1;
        delete quary2;
    }
};



#endif