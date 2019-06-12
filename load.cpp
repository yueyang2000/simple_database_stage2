#include "Database.h"
#include <fstream>
#include <sstream>
void Database::load()
{
	string temp;
	ss >> temp; ss >> temp; ss >> temp;
	string filename(temp.begin() + 1, temp.end() - 1);
	ss >> temp; ss >> temp;
	string tablename;
	ss >> tablename;
	tables[tablename]->loadtocolumn(filename);
}
void Table::loadtocolumn(string& filename)
{
	string temp;
	vector<string> attrname;
	while (ss >> temp)
	{
		attrname.push_back(temp);
	}
	ifstream fin(filename);
	string record;
	while (getline(fin, record))
	{
		//cout<<record<<endl;
		vector<string> value;
		stringstream trans(record);
		string t;
		while (trans >> t)
		{
			value.push_back(t);
		}
		int pripos = 0;
		for (int i = 0; i < attrname.size(); i++) {
			//cout<<i<<':'<<attrname[i]<<endl;
			if (attrname[i] == primarykey) {//找到主键位置
				pripos = i;
				break;
			}
		}
		//    cout<<value[pripos]<<endl;
		string type = typemap[attrname[pripos]];
		if (type == "INT") {
			int k = stoi(value[pripos]);
			if (primary_int.count(k)) {//当插入数据的主键与z当前已存在的主键重复时报错
				cout << k << " as primary key has existed." << endl;
				return;
			}
			else {
				primary_int[k] = 1;
			}
		}
		else if (type == "DOUBLE") {
			double k = stod(value[pripos]);
			if (primary_double.count(k)) {
				cout << k << " as primary key has existed." << endl;
				return;
			}
			else {
				primary_double[k] = 1;
			}
		}
		else if (type == "CHAR") {
			string k(value[pripos].begin() + 1, value[pripos].end() - 1);
			if (primary_char.count(k)) {
				cout << k << " as primary key has existed." << endl;
				return;
			}
			else {
				primary_char[k] = 1;
			}
		}
		else if (type == "DATE") {
			string k(value[pripos].begin() + 1, value[pripos].end() - 1);
			if (primary_char.count(k)) {
				cout << k << " as primary key has existed." << endl;
				return;
			}
			else {
				primary_char[k] = 1;
			}
		}
		else if (type == "TIME") {
			string k(value[pripos].begin() + 1, value[pripos].end() - 1);
			if (primary_char.count(k)) {
				cout << k << " as primary key has existed." << endl;
				return;
			}
			else {
				primary_char[k] = 1;
			}
		}
		int m;
		double n;
		int loc = 0;
		if (type == "INT") {
			m = stoi(value[pripos]);
			for (int i = 0; i < size; i++) {//遍历所有行，比较主键的大小，找到正确的插入位置
				Column<int>* temp = dynamic_cast<Column<int>*> (columns[attrname[pripos]]);//指针向下类型转化为该数据类型的派生类，下同
				if (temp->cmp(i, 1, m) || temp->cmp(i, 3, m)) {
					loc = i;
					break;
				}
				loc = i + 1;
			}
		}
		else if (type == "DOUBLE") {
			n = stod(value[pripos]);
			for (int i = 0; i < size; i++) {
				Column<double>* temp = dynamic_cast<Column<double>*> (columns[attrname[pripos]]);
				if (temp->cmp(i, 1, n) || temp->cmp(i, 3, n)) {
					loc = i;
					break;
				}
				loc = i + 1;
			}
		}
		else if (type == "CHAR") {
			string s(value[pripos].begin() + 1, value[pripos].end() - 1);
			for (int i = 0; i < size; i++) {
				Column<string>* temp = dynamic_cast<Column<string>*> (columns[attrname[pripos]]);
				if (temp->cmp(i, 1, s) || temp->cmp(i, 3, s)) {
					loc = i;
					break;
				}
				loc = i + 1;
			}
		}
		else if (type == "DATE") {
			string s(value[pripos].begin() + 1, value[pripos].end() - 1);
			for (int i = 0; i < size; i++) {
				Column<string>* temp = dynamic_cast<Column<string>*> (columns[attrname[pripos]]);
				if (temp->cmp(i, 1, s) || temp->cmp(i, 3, s)) {
					loc = i;
					break;
				}
				loc = i + 1;
			}
		}
		else if (type == "TIME") {
			string s(value[pripos].begin() + 1, value[pripos].end() - 1);
			for (int i = 0; i < size; i++) {
				Column<string>* temp = dynamic_cast<Column<string>*> (columns[attrname[pripos]]);
				if (temp->cmp(i, 1, s) || temp->cmp(i, 3, s)) {
					loc = i;
					break;
				}
				loc = i + 1;
			}
		}
		//   cout<<loc<<endl;
		map<string, colbase*>::iterator iter;
		iter = columns.begin();
		while (iter != columns.end()) {//初始化占位（一整行，若isnull为1，则此次没有加入该位置数据），保证行的对齐
			if (typemap[iter->first] == "CHAR") {
				Column<string>* temp = dynamic_cast<Column<string>*>(iter->second);
				string value(iter->first.begin() + 1, iter->first.end() - 1);
				temp->initialdata("/", loc);
			}
			else if (typemap[iter->first] == "INT") {
				Column<int>* temp = dynamic_cast<Column<int>*>(iter->second);
				temp->initialdata(0, loc);
			}
			else if (typemap[iter->first] == "DOUBLE") {
				Column<double>* temp = dynamic_cast<Column<double>*>(iter->second);
				temp->initialdata(0, loc);
			}
			else if (typemap[iter->first] == "DATE") {
				Column<string>* temp = dynamic_cast<Column<string>*>(iter->second);
				string value(iter->first.begin() + 1, iter->first.end() - 1);
				temp->initialdata("/", loc);
			}
			else if (typemap[iter->first] == "TIME") {
				Column<string>* temp = dynamic_cast<Column<string>*>(iter->second);
				string value(iter->first.begin() + 1, iter->first.end() - 1);
				temp->initialdata("/", loc);
			}
			iter++;
		}
		int inserttimes = (int)value.size();
		string attrvalue;
		for (int i = 0; i < inserttimes; i++) {//真正的赋值操作，调用Column中的插入函数
			attrvalue = value[i];
			if(attrvalue=="NULL") continue;
			if (typemap[attrname[i]] == "CHAR") {
				Column<string>* temp = dynamic_cast<Column<string>*>(columns[attrname[i]]);
				//string value(attrvalue.begin()+1,attrvalue.end()-1);
				temp->insertdata(attrvalue, loc);
			}
			else if (typemap[attrname[i]] == "INT") {
				Column<int>* temp = dynamic_cast<Column<int>*>(columns[attrname[i]]);
				int a;
				a = stoi(attrvalue);
				temp->insertdata(a, loc);
			}
			else if (typemap[attrname[i]] == "DOUBLE") {
				Column<double>* temp = dynamic_cast<Column<double>*>(columns[attrname[i]]);
				double a;
				a = stod(attrvalue);
				temp->insertdata(a, loc);
			}
			else if (typemap[attrname[i]] == "DATE") {
				Column<string>* temp = dynamic_cast<Column<string>*>(columns[attrname[i]]);
				//string value(attrvalue.begin()+1,attrvalue.end()-1);
				temp->insertdata(attrvalue, loc);
			}
			else if (typemap[attrname[i]] == "TIME") {
				Column<string>* temp = dynamic_cast<Column<string>*>(columns[attrname[i]]);
				//string value(attrvalue.begin()+1,attrvalue.end()-1);
				temp->insertdata(attrvalue, loc);
			}
		}
		size++;//扩行完成
	}
	fin.close();
}

