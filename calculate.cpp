#include"pch.h"
#include"calculate.h"
#include<iostream>
#include<cmath>
#include<stack>
#include <regex>
#include<cstdlib>
using namespace std;
set<string> calculate::fuc = { "ABS(","SIN(","COS(","FLOOR(","PI(","TAN(","SQRT(","(" };
map<string, int> calculate::p = { {"+",4},{"-",4},{"*",5},{"/",5},{"%",5},{"DIV",5},{"MOD",5}, {"(",0},{"ABS(",0},{"SIN(",0},{"COS(",0},{"FLOOR(",0},{"PI(",0}
,{"TAN(",0},{"SQRT(",0},{")",0} ,{"OR",1},{"XOR",1},{"AND",2},{"NOT",3},{"m+",6},{"m-",6} };
void calculate::transition() {
	stack<string>s;
	string temp;
	for(int i=0;i<(int)ex.size();i++) {
		temp = ex[i];
		if (isnumber(temp)) {
			suff += temp;
			suff += " ";
		}
		else {
			if (isfuc(temp)) {
				s.push(temp);
			}
			else {//如果是普通的运算符或）
				if (temp == ")") {//如果是）
					while (!isfuc(s.top())) {
						suff += s.top();
						suff += " ";
						s.pop();
					}
					if (s.top() != "(") {
						suff += s.top();
						suff += " ";
						s.pop();
					}
					else {
						s.pop();
					}
				}
				else {//加减乘除乘除运算符
					if (temp == "+" || temp == "-") {
						if (i == 0 || !isnumber(ex[i - 1])) {
							s.push("m" + temp);
							continue;
						}
					}
					if (s.empty()) {
						s.push(temp);
					}
					else {
						while (!s.empty() && p[s.top()] > p[temp]) {
							suff += s.top();
							suff += " ";
							s.pop();
						}
						s.push(temp);
					}
				}
			}
		}
	}
	while (!s.empty()) {
		suff += s.top();
		suff += " ";
		s.pop();
	}
	//cout << suff << endl;
}
void calculate::setvec() {
	stringstream ss(expression);
	string temp;
	while (ss >> temp) {
		ex.push_back(temp);
	}
	/*for (int i = 0; i < (int)ex.size(); i++) {
		cout << ex[i] << " ";
	}
	cout << endl;*/
}
string calculate::getresult() {
	form();
	setvec();
	transition();
	stringstream ss(suff);
	stack<double> s;
	string piece;
	while (ss >> piece) {
		if (isnumber(piece)) {//如果是数字直接入栈
			s.push(atof(piece.c_str()));
			continue;
		}
		if (p.count(piece) == 0) {
			return "NULL";
		}
		if (piece == "PI(") {
			s.push(3.14159265358979323846);
			continue;
		}
			if (piece != "NOT"&&!isfuc(piece)&&piece!="m+"&&piece!="m-") {//二元运算符
				double t2 = s.top(); s.pop();
				double t1 = s.top(); s.pop();
				if (piece == "+") {
					s.push(t1 + t2);
				}
				else if (piece == "-") {
					s.push(t1 - t2);
				}
				else if (piece == "*") {
					s.push(t1*t2);
				}
				else if (piece == "/"||piece=="DIV") {
					if (t2 == 0) {
						return "NULL";
					}
					else {
						s.push(t1 / t2);
					}
				}
				else if (piece == "%" || piece == "MOD") {
					if (t2 == 0) {
						return "NULL";
					}
					else {
						s.push((int)t1 % (int)t2);
					}
				}
				else if (piece == "OR") s.push((double)(t1 || t2));
				else if (piece == "XOR") {
					if ((!t1&&t2) || (t1 && !t2)) { s.push(1.0); }
					else s.push(0.0);
				}
				else if (piece == "AND") s.push(t1&&t2);
			}
			else {//一元运算符
				double t1 = s.top();
				s.pop();
				if (piece == "m+") s.push(t1);
				else if (piece == "m-") s.push(-t1);
				else if (piece == "NOT") {
					s.push(!t1);
				}
				else if (piece == "ABS(") s.push(abs(t1));
				else if (piece == "SIN(") s.push(sin(t1));
				else if (piece == "COS(") s.push(cos(t1));
				else if (piece == "TAN(") s.push(tan(t1));
				else if (piece == "EXP(") s.push(exp(t1));
				else if (piece == "SQRT(") s.push(sqrt(t1));
				else if (piece == "FLOOR(") s.push(floor(t1));
			}
		
	}
	double res = s.top();
	s.pop();
	return to_string(res);
}
void calculate::form() {
	regex r(" ?(\\+|-|\\*|/|%| (DIV) | (MOD) |\\(|\\)) ?");
	expression = regex_replace(expression, r, " $1 ");
	r = " ?( (OR)| (XOR)|(AND)|(NOT)) ?";
	expression = regex_replace(expression, r, " $1 ");
	r = " ?((ABS) | (SIN) | (EXP) | (COS) | (TAN) | (SQRT) | (FLOOR)|(PI)) ?";
	expression = regex_replace(expression, r, " $1 ");
	
	r = "(^ +)|( +$)";
	expression = regex_replace(expression, r, "");
	r = " +";
	expression = regex_replace(expression, r, " ");
	r = "ABS *\\(";
	expression = regex_replace(expression, r, "ABS(");
	r = "SIN *\\(";
	expression = regex_replace(expression, r, "SIN(");
	r = "COS *\\(";
	expression = regex_replace(expression, r, "COS(");
	r = "PI *\\(";
	expression = regex_replace(expression, r, "PI(");
	r = "EXP *\\(";
	expression = regex_replace(expression, r, "EXP(");
	r = "TAN *\\(";
	expression = regex_replace(expression, r, "TAN(");
	r = "SQRT *\\(";
	expression = regex_replace(expression, r, "SQRT(");
	r = "FLOOR *\\(";
	expression = regex_replace(expression, r, "FLOOR(");
	//cout << expression << endl;
}