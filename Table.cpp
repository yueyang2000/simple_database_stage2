
#include "Table.h"

void stringtoUpper(string& x);

Table::~Table() {//析构函数，用迭代器逐个释放指针对应的内存
	map<string, colbase*>::iterator it;
	it = columns.begin();
	while (it != columns.end())
	{
		delete it->second;//删除指针，释放内存
		it++;
	}
	columns.clear();//删除vector
//    cout<<"destruct table "<<tname<<endl;
}


void Table::createcolumn(string ccname) {//创建新列的函数
	string ttype;
	ss >> ttype;//输入类型
	stringtoUpper(ttype);
	string k;
	ss >> k;//输入值
	stringtoUpper(k);//大写规范化
	//    int m=count(ttype.begin(),ttype.end(),',');
	if (k == "NOT") {
		//按照该列是否为notnull构造新列
		//按照三种不同类型来构造新列
		if (ttype == "INT") {
			colbase* ptr = new Column<int>(ccname, true, "INT");
			//            cout<<ccname<<' '<<ttype<<" NOT NULL"<<endl;
			//            stringtoUpper(ccname);
			columns[ccname] = ptr;
			typemap[ccname] = "INT";
		}
		else if (ttype == "CHAR") {
			colbase* ptr = new Column<string>(ccname, true, "CHAR");
			//            cout<<ccname<<' '<<ttype<<" NOT NULL"<<endl;
			//            stringtoUpper(ccname);
			columns[ccname] = ptr;
			typemap[ccname] = "CHAR";
		}
		else if (ttype == "DOUBLE") {
			colbase* ptr = new Column<double>(ccname, true, "DOUBLE");
			//            cout<<ccname<<' '<<ttype<<" NOT NULL"<<endl;
			//            stringtoUpper(ccname);
			columns[ccname] = ptr;
			typemap[ccname] = "DOUBLE";
		}
		else if (ttype == "DATE") {
			colbase* ptr = new Column<string>(ccname, true, "DATE");
			//            cout<<ccname<<' '<<ttype<<" NOT NULL"<<endl;
			//            stringtoUpper(ccname);
			columns[ccname] = ptr;
			typemap[ccname] = "DATE";
		}
		else if (ttype == "TIME") {
			colbase* ptr = new Column<string>(ccname, true, "TIME");
			//            cout<<ccname<<' '<<ttype<<" NOT NULL"<<endl;
			//            stringtoUpper(ccname);
			columns[ccname] = ptr;
			typemap[ccname] = "TIME";
		}
		else {//当类型是当前不支持的时候，报告错误
			cout << "Alert: type of " << ccname << " is not acceptable." << endl;
			order.pop_back();//将order中加入的错误代码删除
		}
		ss >> ttype;
	}
	else {
		for (int i = 0; i < k.length(); i++) {
			ss.unget();//将k（类型名）回流
		}
		if (ttype == "INT") {
			colbase* ptr = new Column<int>(ccname, false, "INT");
			columns[ccname] = ptr;
			typemap[ccname] = "INT";
		}
		else if (ttype == "CHAR") {
			colbase* ptr = new Column<string>(ccname, false, "CHAR");
			columns[ccname] = ptr;
			typemap[ccname] = "CHAR";
		}
		else if (ttype == "DOUBLE") {
			colbase* ptr = new Column<double>(ccname, false, "DOUBLE");
			columns[ccname] = ptr;
			typemap[ccname] = "DOUBLE";
		}
		else if (ttype == "DATE") {
			colbase* ptr = new Column<string>(ccname, false, "DATE");
			columns[ccname] = ptr;
			typemap[ccname] = "DATE";
		}
		else if (ttype == "TIME") {
			colbase* ptr = new Column<string>(ccname, false, "TIME");
			columns[ccname] = ptr;
			typemap[ccname] = "TIME";
		}
		else {
			cout << "Alert: type of " << ccname << " is not acceptable." << endl;
			order.pop_back();
		}

	}
}

