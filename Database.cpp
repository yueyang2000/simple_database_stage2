
#include "Quary.h"

void stringtoUpper(string& op){//实现字符串转大写
    for(int i=0;i<op.length();i++){
        if(op[i]>='a'&&op[i]<='z'){
            op[i]-=32;
        }
    }
}


bool Sign(){//模仿mysql的指示符
    cout<<"MySQL>";
    return true;
}//new


void Database::showtable(string &x) {
    string output=x;
//    stringtoUpper(x);
    map<string,Table*>::iterator it;
    if(tables.count(x)){
        it=tables.find(x);
        it->second->showinfo();
    }
    else{
        cout<<"There is no table named "<<output<<" in this database."<<endl;
    }
}

void Database::createtable(string name){
    Table* newtable=new Table(name);
//    stringtoUpper(name);
    whichdb->tables[name]=newtable;
//    cout<<"now there are "<<tables.size()<<" table(s) in this database"<<endl;
}//new

Database::Database(string& x):dname(x){}

ostream& operator <<(ostream& out, Database& t){//use database的输出
    out<<"the database being used is "<<t.dname<<endl;
    return out;
}

bool Database::compare(string x){
    if(dname==x)return true;
    return false;
}


void Database::showname() {
    cout<<"Tables_in_"<<dname<<endl;
    if(!tables.empty()) {
        map<string, Table *>::iterator it;
        it = tables.begin();
        //        bool empty = true;
        while (it != tables.end()) {
            it->second->showname();
            it++;
            //            empty = false;
        }
    }
//    else cout<<"Empty"<<endl;
}

void Database::showdb(){
    cout<<dname<<endl;
}

void Database::droptable() {//此处注意删除指针
    string k;
    ss>>k;
    string output=k;

    if(tables.count(k)){
        delete tables.find(k)->second;
        tables.erase(k);
    }
    else cout<<"no table "<<output<<" found"<<endl;
}

void createdatabase(){
    string name;
    ss>>name;
    Database* tmp=new Database(name);
    database[name]=tmp;}
    

void Database::inserttable(){
    string tablename;
    ss>>tablename;
//    stringtoUpper(tablename);
    if(tables.count(tablename)){
        Table* whichtb=tables[tablename];
        whichtb->inserttocolumn();
    }
    else{
        cout<<"no table named "<<tablename<<endl;
        return;
    }
}

void dropdatabase(){
    string x;
    ss>>x;
    string t=x;
//    stringtoUpper(x);
    bool found=false;
    if(database.count(x)){
//            cout<<"database "<<t<<" dropped"<<endl;
        if(whichdb==database[x]){
            whichdb=NULL;
        }
        delete database[x];
        database.erase(x);
        found=true;
    }
    if(!found)cout<<"no database "<<t<<" found"<<endl;
}//new

void usedatabase(){
    //    cout<<"USE"<<endl;
    bool found=false;
    string x;
    ss>>x;
//    stringtoUpper(x);
    if(database.count(x)){
        whichdb=database[x];
        found=true;
    }
    if(!found)cout<<"no database "<<x<<" found"<<endl;
    //    else cout<<*whichdb;
}

bool Process_operation(string& operation){//将输⼊规范化，即去掉所有的括号和逗号，各个命令成分之间用空格相隔，使得接下来可以⽅便的使用字符串流来逐个部分地解析。
    while(operation.find(',')!=string::npos){
        operation[operation.find(',')]=' ';
    }
    while(operation.find('(')!=string::npos){
        operation[operation.find('(')]=' ';
    }
    while(operation.find(')')!=string::npos){
        operation[operation.find(')')]=' ';
    }
    //    cout<<operation<<endl;
    operation.erase(operation.end()-1);//get rid of ;
    //    cout<<operation<<endl;

    ss.clear();
    ss<<operation;
    //    cout<<ss.str()<<endl;
    string op;
    ss>>op;
    stringtoUpper(op);
    //    cout<<op<<endl;
    if(op=="SELECT")
    {
        Quary quary(operation);
        quary.execute();
        while(ss>>op){}
        return false;
    }
    if(op=="CREATE"){
        ss>>op;
        stringtoUpper(op);
        if(op=="DATABASE"){
            createdatabase();
            ss.str("");
            return false;
        }
        else if(op=="TABLE"){
            if(whichdb==NULL){cout<<"please use a database first."<<endl;}
            else {
                string name;
                ss>>name;
                whichdb->createtable(name);
            }
            ss.str("");
            return false;
        }
    }
    else if(op=="INSERT"){
            if(whichdb==NULL){cout<<"please use a database first."<<endl;}
            else {
                string tmp;
                ss>>tmp;
                //剩下的部分从tablename开始，到values之前是第一个子串，value之后是第二个子串。
                whichdb->inserttable();
             }
    }
    else if(op=="DROP"){
        ss>>op;
        stringtoUpper(op);
        if(op=="DATABASE"){
            dropdatabase();
            if(database.empty())return true;
            ss.str("");
            return false;
        }
        else if(op=="TABLE"){
            if(whichdb==NULL){cout<<"Alert: please use a database first."<<endl;}
            else whichdb->droptable();
            ss.str("");
            return false;
        }
    }
    else if(op=="USE"){
        usedatabase();
        ss.str("");
        return false;
    }
    else if(op=="WHERE"){
        string which;
        ss>>which;
        stringtoUpper(which);
        if(whichdb->tableexist(which)) {
            int *output = whereclause(whichdb->gettable(which));
            delete[] output;
        }
        else cout<<"no table named "<<which<<" exists"<<endl;
        ss.str("");
        return false;
    }
    else if(op=="SHOW"){
        ss>>op;
        stringtoUpper(op);
        if(op=="DATABASES"){
            cout<<"Database"<<endl;
            map<string,Database*>::iterator it;
            it = database.begin();
            while(it != database.end())
            {
                it->second->showdb();
                it ++;
            }
            //            cout<<"In total: "<<database.size()<<endl;
        }
        else if(op=="COLUMNS"){
            string tmpname;
            ss>>tmpname;
            ss>>tmpname;
            whichdb->showtable(tmpname);
        }
        else if(op=="TABLES"){
            whichdb->showname();
        }
        ss.str("");
        return false;
    }
    else if(op=="QUIT"){//删库跑路
        map<string,Database*>::iterator it;
        it = database.begin();
        while(it != database.end())
        {
            delete it->second;
            it ++;
        }
        return true;
    }
	else if(op=="DELETE")
	{
		whichdb->deletedata();
	}
	else if(op=="UPDATE")
	{
		whichdb->update();
	}
    else if(op=="LOAD")
    {
        whichdb->load();
    }
    ss.str("");
    return false;
}

