#include "Quary.h"
#include <set>
#include <stack>
#include <fstream>
#include "Group.h"
void Quary::execute()
{
    parser();
    if(use_table.size()==1)
    {
        simple_create_column();
        simple_where_clause();
        if(group.size()==0){
            simple_insert();
        }
        else{
            get_groupby_id();
            //group_insert();
        }
    }
    //output();
    debug();
}
set<string> Quary::keywords                                             //set容器：装有SQL语句的关键字(均为大写)
    {
        "CREATE","DATABASE","DATABASES","USE","WHERE","SHOW","TABLE",
        "TABLES","INT","DOUBLE","CHAR","NOT","NULL","INSERT","INTO",
        "VALUES","DELETE","FROM","UPDATE","SET","SELECT","COLUMNS",
        "DROP","AND","OR","XOR","NOT","OUTFILE","LEFT","RIGHT","ON",
        "GROUP","BY","ORDER"
    };
Quary::Quary(string& sql)
{
    for(auto it=sql.begin();it!=sql.end();it++)                             //遍历str的每个字符，遇到',''('')'';'就转为空格
    {
        if(*it==','||*it=='('||*it==')'||*it==';')
        {
            *it=' ';
        }
        else if(*it=='\'')
        {
            *it='\"';
        }
    }
    stringstream s(sql);                                                   //用处理后的str初始化一个stringstream
    string temp;                                                            //暂存输入字符串
    while(s>>temp)
    {
        if(isalpha(temp[0]))                                                //判断：如果第一位是字母
        {
            string copy=temp;                                               //copy是temp的复制
            stringtoUpper(temp);
            if(keywords.find(temp)!=keywords.end())                         //如果是关键字
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
    for(auto it=result.begin();it!=result.end();it++){
        delete it->second;
    }
}
void Quary::debug()
{
    cout<<"---DEBUG---\n";
    cout<<"words:";
    for(int i=0;i<(int)words.size();i++)
    {
        cout<<words[i]<<' ';
    }
    cout<<endl<<"col_name:";
    for(int i=0;i<(int)col_name.size();i++)
    {
        cout<<col_name[i]<<' ';
    }
    cout<<endl<<"result:";
    for(auto it=result.begin();it!=result.end();it++){
        if(it->second!=nullptr){
            cout<<it->first<<' ';
        }
    }
    cout<<endl<<"use_table:";
    for(auto it=use_table.begin();it!=use_table.end();it++){
        if(it->second!=nullptr){
            cout<<it->first<<' ';
        }
    }
    cout<<endl<<"group:";
    for(int i=0;i<(int)group.size();i++)
    {
        cout<<group[i]<<' ';
    }
    cout<<endl<<"order by:"<<order_by<<endl;
    if(has_function) cout<<"has function\n";
    if(filename!="") cout<<"output:"<<filename<<endl;
    cout<<"---DEBUG END---\n";
}

void Quary::output()
{
    streambuf* backup;
    ofstream fout;
    if(filename!=""){
        fout.open(filename);
        //cout<<filename<<endl;
        backup=cout.rdbuf();
        cout.rdbuf(fout.rdbuf());
    }
    for(int i=0;i<(int)col_name.size();i++)
    {
        cout<<col_name[i]<<'\t';
    }
    cout<<endl;
    for(int i=0;i<row;i++)
    {
        for(int lp=0;lp<(int)col_name.size();lp++)
        {
            colbase* ptr=result[col_name[lp]];
            if(ptr->gettype()=="INT"){
                Column<int>* ptr1=dynamic_cast<Column<int>*> (ptr);
                ptr1->select(i);
            }
            else if(ptr->gettype()=="DOUBLE"){
                Column<double>* ptr1=dynamic_cast<Column<double>*> (ptr);
                ptr1->select(i);
            }
            else if(ptr->gettype()=="CHAR"){
                Column<string>* ptr1=dynamic_cast<Column<string>*> (ptr);
                ptr1->select(i);
            }
            cout<<'\t';
        }
        cout<<endl;
    }
    if(filename!=""){
        cout.rdbuf(backup);
        fout.close();
    }
}
void Quary::parser()
{
    int pos=1;
        while(words[pos]!="FROM"&&words[pos]!="INTO")
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
                pos+=3;
            }
            else
            {
                as.push_back(temp);
                col_name.push_back(temp);
                pos++;
            }
        }
        if(words[pos]=="INTO")
        {
            filename=string(words[pos+2].begin()+1,words[pos+2].end()-1);
            pos+=3;
        }
        //这个时候读到了from
        pos++;
        while(words[pos]!="WHERE"&&words[pos]!="GROUP"&&words[pos]!="ORDER")
        {
            use_table[words[pos]]=whichdb->gettable(words[pos]);
            //cout<<words[pos]<<endl;
            pos++;
            if(pos==words.size())
                break;
        }
        if(words[pos]=="WHERE")
        {
            static set<string> logic{"AND","OR","NOT","XOR"};
            where_begin=pos+1;
            pos++;
            while(keywords.count(words[pos])==0||logic.count(words[pos])!=0)
            {
                pos++;
                if(pos==words.size())
                {
                    break;
                }
            }
            where_end=pos;
        }
        if(words[pos]=="GROUP")
        {
            pos+=2;
            while(keywords.count(words[pos])==0)
            {
                group.push_back(words[pos]);
                pos++;
                if(pos==words.size()){
                    break;
                }
            }
        }
        if(words[pos]=="ORDER")
        {
            pos+=2;
            order_by=words[pos];
        }
    cout<<"parser succeed!\n";
}

