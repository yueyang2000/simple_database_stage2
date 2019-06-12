#include"Database.h"

void Database::update()
{
	string op;
	ss >> op;
	if (tables.count(op))
		tables[op]->update();
	else {//当没有找到需要操作的表格时报错
		cout << "no table named " << op << " exists." << endl;
		return;
	}
}

void Table::swap(Table*, int i1, int i2)
{//对于整个表格的所有列进行两个位置的交换，从而实现行的交换
	map<string, colbase*>::iterator it;
	it = columns.begin();
	while (it != columns.end())
	{
		string name = it->first;
		string type = typemap[name];
		if (type == "INT")
		{
			Column<int>* ptr = dynamic_cast<Column<int>*>(columns[name]);
			ptr->swap<int>(i1, i2);
		}
		else if (type == "DOUBLE")
		{
			Column<double>* ptr = dynamic_cast<Column<double>*>(columns[name]);
			ptr->swap<double>(i1, i2);
		}
		else if (type == "CHAR")
		{
			Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
			ptr->swap<string>(i1, i2);
		}
		else if (type == "DATE")
		{
			Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
			ptr->swap<string>(i1, i2);
		}
		else if (type == "TIME")
		{
			Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
			ptr->swap<string>(i1, i2);
		}
		it++;
	}
	return;
}

//	template<class A>
//	void Column::swap(int i1,int i2)
//	{
//		A temp=data[i1].val;
//		data[i1].val=data[i2].val;
//		data[i2].val=temp;
//		return;
//	}	

