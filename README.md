# 大作业——第二阶段
## 代码阅读笔记
- 处理sql语句的方式是使用全局的字符串流，先读前几个关键字，在process_operation函数里分流。可以考虑在这个函数的最下方再调用一个新写的函数实现附加功能分流（因为本来这个函数已经相当长了）
- database.cpp 207行op==where不知道处理的是哪种语句
- 注意容器里存的都是指针
- database::select()里处理FROM哪列和哪个表的问题，然后调用table::select()继续处理WHERE
- whereclause实现的方式很奇怪
- 可以实现一下AS关键字
## 修改记录
- 为了读入多行sql语句，添加了`get_operation()`函数
- 实现了LOAD，通过大段复制`Table::inserttocolum`函数实现，看来他们的代码基本是通的。
- 决定创建类型`Quary`全全托管SELECT语句，彻底放弃原有select接口
- 6.3 15：50：无group 的 COUNT基本完成，MAX和MIN暂时搁置
- 6.3 完成第一阶段的select功能
- 6.4早：文件输入输出完成
- group by思路：


## 利用接口
### `colbase`类接口
- `update(int i,T changed)`修改第i个数据点为change
- `swap(int i,int j)`交换i、j两个位置
- `select(int i)`输出第i个位置的值，调用`datastruct::output()`
- `cmp(int pos,int k,T val)`比较键值大小，比较`data[pos].val`和val,k=1大于，k=2小于，k=3等于
- `getvalue(int i)1`获得第i个位置的值
- `initialdata(T x,int place)`在place位置插入占位数据x（比如0），并设为isnull
- `insertdata(T x,int place)`真正在place位置赋值x,并将isnull置否
### `Table`类成员
- 表格列数size
- 主键名primarykey
- `vector<string> order`列的排列顺序
- `map<string,colbase*> columns `存着各列
- `map<string,string> typemap`存着列type
### `database`类成员
- `whichdb->Table* gettable(string& x)`找到表格
##附加功能
- AS
- DISTINCT
- ORDER升降序