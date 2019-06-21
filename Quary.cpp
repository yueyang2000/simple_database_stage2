
#include <set>
#include <stack>
#include <fstream>
#include <algorithm>
#include "Group.h"
#include "calculate.h"
#include "datetime.h"
#include <regex>
void Quary::execute()
{
	parser();
	//debug();
	if (simple_mode) {
		create_column();

		get_result();
	}
	else
	{
		create_column();
		where_clause();
		get_groupby_id();
		get_result();
		sort();
	}
	//debug();
}
set<string> Quary::keywords                                             //set容器：装有SQL语句的关键字(均为大写)
{
	"CREATE","DATABASE","DATABASES","USE","WHERE","SHOW","TABLE",
	"TABLES","INT","DOUBLE","CHAR","NOT","NULL","INSERT","INTO",
	"VALUES","DELETE","FROM","UPDATE","SET","SELECT","COLUMNS",
	"DROP","AND","OR","XOR","NOT","OUTFILE","LEFT","RIGHT","ON",
	"GROUP","BY","ORDER","MAX","MIN","DISTINCT"
	"ABS","SIN","COS","FLOOR","TAN","SQRT","PI","MOD","DIV" //math functions
	"DATE","TIME"
};
Quary::Quary(string& sql)
{
	for (auto it = sql.begin(); it != sql.end(); it++)                             //遍历str的每个字符，遇到',''('')'';'就转为空格
	{
		/*if(*it=='('||*it==')'||*it==','){
			*it=' ';
		}
		else */if (*it == '\'')
		{
			*it = '\"';
		}
	}
	regex r(" ?(,) ?");
	sql = regex_replace(sql, r, " $1 ");
	stringstream s(sql);                                                   //用处理后的str初始化一个stringstream
	string temp;                                                            //暂存输入字符串
	while (s >> temp)
	{
		if (isalpha(temp[0]))                                                //判断：如果第一位是字母
		{
			string copy = temp;                                               //copy是temp的复制
			stringtoUpper(temp);
			if (keywords.find(temp) != keywords.end())                         //如果是关键字
			{
				words.push_back(temp);                                      //就把生成的全为大写的关键字放入vector<string>words中
			}
			else
			{
				words.push_back(copy);                                      //如果不是关键字，那么将原型放入vector<string>words中
			}
		}
		else
		{
			words.push_back(temp);   //如果第一位不是字母，则肯定不是关键字，将temp放入vector<string>words中
		}
	}
}
Quary::~Quary()
{
	for (auto it = result.begin(); it != result.end(); it++) {
		delete it->second;
	}
}
void Quary::debug()
{
	cout << "---DEBUG---\n";
	cout << "words:";
	for (int i = 0; i < (int)words.size(); i++)
	{
		cout << words[i] << ' ';
	}
	cout << endl << "col_name:";
	for (int i = 0; i < (int)col_name.size(); i++)
	{
		cout << col_name[i] << ' ';
	}
	cout << endl << "as:";
	for (int i = 0; i < (int)as.size(); i++) {
		if (as[i] != "") { cout << as[i] << ' '; }
		else { cout << "-" << ' '; }
	}
	cout << endl << "result:";
	for (auto it = result.begin(); it != result.end(); it++) {
		if (it->second != nullptr) {
			cout << it->first << ' ';
		}
	}
	cout << endl << "use_table:"<<local->tname;
	cout << endl << "group:";
	for (int i = 0; i < (int)group.size(); i++)
	{
		cout << group[i] << ' ';
	}
	cout << endl << "max_id:" << max_id;
	cout << endl << "row:" << row;
	cout << endl << "order by:" << order_by << endl;
	if (has_function) cout << "has function\n";
	if (filename != "") cout << "output:" << filename << endl;
	if (simple_mode) cout << "simple mode\n";
	cout << "---DEBUG END---\n";
}

