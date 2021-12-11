	#include<iostream>
	#include"Database.h"
		
	void Database::deletedata()
	{
		string op;
		ss>>op;
		ss>>op;
//		string x=stringtoUpper(op);
		if(tables.count(op))
		tables[op]->deletedata();
		else{
			cout<<"no table named "<<op<<" exists."<<endl;
		}
		return;
	}
	
	void Table::deletedata()
	{//根据whereclause计算所得的删除位置直接将行删去，同时还要将主键的值清除掉
		if(size==0){//如果表为空，输出提示，终止操作
			cout<<"Alert: the table has already been empty."<<endl;
			return;
		}
		string op;
		int *outcome;
		ss>>op;
		stringtoUpper(op);
		if(op=="WHERE")
			outcome=whereclause(this);
		else{//如果没有whereclause对整个表进行操作
			outcome=new int [size];
			for(int i=0;i<size;i++){
				outcome[i]=1;
			}
		}
		for(int i=size-1;i>=0;i--)
		{
			if(outcome[i])
			{
				size--;

				map<string,colbase*>::iterator it;
				it=columns.begin();
				while(it!=columns.end())
				{
					if(it->first!=primarykey) {
						if (typemap[it->first] == "INT") {
							Column<int> *ptr = dynamic_cast<Column<int> *>(it->second);
							ptr->deletedata(i);
						} else if (typemap[it->first] == "DOUBLE") {
							Column<double> *ptr = dynamic_cast<Column<double> *>(it->second);
							ptr->deletedata(i);
						} else if (typemap[it->first] == "CHAR") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(it->second);
							ptr->deletedata(i);
						}
						else if (typemap[it->first] == "DATE") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(it->second);
							ptr->deletedata(i);
						}
						else if (typemap[it->first] == "TIME") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(it->second);
							ptr->deletedata(i);
						}
					}
					else{
						if (typemap[it->first] == "INT") {
							Column<int> *ptr = dynamic_cast<Column<int> *>(it->second);
							int k=ptr->getvalue(i);
							primary_int.erase(k);
							ptr->deletedata(i);
						} else if (typemap[it->first] == "DOUBLE") {
							Column<double> *ptr = dynamic_cast<Column<double> *>(it->second);
							double k=ptr->getvalue(i);
							primary_double.erase(k);
							ptr->deletedata(i);
						} else if (typemap[it->first] == "CHAR") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(it->second);
							string k=ptr->getvalue(i);
							primary_char.erase(k);
							ptr->deletedata(i);
						}
						else if (typemap[it->first] == "DATE") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(it->second);
							string k = ptr->getvalue(i);
							primary_char.erase(k);
							ptr->deletedata(i);
						}
						else if (typemap[it->first] == "TIME") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(it->second);
							string k = ptr->getvalue(i);
							primary_char.erase(k);
							ptr->deletedata(i);
						}
					}
					it++;
				}				
			}
		}
		delete[] outcome;
	}
	
//	void Column::deletedata(int i)
//	{
//		data.erase(data.begin()+i);
//		return;
//	}
