# 大作业——第二阶段
## 开发日志
- 处理sql语句的方式是使用全局的字符串流，先读前几个关键字，在process_operation函数里分流。可以考虑在这个函数的最下方再调用一个新写的函数实现附加功能分流（因为本来这个函数已经相当长了）
- 注意容器里存的都是指针
- 为了读入多行sql语句，添加了`get_operation()`函数
- 实现了LOAD，通过大段复制`Table::inserttocolum`函数实现，看来他们的代码基本是通的。
- 决定创建类型`Quary`全全托管SELECT语句，彻底放弃原有select接口
- 6.3 15：50：无group 的 COUNT基本完成，MAX和MIN暂时搁置
- 6.3 完成第一阶段的select功能
- 6.4早：文件输入输出完成
- 建立Group类型管理group相同字段的标号
- 6.4 17:00:完成group by多列分组标号
- 删除原有select的所有代码以及相关函数、接口，去调Column.cpp和select.cpp两个文件
- 6.4 21：00完成group by输出
- 6.5 0:00 完成order by
- 现有Quary类已经十分臃肿，考虑在多表时建立新的类型，而Quary只处理单表。
- 6.6 15:00 发现select *运行不对，查明原因是因为没有维护好col_output
- 准备支持DATE和TIME类型，仍然使用datapoint，值为int型
- 6.6上午，新增handle_col管理所有类型分支和cast，删减Quary.cpp代码量
- 6.6上午，完成存档、读档，每步操作都存一次（采取的是线性读入读出）
##附加功能
- AS
- DISTINCT
- ORDER升降序

## 利用接口
### `Column`类接口
- `update(int i,T changed)`修改第i个数据点为change
- `swap(int i,int j)`交换i、j两个位置
- `select(int i)`输出第i个位置的值，调用`datastruct::output()`
- `cmp(int pos,int k,T val)`比较键值大小，比较`data[pos].val`和val,k=1大于，k=2小于，k=3等于
- `getvalue(int i)1`获得第i个位置的值
- `initialdata(T x,int place)`在place位置插入占位数据x（比如0），并设为isnull
- `insertdata(T x,int place)`真正在place位置赋值x,并将isnull置否
- `push_back`(int x)添加一个int型值为x的datapoint在data数组最后。
- `getdata(int i)`直接返回第i个datapoint
### `Table`类成员
- 表格列数size
- 主键名primarykey
- `vector<string> order`列的排列顺序
- `map<string,colbase*> columns `存着各列
- `map<string,string> typemap`存着列type
### `database`类成员
- `whichdb->Table* gettable(string& x)`找到表格