void Quary::output()
{
	if (row == 0) { return; }
	streambuf* backup=nullptr;
	ofstream fout;
	if (filename != "") {
		fout.open(filename);
		//cout<<filename<<endl;
		backup = cout.rdbuf();
		cout.rdbuf(fout.rdbuf());
	}
	if(filename==""){
	for (int i = 0; i < (int)col_name.size(); i++)
	{
		if (!col_output[i]) continue;
		if (as[i] == "") { cout << col_name[i] << '\t'; }
		else { cout << as[i] << '\t'; }
	}
	cout << endl;
	}
	for (int i = 0; i < row; i++)
	{
		for (int lp = 0; lp < (int)col_name.size(); lp++)
		{
			if (!col_output[lp]) continue;
			handle_col column(result[col_name[lp]]);
			column.select(i);
			cout << '\t';
		}
		cout << endl;
	}
	if (filename != "") {
		cout.rdbuf(backup);
		fout.close();
	}
}
void Quary::parser()
{
	int pos = 1;
	while (true) {//循环读取select的列名
		if (words[pos] == "*") {
			select_all = true;
			pos++;
			break;
		}
		string cname;
		while (pos != words.size()&&words[pos] != ","&&words[pos] != "FROM"&&words[pos] != "INTO"&&words[pos] != "AS") {
			cname += words[pos];
			pos++;
		}
		//函数特判
		if (cname.find("COUNT") != string::npos ||
			cname.find("MAX") != string::npos ||
			cname.find("MIN") != string::npos) {
			if(cname.find("DISTINCT")!=string::npos){
				int tmp=cname.find("DISTINCT");
				cname.insert(tmp+8," ");
			}
			has_function = true;
		}
		else if(cname.find("ADD")!=string::npos){//ADDDATE(col,n),
			cname+=words[pos];
			pos++;
			while (pos != words.size()&&words[pos] != ","&&words[pos] != "FROM"&&words[pos] != "INTO"&&words[pos] != "AS") {
				cname += words[pos];
				pos++;
			}
		}
		col_name.push_back(cname);
		col_output.push_back(true);
		if (pos == words.size()) {
			as.push_back("");
			simple_mode = true;
			return;
		}//没有FROM
		else if (words[pos] == "AS") {
			as.push_back(words[pos + 1]);
			if (words[pos+2] == "FROM" || words[pos] == "INTO") { pos+=2;break; }
			else {//words[pos]==","
				pos+=3;
			}
		}
		else {
			as.push_back("");
			if (words[pos] == "FROM" || words[pos] == "INTO") { break; }
			else {//words[pos]==","
				pos++;
			}
		}
	}
	/*while(words[pos]!="FROM"&&words[pos]!="INTO")
	{
		if(words[pos]=="*")
		{
			select_all=true;
			pos++;
			break;
		}
		string temp=words[pos];
		if(words[pos]=="COUNT"||words[pos]=="MAX"||words[pos]=="MIN")
		{
			has_function=true;
			temp=words[pos]+"("+words[pos+1]+")";
			pos++;
		}
		if(words[pos+1]=="AS")
		{
			as.push_back(words[pos+2]);
			col_name.push_back(temp);
			col_output.push_back(true);
			pos+=3;
		}
		else
		{
			as.push_back("");
			col_name.push_back(temp);
			col_output.push_back(true);
			pos++;
		}
	}*/
	if (words[pos] == "INTO")//into outfile
	{
		filename = string(words[pos + 2].begin() + 1, words[pos + 2].end() - 1);
		pos += 3;
	}
	//这个时候读到了from
	pos++;
	local = whichdb->gettable(words[pos]);
	rnum = local->size;
	pos++;
	//知道table了，select all 时需要马上把table的所有列名插进去
	if (select_all)
	{
		for (int i = 0; i < local->order.size(); i++) {
			col_name.push_back(local->order[i]);
			col_output.push_back(true);
			as.push_back("");
		}
	}
	if (pos != words.size()&&words[pos] == "WHERE")
	{
		static set<string> logic{ "AND","OR","NOT","XOR" };
		where_begin = pos + 1;
		pos++;
		while (pos != words.size() && (keywords.count(words[pos]) == 0 || logic.count(words[pos]) != 0))
		{
			pos++;
		}
		where_end = pos;
	}
	if (pos != words.size() && words[pos] == "GROUP")
	{
		pos += 2;
		while (pos != words.size() && keywords.count(words[pos]) == 0)//不是关键词，是列名
		{
			if (words[pos] == ",") { pos++; continue; }
			group.push_back(words[pos]);
			bool found = false;
			for (int i = 0; i < col_name.size(); i++) {
				if (col_name[i] == words[pos]) { found = true; break; }
			}
			if (!found) {
				col_name.push_back(words[pos]);
				col_output.push_back(false);
			}
			pos++;
		}
	}
	if (pos != words.size() && words[pos] == "ORDER")
	{
		pos += 2;
		order_by = words[pos];
		pos++;
		/*string temp = "(" + words[pos + 1] + ")";
		if (order_by == "COUNT") {
			order_by += temp;
		}*/
		bool found = false;
		for (int i = 0; i < col_name.size(); i++) {
			if (col_name[i] == order_by) { found = true; break; }
		}
		if (!found) {
			col_name.push_back(order_by);
			col_output.push_back(false);
		}
		if(pos!=words.size()){
			if(words[pos]=="DESC"){
				ASC=false;
			}
		}
	}
	//cout << "parser succeed!\n";
}

