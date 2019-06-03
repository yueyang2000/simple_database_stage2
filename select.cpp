	#include<iostream>
	#include"Database.h"
	
	void Database::select()
	{
		string op;
		ss>>op;
		if(op=="*")//输出表格所有行
		{
			ss>>op;
			ss>>op;
			if(tables.count(op))
			tables[op]->select();
			else{
				cout<<"no table named "<<op<<" exists."<<endl;
				return;
			}
		}
		else
		{//对于输出部分行的情况，依然要首先找到查找的表格
			vector<string> name;
			int total=0;
			while(true)
			{
				string temp;
				name.push_back(op);
				ss>>op;
				total++;
				string tem=op;
				stringtoUpper(op);
				temp=op;
				if(temp=="FROM")
					break;
				op=tem;
			}
			ss>>op;
			if(tables.count(op))
			tables[op]->select(name,total);
			else{
				cout<<"no table named "<<op<<" exists."<<endl;
				return;
			}
		}
	}
	
	void Table::select()
	{
        if(size==0){
            return;
        }
		string op;

		ss>>op;

		stringtoUpper(op);
		if(op=="WHERE"){
			int* outcome;
			outcome=whereclause(this);//用whereclause函数计算出需要操作的行号
			bool empty=true;//看看有没有找到符合要求的行
			for(int i=0;i<size;i++){
				if(outcome[i]==1){
					empty=false;
					break;
				}
			}
			if(empty){//搜索结果为空
			    delete[] outcome;
				return;
			}
			for(int i=0;i<order.size()-1;i++){
				cout<<order[i]<<'\t';
			}
			cout<<order[order.size()-1]<<endl;
			for(int i=0;i<size;i++){
			    if(outcome[i]==0)continue;
			    else{
                    for(int j=0;j<order.size()-1;j++) {
						if (typemap[order[j]] == "INT") {
							Column<int> *ptr = dynamic_cast<Column<int> *>(columns[order[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[order[j]] == "DOUBLE") {
							Column<double> *ptr = dynamic_cast<Column<double> *>(columns[order[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[order[j]] == "CHAR") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(columns[order[j]]);
							ptr->select(i);
							cout << '\t';
						}
					}
                    //末尾换行
                    if (typemap[order[order.size()-1]] == "INT") {
                    	Column<int> *ptr = dynamic_cast<Column<int> *>(columns[order[order.size()-1]]);
                    	ptr->select(i);
                    	cout << endl;
                    } else if (typemap[order[order.size()-1]] == "DOUBLE") {
                    	Column<double> *ptr = dynamic_cast<Column<double> *>(columns[order[order.size()-1]]);
                    	ptr->select(i);
                    	cout << endl;
                    } else if (typemap[order[order.size()-1]] == "CHAR") {
                    	Column<string> *ptr = dynamic_cast<Column<string> *>(columns[order[order.size()-1]]);
                    	ptr->select(i);
                    	cout << endl;
                    }
			    }
			}
			delete[] outcome;
		}
		else {//直接输出全表各行
			for(int i=0;i<order.size()-1;i++){
				cout<<order[i]<<'\t';
			}
			cout<<order[order.size()-1]<<endl;
			int *outcome = new int[size];
			for (int i = 0; i < size; i++)
				outcome[i] = 1;
			for(int i=0;i<size;i++){
				if(outcome[i]==0)continue;
				else{
					for(int j=0;j<order.size()-1;j++) {
						if (typemap[order[j]] == "INT") {
							Column<int> *ptr = dynamic_cast<Column<int> *>(columns[order[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[order[j]] == "DOUBLE") {
							Column<double> *ptr = dynamic_cast<Column<double> *>(columns[order[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[order[j]] == "CHAR") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(columns[order[j]]);
							ptr->select(i);
							cout << '\t';
						}
					}

					if (typemap[order[order.size()-1]] == "INT") {
						Column<int> *ptr = dynamic_cast<Column<int> *>(columns[order[order.size()-1]]);
						ptr->select(i);
						cout << endl;
					} else if (typemap[order[order.size()-1]] == "DOUBLE") {
						Column<double> *ptr = dynamic_cast<Column<double> *>(columns[order[order.size()-1]]);
						ptr->select(i);
						cout << endl;
					} else if (typemap[order[order.size()-1]] == "CHAR") {
						Column<string> *ptr = dynamic_cast<Column<string> *>(columns[order[order.size()-1]]);
						ptr->select(i);
						cout << endl;
					}
				}
			}
			delete[] outcome;
		}
	}

	
	void Table::select(vector<string> name,int total)//处理选择部分数据的情况
	{
		string op;
		ss>>op;
		stringtoUpper(op);
		if(op=="WHERE")
		{
			int* outcome;
			outcome=whereclause(this);
			bool empty=true;
			for(int i=0;i<size;i++){
				if(outcome[i]==1){
					empty=false;
					break;
				}
			}
			if(empty){
				return;
			}
			for(int i=0;i<total-1;i++){
				cout<<name[i]<<'\t';
			}
			cout<<name[total-1]<<endl;
			for(int i=0;i<size;i++){
				if(outcome[i]==0)continue;
				else{
					for(int j=0;j<total-1;j++) {
						if (typemap[name[j]] == "INT") {
							Column<int> *ptr = dynamic_cast<Column<int> *>(columns[name[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[name[j]] == "DOUBLE") {
							Column<double> *ptr = dynamic_cast<Column<double> *>(columns[name[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[name[j]] == "CHAR") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(columns[name[j]]);
							ptr->select(i);
							cout << '\t';
						}
					}

					if (typemap[name[total-1] ]== "INT") {
						Column<int> *ptr = dynamic_cast<Column<int> *>(columns[name[total-1]]);
						ptr->select(i);
						cout << endl;
					} else if (typemap[name[total-1]] == "DOUBLE") {
						Column<double> *ptr = dynamic_cast<Column<double> *>(columns[name[total-1]]);
						ptr->select(i);
						cout << endl;
					} else if (typemap[name[total-1]] == "CHAR") {
						Column<string> *ptr = dynamic_cast<Column<string> *>(columns[name[total-1]]);
						ptr->select(i);
						cout << endl;
					}
				}
			}
			delete[] outcome;
		}
		else
		{
			if(size==0){
				return;
			}
			for(int i=0;i<total-1;i++){
				cout<<name[i]<<'\t';
			}
			cout<<name[total-1]<<endl;
			int* outcome=new int[size];
			for(int i=0;i<size;i++)
				outcome[i]=1;
			for(int i=0;i<size;i++){
				if(outcome[i]==0)continue;
				else{
					for(int j=0;j<total-1;j++) {
						if (typemap[name[j]] == "INT") {
							Column<int> *ptr = dynamic_cast<Column<int> *>(columns[name[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[name[j]] == "DOUBLE") {
							Column<double> *ptr = dynamic_cast<Column<double> *>(columns[name[j]]);
							ptr->select(i);
							cout << '\t';
						} else if (typemap[name[j]] == "CHAR") {
							Column<string> *ptr = dynamic_cast<Column<string> *>(columns[name[j]]);
							ptr->select(i);
							cout << '\t';
						}
					}

					if (typemap[name[total-1] ]== "INT") {
						Column<int> *ptr = dynamic_cast<Column<int> *>(columns[name[total-1]]);
						ptr->select(i);
						cout << endl;
					} else if (typemap[name[total-1]] == "DOUBLE") {
						Column<double> *ptr = dynamic_cast<Column<double> *>(columns[name[total-1]]);
						ptr->select(i);
						cout << endl;
					} else if (typemap[name[total-1]] == "CHAR") {
						Column<string> *ptr = dynamic_cast<Column<string> *>(columns[name[total-1]]);
						ptr->select(i);
						cout << endl;
					}
				}
			}

		}
	}

	
	