Table::Table(string x) :tname(x) {//构造函数
	//stu_id INT NOT NULL, stu_name CHAR, PRIMARY KEY(stu_id)
	string k;
	while (ss >> k) {
		string cmp = k;
		stringtoUpper(cmp);//全大写处理
		if (cmp != "PRIMARY") {//根据是否为主键决定构造该表的操作
			order.push_back(k);//加入记录顺序的序列
			createcolumn(k);//构造新列
		}
		else {
			ss >> k;
			ss >> k;
			primarykey = k;//设置主键名称
			if (columns.count(k))
				columns[k]->setkey();//更改主键列的主键属性
		}
	}
}

void Table::inserttocolumn() {//新建行的操作，首先找到插入位置，再进行操作

	string attrvalue;
	string currentname;
	vector<string> attrname;//承接输入，暂存值的名称
	vector<string> value;//承接输入，暂存值的数值
	bool* ptr = new bool[order.size()];//寻找将要被加入新值的列
	for (int i = 0; i < order.size(); i++) {
		ptr[i] = false;
	}
	int inserttimes = 0;//本次给予了这一行中多少个位置的实际值。
	ss >> currentname;
	//    stringtoUpper(currentname);
	string cmp = currentname;
	stringtoUpper(cmp);
	attrname.push_back(currentname);//暂存列名
	for (int i = 0; i < order.size(); i++) {//找到目标列
		if (order[i] == currentname) {
			ptr[i] = true;
			break;
		}
	}
	while (cmp != "VALUES") {
		ss >> currentname;
		//        stringtoUpper(currentname);
		cmp = currentname;
		for (int i = 0; i < order.size(); i++) {
			if (order[i] == currentname) {
				ptr[i] = true;
				break;
			}
		}
		stringtoUpper(cmp);
		attrname.push_back(currentname);
		inserttimes++;
	}
	for (int i = 0; i < order.size(); i++) {
		if (ptr[i] == false) {
			if (columns[order[i]]->ifnotnull()) {//当不能为空的列没有输入值，报错
				cout << "Alert: attribute " << order[i] << " cannot be null." << endl;
				delete[] ptr;//释放错误指令产生的内存
				return;
			}
		}
	}
	delete[] ptr;//释放临时指针内存
	int valuetimes = 0;
	while (ss >> attrvalue) {
		valuetimes++;
		value.push_back(attrvalue);
	}
	if (valuetimes != inserttimes) {
		cout << "Alert: the numbers of attributes and values don't match." << endl;
		return;
	}
	int pripos = 0;
	for (int i = 0; i < attrname.size(); i++) {
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
	else if (type=="CHAR") {
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
	//    cout<<loc<<endl;
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
	for (int i = 0; i < inserttimes; i++) {//真正的赋值操作，调用Column中的插入函数
		attrvalue = value[i];
		if (typemap[attrname[i]] == "CHAR") {
			Column<string>* temp = dynamic_cast<Column<string>*>(columns[attrname[i]]);
			string value(attrvalue.begin() + 1, attrvalue.end() - 1);
			temp->insertdata(value, loc);
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
			string value(attrvalue.begin() + 1, attrvalue.end() - 1);
			temp->insertdata(value, loc);
		}
		else if (typemap[attrname[i]] == "TIME") {
			Column<string>* temp = dynamic_cast<Column<string>*>(columns[attrname[i]]);
			string value(attrvalue.begin() + 1, attrvalue.end() - 1);
			temp->insertdata(value, loc);
		}
	}
	size++;//扩行完成


}


void Table::showname() {//输出表格名的接口
	cout << tname << endl;
}

void Table::showinfo() {//输出表格（表头与数据）
	cout << "Field" << '\t' << "Type" << '\t' << "Null" << '\t' << "Key" << '\t' << "Default" << '\t' << "Extra" << endl;
	for (int i = 0; i < order.size(); i++) {
		columns[order[i]]->showname();
	}
}