void Quary::create_column()
{
	if (simple_mode) {
		for (int i = 0; i < (int)col_name.size(); i++) {
			colbase* temp = new Column<string>(col_name[i], 1, "CHAR");
			result[col_name[i]] = temp;
			//cout<<"create " << col_name[i]<<endl; 
		}
		return;
	}
	for (int i = 0; i < (int)col_name.size(); i++)
	{
		colbase* temp=nullptr;
		if (col_name[i].find("COUNT") != string::npos)
		{
			temp = new Column<int>(col_name[i], 1, "INT");
		}
		else if(col_name[i].find("MAX")!=string::npos ||
		col_name[i].find("MIN")!=string::npos){
			string cname(col_name[i].begin()+4,col_name[i].end()-1);
			string t=local->columns[cname]->gettype();
			if(t=="INT"){
				temp=new Column<int>(col_name[i],1,"INT");
			}
			else if(t=="DOUBLE"){
				temp=new Column<double>(col_name[i],1,"DOUBLE");
			}
			else{
				temp=new Column<string>(col_name[i],1,t);
			}
		}
		else if (local->columns.count(col_name[i]))//是列名
		{
			string t = local->typemap[col_name[i]];
			if (t == "INT")
			{
				temp = new Column<int>(col_name[i], 0, "INT");
			}
			else if (t == "CHAR")
			{
				temp = new Column<string>(col_name[i], 0, "CHAR");
			}
			else if (t == "DOUBLE")
			{
				temp = new Column<double>(col_name[i], 0, "DOUBLE");
			}
			else if (t == "DATE")
			{
				temp = new Column<string>(col_name[i], 0, "DATE");
			}
			else if (t == "TIME")
			{
				temp = new Column<string>(col_name[i], 0, "TIME");
			}
			else {}
		}
		else {//是算式
			temp = new Column<string>(col_name[i], 1, "CHAR");
		}
		result[col_name[i]] = temp;
	}
}