void Quary::simple_create_column()
{
    Table* local=use_table.begin()->second;
    if(select_all)
    {

        for(int i=0;i<local->order.size();i++)
        {
            col_name.push_back(local->order[i]);
        }
    }
    for(int i=0;i<(int)col_name.size();i++)
    {
        colbase* temp;
        if(col_name[i].find("COUNT")!=string::npos)
        {
            temp=new Column<int>(col_name[i],1,"INT");
        }
        else
        {
            string t=local->typemap[col_name[i]];
            if(t=="INT")
            {
                temp=new Column<int>(col_name[i],0,"INT");
            }
            else if(t=="CHAR")
            {
                temp=new Column<string>(col_name[i],0,"CHAR");
            }
            else if(t=="DOUBLE")
            {
                temp=new Column<double>(col_name[i],0,"DOUBLE");
            }
            else{}
        }
        result[col_name[i]]=temp;
    }
}

void Quary::simple_insert()
{
    int rnum=use_table.begin()->second->size;
    //cout<<use_table.begin()->second->tname<<endl;
    Table* local=use_table.begin()->second;
    if(group.size()==0)//没有group by
    {
        if(has_function)
        {
            row=1;
            for(int lp=0;lp<(int)col_name.size();lp++)
            {
                auto it=col_name[lp].begin();
                while(*it!='(') it++;
                string cname(it+1,col_name[lp].end()-1);
                if(col_name[lp][0]=='C')
                {
                    int count=0;
                    if(cname=="*"){
                        for(int i=0;i<rnum;i++)
                            if(pick[i]) count++;
                    }
                    else{
                        for(int i=0;i<rnum;i++)
                        {
                            if(pick[i]&&!local->columns[cname]->getnull(i))
                                count++;
                        }
                    }
                    //cout<<result[col_name[lp]]->gettype()<<endl;
                    Column<int> * ptr=dynamic_cast<Column<int>*> (result[col_name[lp]]);
                    ptr->push_back(count);
                }
                else if(col_name[lp][0]=='M'){}//以后再写MAX和MIN
            }
        }
        else//没有函数
        {
            row=0;
            for(int i=0;i<col_name.size();i++)
            {
                colbase* ptr=result[col_name[i]];
                if(ptr->gettype()=="INT"){
                    Column<int>* from=dynamic_cast<Column<int>*> (local->columns[col_name[i]]);
                    Column<int> * to=dynamic_cast<Column<int>*> (result[col_name[i]]);
                    for(int j=0;j<rnum;j++){
                        if(pick[j])
                        {
                            to->data.push_back(from->getdata(j));row++;
                        }
                    }
                }
                else if(ptr->gettype()=="DOUBLE"){
                    Column<double>* from=dynamic_cast<Column<double>*> (local->columns[col_name[i]]);
                    Column<double> * to=dynamic_cast<Column<double>*> (result[col_name[i]]);
                    for(int j=0;j<rnum;j++){
                        if(pick[j])
                        {
                            to->data.push_back(from->getdata(j));row++;
                        }
                    }
                }
                else if(ptr->gettype()=="CHAR"){
                    Column<string>* from=dynamic_cast<Column<string>*> (local->columns[col_name[i]]);
                    Column<string> * to=dynamic_cast<Column<string>*> (result[col_name[i]]);
                    for(int j=0;j<rnum;j++){
                        if(pick[j])
                        {
                            to->data.push_back(from->getdata(j));row++;
                        }
                    }
                }
            }
            row=row/(int)col_name.size();//最后算出行数
        }
    }
    else
    {
        group_insert();
    }
}
bool Quary::simple_judge(string& str,int r)
{
    Table* local=use_table.begin()->second;
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
    if (local->typemap[name]== "DOUBLE") {                 //如果类型是int或double
        double value;
        ss>>value;
        if (op ==">") {                         //如果是'<'
            if (local->compare<double>(r,name,1,value)) return true;    //小于为true，反之为false
            else return false;
        }
        else if (op == "<") {                   //如果是'='
            if  (local->compare<double>(r,name,2,value)) return true;   //等于为true，反之为false
            else return false;
        }
        else {                                  //如果是'>'
            if (local->compare<double>(r,name,3,value)) return true;    //大于为true，反之为false
            else return false;
        }
    }
    else if(local->typemap[name]=="INT"){                                      //类型是INT
        int value;
        ss>>value;
        if (op ==">") {                         //如果是'<'
            if (local->compare<int>(r,name,1,value)) return true;    //小于为true，反之为false
            else return false;
        }
        else if (op == "<") {                   //如果是'='
            if  (local->compare<int>(r,name,2,value)) return true;   //等于为true，反之为false
            else return false;
        }
        else {                                  //如果是'>'
            if (local->compare<int>(r,name,3,value)) return true;    //大于为true，反之为false
            else return false;
        }
    }
    else if(local->typemap[name]=="CHAR"){
        string v;
        ss>>v;
        string value(v.begin()+1,v.end()-1);
        if (op ==">") {                         //如果是'<'
            if (local->compare<string>(r,name,1,value)) return true;    //小于为true，反之为false
            else return false;
        }
        else if (op == "<") {                   //如果是'='
            if  (local->compare<string>(r,name,2,value)) return true;   //等于为true，反之为false
            else return false;
        }
        else {                                  //如果是'>'
            if (local->compare<string>(r,name,3,value)) return true;    //大于为true，反之为false
            else return false;
        }
    }
    return false;
}
void Quary::simple_where_clause()
{
    int rnum=use_table.begin()->second->size;
    int b=where_begin;
    int e=where_end;
    if(where_begin==-1)                       //如果没有where
    {
        pick.clear();                           //那么清空pick
        for(int lp=0;lp<rnum;lp++)              //把所有行都置为ture
        {
            pick.push_back(true);
        }
        return;
    }
    string suff;                                    //suff为转成的后缀式
    map<string, int> p = { {"NOT",3},{"AND",2},{"XOR",1},{"OR",0} };     //map p中AND对应1，OR对应0
    stack<string> s;                                //转后缀式时用到的栈s
    for (int i = b; i!=e; i++) {  //从where后的第一个词开始遍历语句中的每一个词
        if (p.count(words[i])) {    //假如遇到了AND或OR
            if (s.empty()) {                        //若栈为空
                s.push(words[i]);                     //把AND或OR放入栈中
            }
            else {                                  //若栈不为空
                while (!s.empty()&&p[s.top()] > p[words[i]]) {    //当栈不为空且顶部的运算优先级高于当前的运算优先级
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
    cout << suff << endl;//后缀式
    pick.clear();                                   //清空原pick
    for (int i = 0; i < rnum; i++) {                //遍历所有的行
        stringstream ss(suff);                      //用转成的后缀式来初始化ss stringstream
        string temp;                                //用来读取当前的词
        stack<bool> cal;                            //
        while (ss >> temp) {                        //当ss没有结束的时候
            if (p.count(temp)) {    //若temp是运算符
                if(temp=="NOT")
                {
                    bool t=cal.top();cal.pop();
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
                    else if(temp=="OR") {                              //如果读入的是OR
                        if (t1 || t2) {                 //如果t1和t2有一为true
                            cal.push(true);             //在栈中放入true
                        }
                        else cal.push(false);           //否则放入false
                    }
                    else if(temp=="XOR")
                    {
                        if((t1||t2)&&(!(t1&&t2))){
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
                cal.push(simple_judge(temp, i));           //第i行
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
void Quary::get_groupby_id(){
    int rnum=use_table.begin()->second->size;
    Table* local=use_table.begin()->second; 
    vector<colbase*> compare;//group的列
    Group groupby;
    for(int i=0;i<group.size();i++){
        colbase* temp=local->columns[group[i]];
        compare.push_back(temp);
        groupby.add_column(temp);
    }
    for(int i=0;i<rnum;i++){
        if(!pick[i]){
            group_id.push_back(-1);
        }
        else{
            group_id.push_back(groupby.get_id(compare,i));
        }
    }
    cout<<"group_id:";
    for(int i=0;i<rnum;i++){
        cout<<group_id[i]<<' ';
    }
    cout<<endl;
}
void Quary::group_insert()
{

}