void Table::update()
{
	if (size == 0) {//如果表为空，输出提示终止操作
		cout << "Alert: the table is empty" << endl;
		return;
	}
	string op;
	ss >> op;
	ss >> op;
	int place = 0;
	for (; place < op.size(); place++)
	{
		if (op[place] == '=')
			break;
	}
	//		cout<<place<<endl;
	string name(op, 0, place);
	string changed(op, place + 1, op.size());
	ss >> op;
	int* outcome;
	stringtoUpper(op);
	if (op == "WHERE") {
		outcome = whereclause(this);
	}
	else {//如果没有whereclause，全部操作
		outcome = new int[size];
		for (int i = 0; i < size; i++) {
			outcome[i] = 1;
		}
	}
	string type = typemap[name];
	//		cout<<name<<endl;
	//		cout<<changed<<endl;
	if (name == primarykey)
	{
		string type = typemap[primarykey];
		if (type == "INT") {
			int k = stoi(changed);
			if (primary_int.count(k)) {//更新的主键值与当前的主键值出现了重复
				cout << "Alert: " << k << " as a primarykey has existed." << endl;
				delete[] outcome;
				return;
			}
		}
		else if (type == "DOUBLE") {
			double k = stod(changed);
			if (primary_double.count(k)) {
				cout << "Alert: " << k << " as a primarykey has existed." << endl;
				delete[] outcome;
				return;
			}
		}
		else if (type == "CHAR") {
			string k(changed.begin() + 1, changed.end() - 1);
			if (primary_char.count(k)) {
				cout << "Alert: " << k << " as a primarykey has existed." << endl;
				delete[] outcome;
				return;
			}
		}
		else if (type == "DATE") {
			string k(changed.begin() + 1, changed.end() - 1);
			if (primary_char.count(k)) {
				cout << "Alert: " << k << " as a primarykey has existed." << endl;
				delete[] outcome;
				return;
			}
		}
		else if (type == "TIME") {
			string k(changed.begin() + 1, changed.end() - 1);
			if (primary_char.count(k)) {
				cout << "Alert: " << k << " as a primarykey has existed." << endl;
				delete[] outcome;
				return;
			}
		}
		map<string, colbase*>::iterator it;
		int oriplace = 0;
		int count = 0;
		for (int i = 0; i < size; i++) {
			if (count == 2)break;
			if (outcome[i] == 1) {
				count++;
			}
		}
		if (count > 1) {//将同一个主键的值赋予了多个行的时候报错
			cout << "Alert: " << changed << " as a primarykey cannot be assigned to more than one rows." << endl;
			delete[]outcome;
			return;
		}
		else {
			if (type == "INT") {
				int k = stoi(changed);
				primary_int[k] = 1;
			}
			else if (type == "DOUBLE") {
				double k = stod(changed);
				primary_double[k] = 1;
			}
			else if (type == "CHAR") {
				string k(changed.begin() + 1, changed.end() - 1);
				primary_char[k] = 1;
			}
			else if (type == "DATE") {
				string k(changed.begin() + 1, changed.end() - 1);
				primary_char[k] = 1;
			}
			else if (type == "TIME") {
				string k(changed.begin() + 1, changed.end() - 1);
				primary_char[k] = 1;
			}
		}
		for (int i = 0; i < size; i++)
		{
			if (outcome[i] == 1)
			{
				oriplace = i;
				break;
			}
		}
		if (type == "INT")
		{
			Column<int>* ptr = dynamic_cast<Column<int>*>(columns[name]);
			int k = ptr->getvalue(oriplace);
			primary_int.erase(k);
			if (ptr->cmp(oriplace, 1, stoi(changed)))//若更新的主键值比当前值小，则遍历原位置之前的各个主键值，找到合适的交换位置，后面类似
			{
				for (int i = 0; i < oriplace; i++)
				{
					if (ptr->cmp(oriplace - i - 1, 1, stoi(changed)))
					{
						swap(this, oriplace - i - 1, oriplace - i);
						int temp = outcome[oriplace - i - 1];
						outcome[oriplace - i - 1] = outcome[oriplace - i];
						outcome[oriplace - i] = temp;
					}
					else
						break;
				}
			}
			else
			{
				for (int i = 0; i < size - oriplace - 1; i++)
				{
					if (ptr->cmp(oriplace + i + 1, 2, stoi(changed)))
					{
						swap(this, oriplace + i + 1, oriplace + i);
						int temp = outcome[oriplace + i + 1];
						outcome[oriplace + i + 1] = outcome[oriplace + i];
						outcome[oriplace + i] = temp;
					}
					else
						break;
				}
			}
		}
		else if (type == "DOUBLE")
		{
			Column<double>* ptr = dynamic_cast<Column<double>*>(columns[name]);
			double k = ptr->getvalue(oriplace);
			primary_double.erase(k);
			if (ptr->cmp(oriplace, 1, stod(changed)))
			{
				for (int i = 0; i < oriplace; i++)
				{
					if (ptr->cmp(oriplace - i - 1, 1, stod(changed)))
					{
						swap(this, oriplace - i - 1, oriplace - i);
						int temp = outcome[oriplace - i - 1];
						outcome[oriplace - i - 1] = outcome[oriplace - i];
						outcome[oriplace - i] = temp;
					}
					else
						break;
				}
			}
			else
			{
				for (int i = 0; i < size - oriplace - 1; i++)
				{
					if (ptr->cmp(oriplace + i + 1, 2, stod(changed)))
					{
						swap(this, oriplace + i + 1, oriplace + i);
						int temp = outcome[oriplace + i + 1];
						outcome[oriplace + i + 1] = outcome[oriplace + i];
						outcome[oriplace + i] = temp;
					}
					else
						break;
				}
			}
		}
		else if (type == "CHAR")
		{
			string tmp(changed.begin() + 1, changed.end() - 1);
			Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
			string k = ptr->getvalue(oriplace);
			primary_char.erase(k);
			if (ptr->cmp(oriplace, 1, changed))
			{
				for (int i = 0; i < oriplace; i++)
				{
					if (ptr->cmp(oriplace - i - 1, 1, tmp))
					{
						swap(this, oriplace - i - 1, oriplace - i);
						int temp = outcome[oriplace - i - 1];
						outcome[oriplace - i - 1] = outcome[oriplace - i];
						outcome[oriplace - i] = temp;
					}
					else
						break;
				}
			}
			else
			{
				for (int i = 0; i < size - oriplace - 1; i++)
				{
					if (ptr->cmp(oriplace + i + 1, 2, tmp))
					{
						swap(this, oriplace + i + 1, oriplace + i);
						int temp = outcome[oriplace + i + 1];
						outcome[oriplace + i + 1] = outcome[oriplace + i];
						outcome[oriplace + i] = temp;
					}
					else
						break;
				}
			}
		}
		else if (type == "DATE")
		{
		string tmp(changed.begin() + 1, changed.end() - 1);
		Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
		string k = ptr->getvalue(oriplace);
		primary_char.erase(k);
		if (ptr->cmp(oriplace, 1, changed))
		{
			for (int i = 0; i < oriplace; i++)
			{
				if (ptr->cmp(oriplace - i - 1, 1, tmp))
				{
					swap(this, oriplace - i - 1, oriplace - i);
					int temp = outcome[oriplace - i - 1];
					outcome[oriplace - i - 1] = outcome[oriplace - i];
					outcome[oriplace - i] = temp;
				}
				else
					break;
			}
		}
		else
		{
			for (int i = 0; i < size - oriplace - 1; i++)
			{
				if (ptr->cmp(oriplace + i + 1, 2, tmp))
				{
					swap(this, oriplace + i + 1, oriplace + i);
					int temp = outcome[oriplace + i + 1];
					outcome[oriplace + i + 1] = outcome[oriplace + i];
					outcome[oriplace + i] = temp;
				}
				else
					break;
			}
		}
		}
		else if (type == "TIME")
		{
		string tmp(changed.begin() + 1, changed.end() - 1);
		Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
		string k = ptr->getvalue(oriplace);
		primary_char.erase(k);
		if (ptr->cmp(oriplace, 1, changed))
		{
			for (int i = 0; i < oriplace; i++)
			{
				if (ptr->cmp(oriplace - i - 1, 1, tmp))
				{
					swap(this, oriplace - i - 1, oriplace - i);
					int temp = outcome[oriplace - i - 1];
					outcome[oriplace - i - 1] = outcome[oriplace - i];
					outcome[oriplace - i] = temp;
				}
				else
					break;
			}
		}
		else
		{
			for (int i = 0; i < size - oriplace - 1; i++)
			{
				if (ptr->cmp(oriplace + i + 1, 2, tmp))
				{
					swap(this, oriplace + i + 1, oriplace + i);
					int temp = outcome[oriplace + i + 1];
					outcome[oriplace + i + 1] = outcome[oriplace + i];
					outcome[oriplace + i] = temp;
				}
				else
					break;
			}
		}
		}
	}
	//		cout<<"\n"<<type<<"\n";
	if (type == "INT")//按照上面计算得到的交换位置将行交换后更新值
	{
		Column<int>* ptr = dynamic_cast<Column<int>*>(columns[name]);
		//			cout<<"\nint\n";
		for (int i = 0; i < size; i++)
		{
			int change = stoi(changed);
			if (outcome[i])
				ptr->update(i, change);
		}
	}
	else if (type == "DOUBLE")
	{
		Column<double>* ptr = dynamic_cast<Column<double>*>(columns[name]);
		//			cout<<"\ndouble\n";
		for (int i = 0; i < size; i++)
		{
			double change = stod(changed);
			if (outcome[i])
				ptr->update(i, change);
		}
	}
	else if (type == "CHAR")
	{
		Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
		//			cout<<"\nchar\n";		
		for (int i = 0; i < size; i++)
		{
			//				cout<<changed<<endl;
			string change(changed.begin() + 1, changed.end() - 1);
			if (outcome[i])
				ptr->update(i, change);
		}
	}
	else if (type == "DATE")
	{
		Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
		//			cout<<"\nchar\n";		
		for (int i = 0; i < size; i++)
		{
			//				cout<<changed<<endl;
			string change(changed.begin() + 1, changed.end() - 1);
			if (outcome[i])
				ptr->update(i, change);
		}
	}
	else if (type == "TIME")
	{
		Column<string>* ptr = dynamic_cast<Column<string>*>(columns[name]);
		//			cout<<"\nchar\n";		
		for (int i = 0; i < size; i++)
		{
			//				cout<<changed<<endl;
			string change(changed.begin() + 1, changed.end() - 1);
			if (outcome[i])
				ptr->update(i, change);
		}
	}
	delete[] outcome;
	return;
}

//	void Column::update(int i,string changed)
//	{
//		data[i](changed,0);
//		return;
//	}