bool Quary::judge(string& str, int r)
{
	string op;                                  //用来读取运算符'>''<''='
	for (int i = 0; i < str.size(); i++) {      //遍历str，遇到'<'，'>'，'='后就停下
		if (str[i] == '<' || str[i] == '=' || str[i] == '>') {
			op = str[i];
			str[i] = ' ';
			break;
		}
	}
	stringstream ss(str);                       //用str初始化ss stringstream
	string name;                        //要比较的列名和数值
	ss >> name;                        //输入要比较的列名和数值
	if (local->typemap[name] == "DOUBLE") {                 //如果类型是int或double
		double value;
		ss >> value;
		if (op == ">") {                         //如果是'<'
			if (local->compare<double>(r, name, 1, value)) return true;    //小于为true，反之为false
			else return false;
		}
		else if (op == "<") {                   //如果是'='
			if (local->compare<double>(r, name, 2, value)) return true;   //等于为true，反之为false
			else return false;
		}
		else {                                  //如果是'>'
			if (local->compare<double>(r, name, 3, value)) return true;    //大于为true，反之为false
			else return false;
		}
	}
	else if (local->typemap[name] == "INT") {                                      //类型是INT
		int value;
		ss >> value;
		if (op == ">") {                         //如果是'<'
			if (local->compare<int>(r, name, 1, value)) return true;    //小于为true，反之为false
			else return false;
		}
		else if (op == "<") {                   //如果是'='
			if (local->compare<int>(r, name, 2, value)) return true;   //等于为true，反之为false
			else return false;
		}
		else {                                  //如果是'>'
			if (local->compare<int>(r, name, 3, value)) return true;    //大于为true，反之为false
			else return false;
		}
	}
	else if (local->typemap[name] == "CHAR"||local->typemap[name]=="DATE"||local->typemap[name]=="TIME") {
		string v;
		ss >> v;
		string value(v.begin() + 1, v.end() - 1);
		if (op == ">") {                         //如果是'<'
			if (local->compare<string>(r, name, 1, value)) return true;    //小于为true，反之为false
			else return false;
		}
		else if (op == "<") {                   //如果是'='
			if (local->compare<string>(r, name, 2, value)) return true;   //等于为true，反之为false
			else return false;
		}
		else {                                  //如果是'>'
			if (local->compare<string>(r, name, 3, value)) return true;    //大于为true，反之为false
			else return false;
		}
	}
	return false;
}
void Quary::where_clause()
{
	int b = where_begin;
	int e = where_end;
	if (where_begin == -1)                       //如果没有where
	{
		pick.clear();                           //那么清空pick
		for (int lp = 0; lp < rnum; lp++)              //把所有行都置为ture
		{
			pick.push_back(true);
		}
		return;
	}
	string suff;                                    //suff为转成的后缀式
	map<string, int> p = { {"NOT",3},{"AND",2},{"XOR",1},{"OR",1} };     //map p中AND对应1，OR对应0
	stack<string> s;                                //转后缀式时用到的栈s
	for (int i = b; i != e; i++) {  //从where后的第一个词开始遍历语句中的每一个词
		if (p.count(words[i])) {    //假如遇到了AND或OR
			if (s.empty()) {                        //若栈为空
				s.push(words[i]);                     //把AND或OR放入栈中
			}
			else {                                  //若栈不为空
				while (!s.empty() && p[s.top()] > p[words[i]]) {    //当栈不为空且顶部的运算优先级高于当前的运算优先级
					suff += s.top();                //把栈顶的词放入待完成的后缀式
					suff += " ";                    //加一个空格
					s.pop();                        //把栈顶词pop出来
				}
				s.push(words[i]);                     //直到栈为空或者目前的运算符优先级比栈顶大，就把当前运算符放入栈顶
			}
		}
		else {                                      //如果不是运算符
			suff += words[i];                         //就直接把该词放入待转后缀式
			suff += " ";                            //加空格
		}
	}
	while (!s.empty()) {                            //当栈不空时
		suff += s.top();                            //把所有的栈中的词放入待转后缀式中
		suff += " ";                                //加空格
		s.pop();
	}
	//cout << suff << endl;//后缀式
	pick.clear();                                   //清空原pick
	for (int i = 0; i < rnum; i++) {                //遍历所有的行
		stringstream ss(suff);                      //用转成的后缀式来初始化ss stringstream
		string temp;                                //用来读取当前的词
		stack<bool> cal;                            //
		while (ss >> temp) {                        //当ss没有结束的时候
			if (p.count(temp)) {    //若temp是运算符
				if (temp == "NOT")
				{
					bool t = cal.top(); cal.pop();
					cal.push(!t);
				}
				else
				{
					bool t2 = cal.top(); cal.pop();     //t1是cal的栈顶
					bool t1 = cal.top(); cal.pop();     //t2是cal的顶部第二个
					if (temp == "AND") {                //若读入的是AND
						if (t1&&t2) {                   //如果前两个都为true
							cal.push(true);             //则在cal中放入true
						}
						else cal.push(false);           //否则放入false
					}
					else if (temp == "OR") {                              //如果读入的是OR
						if (t1 || t2) {                 //如果t1和t2有一为true
							cal.push(true);             //在栈中放入true
						}
						else cal.push(false);           //否则放入false
					}
					else if (temp == "XOR")
					{
						if ((t1 || t2) && (!(t1&&t2))) {
							cal.push(true);
						}
						else
						{
							cal.push(false);
						}
					}
				}
			}
			else {
				cal.push(judge(temp, i));           //第i行
				//cout<<"judge "<<(int)cal.top()<<endl;
			}
		}
		bool res = cal.top(); cal.pop();            //直到后缀式读完，读取cal栈顶的变量，弹出栈顶
		pick.push_back(res);                        //把cal栈顶放入pick中，决定是否选择这一行
		//cout<<"push_back "<<(int)res<<endl;
	}
	/*for (int i = 0; i < rnum; i++) {
		if (pick[i]) cout <<"yes" << " ";
		else cout << "no" << " ";
	}
	cout << endl;*/
}
void Quary::get_groupby_id() {
	if (group.size() == 0) {//no group by
		if (has_function) {//有函数，则整体是一组
			for (int i = 0; i < rnum; i++) {
				if (pick[i]) { group_id.push_back(0); }
				else { group_id.push_back(-1); }
			}
			max_id = row = 1;
		}
		else {//没有函数，则每一行是一组
			for (int i = 0; i < rnum; i++) {
				if (pick[i]) { group_id.push_back(row); row++; }
				else { group_id.push_back(-1); }
			}
			max_id = row;
		}
		return;
	}
	vector<colbase*> compare;//group的列
	Group groupby;
	for (int i = 0; i < group.size(); i++) {
		colbase* temp = local->columns[group[i]];
		compare.push_back(temp);
		groupby.add_column(temp);
	}
	for (int i = 0; i < rnum; i++) {
		if (!pick[i]) {
			group_id.push_back(-1);
		}
		else {
			group_id.push_back(groupby.get_id(compare, i));
		}
	}
	max_id = groupby.get_num();
	row = max_id;
	/*cout<<"group_id:";
	for(int i=0;i<rnum;i++){
		cout<<group_id[i]<<' ';
	}
	cout<<endl;*/
}
void Quary::get_result()
{
	if (simple_mode) {//纯计算器模式
		for (int j = 0; j < col_name.size(); j++) {
			calculate cal(col_name[j]);
			//cout<<col_name[j]<<endl;
			string answer = cal.getresult();
			//cout<<col_name[j]<<'='<<answer<<endl;
			while (*(answer.end() - 1) == '0') { answer.erase(answer.end() - 1); }
			if (*(answer.end() - 1) == '.') { answer.erase(answer.end() - 1); }
			auto ptr = dynamic_cast<Column<string>*>(result[col_name[j]]);
			ptr->push_back(answer);
		}
		row = 1;
		return;
	}
	for (int id = 0; id < max_id; id++) {
		for (int j = 0; j < col_name.size(); j++) {
			if (local->columns.count(col_name[j])) {//是列名
				for (int i = 0; i < rnum; i++) {
					handle_col column(local->columns[col_name[j]], result[col_name[j]]);
					if (group_id[i] == id) {
						column.transform_data(i);
						break;
					}
				}
			}
			else if (col_name[j].find("COUNT") != string::npos ||
				col_name[j].find("MAX") != string::npos ||
				col_name[j].find("MIN") != string::npos)//是group函数
			{
				auto it = col_name[j].begin();
				while (*it != '(') it++;
				string cname(it + 1, col_name[j].end() - 1);
				if (col_name[j][0] == 'C') {//COUNT
					int count = 0;
					if(cname.find("DISTINCT")!=string::npos){
						string rcname(cname.begin()+8,cname.end());//重新截取的列名
						handle_col column(local->columns[rcname]);
						string t=local->columns[rcname]->gettype();
						if(t=="INT"||t=="DOUBLE"){
							set<double> s;
							for(int i=0;i<rnum;i++){
								if(group_id[i]==id&&!local->columns[rcname]->getnull(i)){
									double val=stof(column.getvalue(i));
									if(s.count(val)){
										continue;
									}
									else{
										count++;
										s.insert(val);
									}
								}
							}
						}
						else{
							set<string> s;
							for(int i=0;i<rnum;i++){
								if(group_id[i]==id&&!local->columns[rcname]->getnull(i)){
									string val=column.getvalue(i);
									if(s.count(val)){
										continue;
									}
									else{
										count++;
										s.insert(val);
									}
								}
							}
						}
					}
					else{
						if (cname == "*") {
							for (int i = 0; i < rnum; i++) {
								if (group_id[i] == id) { count++; }
							}
						}
						else {
							for (int i = 0; i < rnum; i++) {
								if (group_id[i] == id && !local->columns[cname]->getnull(i)) {
									count++;
								}
							}
						}
					}
					auto ptr = dynamic_cast<Column<int>*>(result[col_name[j]]);
					ptr->push_back(count);
				}
				else if (col_name[j][0] == 'M'&&col_name[j][1]=='A') {//MAX
					string t=local->columns[cname]->gettype();
					handle_col column(local->columns[cname]);

					if(t=="INT"){
						int max=-99999999;
						for(int i=0;i<rnum;i++){
							if(group_id[i]==id &&!local->columns[cname]->getnull(i)&& max<stoi(column.getvalue(i))){
								max=stoi(column.getvalue(i));
							}
						}
						auto ptr = dynamic_cast<Column<int>*>(result[col_name[j]]);
						ptr->push_back(max);
					}
					else if(t=="DOUBLE"){
						double max=-99999999;
						for(int i=0;i<rnum;i++){
							if(group_id[i]==id && !local->columns[cname]->getnull(i)&&max<stof(column.getvalue(i))){
								max=stof(column.getvalue(i));
							}
						}
						auto ptr = dynamic_cast<Column<double>*>(result[col_name[j]]);
						ptr->push_back(max);
					}
					else{
						string max="";
						for(int i=0;i<rnum;i++){
							if(group_id[i]==id && !local->columns[cname]->getnull(i)&&max<column.getvalue(i)){
								max=column.getvalue(i);
							}
						}
						auto ptr = dynamic_cast<Column<string>*>(result[col_name[j]]);
						ptr->push_back(max);						
					}
				}
				else if (col_name[j][0] == 'M'&&col_name[j][1]=='I') {//MIN
					string t=local->columns[cname]->gettype();
					handle_col column(local->columns[cname]);
					auto ptr = dynamic_cast<Column<string>*>(result[col_name[j]]);
					if(t=="INT"){
						int min=2147483647;
						for(int i=0;i<rnum;i++){
							if(group_id[i]==id &&!local->columns[cname]->getnull(i)&& min>stoi(column.getvalue(i))){
								min=stoi(column.getvalue(i));
							}
						}
						auto ptr = dynamic_cast<Column<int>*>(result[col_name[j]]);
						ptr->push_back(min);
					}
					else if(t=="DOUBLE"){
						double min=99999999;
						for(int i=0;i<rnum;i++){
							if(group_id[i]==id && !local->columns[cname]->getnull(i)&&min>stof(column.getvalue(i))){
								min=stof(column.getvalue(i));
							}
						}
						auto ptr = dynamic_cast<Column<double>*>(result[col_name[j]]);
						ptr->push_back(min);
					}
					else{
						string min=column.getvalue(0);
						for(int i=0;i<rnum;i++){
							if(group_id[i]==id && !local->columns[cname]->getnull(i)&&min>column.getvalue(i)){
								min=column.getvalue(i);
							}
						}
						auto ptr = dynamic_cast<Column<string>*>(result[col_name[j]]);
						ptr->push_back(min);						
					}
				}
			}
			else {//是算式
				for (int i = 0; i < rnum; i++) {
					if (group_id[i] == id) {
						string formula(col_name[j]);
						for (int k = 0; k < local->order.size(); k++) {//列名循环
							string oldval = local->order[k];//列名
							handle_col column(local->columns[local->order[k]]);
							string newval = column.getvalue(i);//列名替换为value
							size_t index = formula.find(oldval);
							while (index != formula.npos)
							{
								formula.erase(index, oldval.size());
								formula.insert(index, newval);
								index = formula.find(oldval);
							}
						}
						string answer;
						//if (formula == col_name[j]) { row = max_id = 1; }//没有列名
						if(formula=="CURTIME()"){
							answer=CurTime();
						}
						else if(formula=="CURDATE"){
							answer=CurDate();
						}
						else if(formula.find("ADDDATE")!=string::npos){
							int pos=formula.find(",");
							string val1(formula.begin()+8,formula.begin()+pos);
							string val2(formula.begin()+pos+1,formula.end()-1);
							if(val1=="CURDATE()"){
								val1=CurDate();
							}
							calculate cal(val2);
							val2=cal.getresult();
							string tmp=val1+" "+val2;
							answer=adddate(tmp);
						}
						else if(formula.find("ADDTIME")!=string::npos){
							int pos=formula.find(",");
							string val1(formula.begin()+8,formula.begin()+pos);
							string val2(formula.begin()+pos+1,formula.end()-1);
							if(val1=="CURDATE()"){
								val1=CurTime();
							}
							calculate cal(val2);
							val2=cal.getresult();
							string tmp=val1+" "+val2;
							answer=addtime(tmp);
						}
						else{
						calculate cal(formula);
						answer = cal.getresult();
						}
						//cout<<formula<<'='<<answer<<endl;
						auto ptr = dynamic_cast<Column<string>*>(result[col_name[j]]);
						ptr->push_back(answer);
						break;
					}
				}
			}
		}
	}
}
void Quary::sort() {
	if (order_by == "" || simple_mode) { return; }
	handle_col column(result[order_by], result[order_by]);
	for (int j = column.size() - 1; j > 0; j--) {
		for (int i = 0; i < j; i++) {
			if (column.compare(i, i + 1, 1)) {
				if(ASC){swap_row(i, i + 1);}
				//cout<<"swap"<<i<<' '<<i+1<<endl;
			}
			else {
				if(!ASC){swap_row(i,i+1);}
			}
		}
	}
}
void Quary::swap_row(int i, int j) {
	for (int lp = 0; lp < col_name.size(); lp++) {
		handle_col column(result[col_name[lp]]);
		column.swap(i, j);
	}
}
