
#include "Table.h"

void stringtoUpper(string& x);

int countor(const std::string& str, const std::string& sub)//查找whereclause中or（各种书写方式）的数量的函数
{
	int num = 0;
	size_t len = sub.length();
	if (len == 0)len = 1;
	for (size_t i = 0; (i = str.find(sub, i)) != std::string::npos; num++, i += len);
	return num;
}

int* whereclause(Table* table) {
	//⾸先根据根据语句之中的OR将语句分割成多个⼦语句，每个子语句又分成⼀个或多个单句 (变量名-符号-值)。对于所有行遍历这些⼦句，记录求得的布尔值，最终返回所有符合 Whereclause 语句的行号。
	int oor1 = countor(operation, "OR");
	int oor2 = countor(operation, "or");
	int oor3 = countor(operation, "oR");
	int oor4 = countor(operation, "Or");
	vector<string>* pointer = new vector<string>[oor1 + oor2 + oor3 + oor4 + 1];//存条件句
	int n = 0;
	string sentence;
	string cmp;
	while (ss >> sentence) {
		cmp = sentence;
		stringtoUpper(cmp);
		if (cmp == "OR") {
			n++;
			continue;
		}
		else if (cmp == "AND") {
			continue;
		}
		else {
			pointer[n].push_back(sentence);
		}
	}
	int** choose = new int*[n + 1];
	for (int i = 0; i < n + 1; i++) {
		choose[i] = new int[table->getsize()];
	}
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < table->getsize(); j++) {
			choose[i][j] = 1;
		}//全部预置为1，之后先进行与运算，之后choose[i]间进行或运算
	}
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < pointer[i].size(); j++) {
			string tmp = pointer[i][j];
			if (count(tmp.begin(), tmp.end(), '>')) {
				unsigned long oprt = tmp.find('>');
				string attr(tmp, 0, oprt);
				string value(tmp, oprt + 1);
				//                cout<<attr<<'>'<<value<<' ';
				//                stringtoUpper(attr);
				string type = table->showtype(attr);
				//                cout<<type<<endl;
				if (type == "INT") {
					int cmpval = stoi(value);
					table->cmpval<int>(cmpval, attr, choose[i], 1);
				}
				else if (type == "DOUBLE") {
					double cmpval = stod(value);
					table->cmpval<double>(cmpval, attr, choose[i], 1);
				}
				else if (type == "CHAR") {
					string cmpval(value.begin() + 1, value.end() - 1);
					table->cmpval<string>(cmpval, attr, choose[i], 1);
				}
				else if (type == "DATE") {
					string cmpval(value.begin() + 1, value.end() - 1);
					table->cmpval<string>(cmpval, attr, choose[i], 1);
				}
				else if (type == "TIME") {
					string cmpval(value.begin() + 1, value.end() - 1);
					table->cmpval<string>(cmpval, attr, choose[i], 1);
				}
			}
			else if (count(tmp.begin(), tmp.end(), '<')) {
				unsigned long oprt = tmp.find('<');
				string attr(tmp, 0, oprt);
				string value(tmp, oprt + 1);
				//                cout<<attr<<'<'<<value<<' ';
				//                stringtoUpper(attr);
				string type = table->showtype(attr);
				//                cout<<type<<endl;
				if (type == "INT") {
					int cmpval = stoi(value);
					table->cmpval<int>(cmpval, attr, choose[i], 2);
				}
				else if (type == "DOUBLE") {
					double cmpval = stod(value);
					table->cmpval<double>(cmpval, attr, choose[i], 2);
				}
				else if (type == "CHAR") {
					string cmpval(value.begin() + 1, value.end() - 1);
					table->cmpval<string>(cmpval, attr, choose[i], 2);
				}
				else if (type == "DATE") {
					string cmpval(value.begin() + 1, value.end() - 1);
					table->cmpval<string>(cmpval, attr, choose[i], 2);
				}
				else if (type == "TIME") {
					string cmpval(value.begin() + 1, value.end() - 1);
					table->cmpval<string>(cmpval, attr, choose[i], 2);
				}
			}
			else if (count(tmp.begin(), tmp.end(), '=')) {
				unsigned long oprt = tmp.find('=');
				string attr(tmp, 0, oprt);
				string value(tmp, oprt + 1);
				//                cout<<attr<<'='<<value<<' ';
				//                stringtoUpper(attr);
				string type = table->showtype(attr);
				//                cout<<type<<endl;
				if (type == "INT") {
					int cmpval = stoi(value);
					table->cmpval<int>(cmpval, attr, choose[i], 3);
				}
				else if (type == "DOUBLE") {
					double cmpval = stod(value);
					table->cmpval<double>(cmpval, attr, choose[i], 3);
				}
				else if (type == "CHAR") {
					string cmpval(value.begin() + 1, value.end() - 1);
					//                    cout<<cmpval<<endl;
					table->cmpval<string>(cmpval, attr, choose[i], 3);
				}
				else if (type == "DATE") {
					string cmpval(value.begin() + 1, value.end() - 1);
					//                    cout<<cmpval<<endl;
					table->cmpval<string>(cmpval, attr, choose[i], 3);
				}
				else if (type == "TIME") {
					string cmpval(value.begin() + 1, value.end() - 1);
					//                    cout<<cmpval<<endl;
					table->cmpval<string>(cmpval, attr, choose[i], 3);
				}
			}
		}
	}
	int* outcome = new int[table->getsize()];
	for (int i = 0; i < table->getsize(); i++) {
		outcome[i] = 0;
	}
	for (int i = 0; i < table->getsize(); i++) {
		for (int j = 0; j < n + 1; j++) {
			if (choose[j][i] == 1) {
				outcome[i] = 1;
				break;
			}
		}
	}
	for (int i = 0; i < n + 1; i++) {
		delete[] choose[i];
	}
	//    for(int i=0;i<table->getsize();i++){
	//        cout<<outcome[i];
	//    }
	//    cout<<endl;
	delete[] choose;
	delete[] pointer;
	return outcome;//记得delete
}
