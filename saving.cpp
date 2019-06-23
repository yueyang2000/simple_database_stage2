#include "Database.h"
#include <fstream>
/*写入一个文件database,依次写入：
database总数
database1的名字
含有表的总数
table1名字
列总数 行总数
column1的名字 类型 是否非空 是否主键
列数据
...
*/
void save_database() {
	ofstream fout;
	streambuf* backup;
	backup = cout.rdbuf();
	fout.open("database");
	cout.rdbuf(fout.rdbuf());
	cout << database.size() << ' ';
	for (auto db = database.begin(); db != database.end(); db++) {
		cout << db->first << ' ';
		int tnum = (int)db->second->tables.size();
		cout << tnum << ' ';
		for (auto tb = db->second->tables.begin(); tb != db->second->tables.end(); tb++) {
			cout << tb->first << ' ';
			int cnum = (int)tb->second->order.size();
			int rnum = tb->second->size;
			cout << cnum << ' ' << rnum << ' ';
			for (int col = 0; col < cnum; col++) {
				colbase* ptr = tb->second->columns[tb->second->order[col]];
				cout << ptr->cname << ' ' << ptr->type << ' ' << ptr->notnull << ' ' << ptr->primarykey << ' ';
				handle_col column(ptr);
				for (int row = 0; row < rnum; row++) {
					column.select(row);
					cout << ' ';
				}
			}
		}
	}
	fout.close();
	cout.rdbuf(backup);
}

void load_database() {
	ifstream fin("database");
	if (!fin.good()) return;
	int dbnum;
	fin >> dbnum;
	for (int i = 0; i < dbnum; i++) {
		string dbname;
		fin >> dbname;
		Database* db = new Database(dbname);
		//cout<<"create database "<<dbname<<endl;
		database[dbname] = db;
		int tbnum;
		fin >> tbnum;
		for (int j = 0; j < tbnum; j++) {
			string tbname;
			fin >> tbname;
			Table* tb = new Table(tbname);
			//cout<<"create table "<<tbname<<endl;
			db->tables[tbname] = tb;
			int cnum, rnum;
			fin >> cnum >> rnum;
			tb->size = rnum;
			tb->rnum = rnum;
			for (int k = 0; k < cnum; k++) {
				string cname;
				string type;
				bool notnull;
				bool primary;
				fin >> cname >> type >> notnull >> primary;
				colbase* col = nullptr;
				if (type == "INT") {
					col = new Column<int>(cname, notnull, type);
				}
				else if (type == "DOUBLE") {
					col = new Column<double>(cname, notnull, type);
				}
				else if (type == "CHAR") {
					col = new Column<string>(cname, notnull, type);
				}
				else if (type == "DATE") {
					col = new Column<string>(cname, notnull, type);
				}
				else if (type == "TIME") {
					col = new Column<string>(cname, notnull, type);
				}
				else {}
				if (primary) { col->setkey(); tb->primarykey = cname; }
				handle_col column(col);
				for (int l = 0; l < rnum; l++) {
					string val;
					fin >> val;
					column.add_val(val);
				}
				//将列数据塞进Table的操作
				tb->order.push_back(cname);
				tb->columns[cname] = col;
				tb->typemap[cname] = type;
				//cout<<"create column "<<cname<<endl;
			}
		}
	}
	fin.close();
	//cout << "record found, load succeed!\n";
}