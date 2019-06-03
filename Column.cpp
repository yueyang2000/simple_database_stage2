
#include "Column.h"

void colbase::showname() {
    cout<<cname;
    if(type=="INT")cout<<'\t'<<"int(11)";//输出类型
    else if(type=="CHAR")cout<<'\t'<<"char(1)";
    else cout<<'\t'<<"double";
    if(notnull)cout<<'\t'<<"NO";//输出notnull
    else cout<<'\t'<<"YES";
    if(primarykey) cout<<'\t'<<"PRI";//输出是否j主键
    else cout<<'\t';
    cout<<'\t'<<"NULL"<<endl;
}


