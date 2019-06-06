
#include "Database.h"

Database* whichdb;
string operation;
stringstream ss;
map <string,Database*> database;
void save_database();
void load_database();
bool get_operation()
{
    operation="";
    string temp;
    if(!getline(cin,temp))
    {
        return false;
    }
    operation+=temp;
    while(temp[temp.size()-1]!=';')
    {
        getline(cin,temp);
        operation+=" ";
        operation+=temp;
    }
    return true;
}
int main(){
    load_database();
    freopen("/Users/yueyang/yiqunyang/大一下/面向对象的程序设计基础/database/database/input.txt","r",stdin);
    while(get_operation()){
        //在未遇到EOF之前逐行读入字符串，每行是一个指令
        if(Process_operation(operation))break;
        //只有遇到QUIT指令时会直接跳出循环，结束进程
        save_database();
    }
    save_database();
    fclose(stdin);
    return 0;
}

