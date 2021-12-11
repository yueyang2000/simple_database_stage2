
#pragma once
#include<string>
#include<sstream>
#include<map>
#include<set>
#include<vector>
using namespace std;

class calculate {
private:
	string expression;
	string suff;
	vector<string> ex;
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
	void setvec();
	void transition();
	void form();//ÕýÔò±í´ïÊ½
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