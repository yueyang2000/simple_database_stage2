#include "Quary.h"
#pragma once
class Group
{
    vector<colbase*> record;
    int num{0};
public:
    void add_column(colbase* col){
        string t=col->gettype();
        string cname=col->getname();
        colbase* temp;
        if(t=="INT")
        {
            temp=new Column<int>(cname,0,"INT");
        }
        else if(t=="CHAR")
        {
            temp=new Column<string>(cname,0,"CHAR");
        }
        else if(t=="DOUBLE")
        {
            temp=new Column<double>(cname,0,"DOUBLE");
        }
        else{}
        record.push_back(temp);
    }
    ~Group(){
        for(int i=0;i<record.size();i++){
            delete record[i];
        }
    }
    int get_id(vector<colbase*>& group,int r){
        for(int i=0;i<num;i++){
            bool row_same=true;
            for(int j=0;j<record.size();j++){
                colbase* ptr1=record[j];
                colbase* ptr2=group[j];
                if(ptr1->gettype()=="INT"){
                    Column<int>* cp1=dynamic_cast<Column<int>*>(ptr1);
                    Column<int>* cp2=dynamic_cast<Column<int>*>(ptr2);
                    if(cp1->getvalue(i)!=cp2->getvalue(r)){
                        row_same=false;
                        break;
                    }
                }
                else if(ptr1->gettype()=="DOUBLE"){
                    Column<double>* cp1=dynamic_cast<Column<double>*>(ptr1);
                    Column<double>* cp2=dynamic_cast<Column<double>*>(ptr2);
                    if(cp1->getvalue(i)!=cp2->getvalue(r)){
                        row_same=false;
                        break;
                    }
                }
                else if(ptr1->gettype()=="CHAR"){
                    Column<string>* cp1=dynamic_cast<Column<string>*>(ptr1);
                    Column<string>* cp2=dynamic_cast<Column<string>*>(ptr2);
                    if(cp1->getvalue(i)!=cp2->getvalue(r)){
                        row_same=false;
                        break;
                    }
                }
                else{}
            }
            if(row_same){
                return i;
            }
        }
        //循环结束还没返回，说明是新一种字段
        num++;
        for(int j=0;j<record.size();j++){
            if(record[j]->gettype()=="INT"){
                auto cp1=dynamic_cast<Column<int>*>(record[j]);
                auto cp2=dynamic_cast<Column<int>*>(group[j]);
                cp1->data.push_back(cp2->getdata(r));
            }
            else if(record[j]->gettype()=="DOUBLE"){
                auto cp1=dynamic_cast<Column<double>*>(record[j]);
                auto cp2=dynamic_cast<Column<double>*>(group[j]);
                cp1->data.push_back(cp2->getdata(r));
            }
            else if(record[j]->gettype()=="CHAR"){
                auto cp1=dynamic_cast<Column<string>*>(record[j]);
                auto cp2=dynamic_cast<Column<string>*>(group[j]);
                cp1->data.push_back(cp2->getdata(r));
            }
            else{}
        }
        return num-1;
    }
};
