#pragma once
#include<string>
#include<sstream>
#include<map>
#include<set>
using namespace std;

class calculate {
private:
	string expression;
	string suff;
	static map<string, int> p;
	static set<string> fuc;
public:
	calculate() {}
	calculate(string &expression) {
		for (int i = 0; i < expression.length(); i++) {
			if (expression[i] >= 'a'&&expression[i] <= 'z') {
				expression[i] -= 32;
			}
		}
		this->expression = expression;
	}
		void transition();
		void form();//正则表达式
		bool isfuc(string &str) {
			if (fuc.count(str) != 0) return true;
			return false;
		}
		bool isnumber(string &str) {
			if (p.count(str) == 0) return true;
			return false;
		}
		string getresult();
};