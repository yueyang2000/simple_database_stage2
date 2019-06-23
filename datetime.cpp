#include <time.h>
#include <sstream>
#include <stack>

using namespace std;

bool legal_date(string date)
{
	bool ans = true;
	if (date.size() != 10)
	{
		ans = false;
	}
	else
	{
		int count = 0;
		for (int i = 0; i < date.size(); i++)
		{
			if (date[i] == '-')
			{
				date[i] = ' ';
				count++;
			}
		}
		if (count != 2)
		{
			ans = false;
		}
		else
		{
			stringstream ss;
			ss << date;
			string y = "", m = "", d = "";
			ss >> y >> m >> d;
			int iy = atoi(y.c_str());
			int im = atoi(m.c_str());
			int id = atoi(d.c_str());
			if ((im > 12) || (im <= 0))
			{
				ans = false;
			}
			else if ((im == 4) || (im == 6) || (im == 9) || (im == 11))
			{
				if ((id <= 0) || (id > 30))
				{
					ans = false;
				}
			}
			else if (im == 2)
			{
				if ((iy % 400 == 0) || ((iy % 100 != 0) && (iy % 4 == 0)))
				{
					if ((id <= 0) || (id > 29))
					{
						ans = false;
					}
				}
				else
				{
					if ((id <= 0) || (id > 28))
					{
						ans = false;
					}
				}
			}
			else
			{
				if ((id > 31) || (id <= 0))
				{
					ans = false;
				}
			}
		}
	}
	return ans;
}
string Current_TimeStamp()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime((&t)));
	string timestamp = tmp;
	return timestamp;
}
string CurDate()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime((&t)));
	string date = tmp;
	return date;
}

string CurTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%X", localtime((&t)));
	string time = tmp;
	return time;
}

class Day
{
private:
	int y;
	int m;
	int d;
	int increase;
	int days[12];
public:
	//根据提供的y, m, d, increase构造一个计算对象，并初始化每月的天数
	Day(int y, int m, int d, int increase) :y(y), m(m), d(d), increase(increase)
	{
		int i;
		for (i = 1; i <= 12; i++)
		{
			switch (i)
			{
			case 2:
				days[i - 1] = isLeapYear() ? 29 : 28; //设置2月天数
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				days[i - 1] = 30;
				break;
			default:
				days[i - 1] = 31;
			}
		}
	}
	//根据当前的y, m, d, increase计算加过后的日期
	void calculate()
	{
		if (d + increase <= days[m - 1])
			d += increase;
		else
		{
			increase -= days[m - 1] - d;
			m++;
			if (m > 12)
			{
				m = 1;
				y++;
			}
			while (increase - days[m - 1] > 0)
			{
				increase -= days[m - 1];
				m++;
				if (m > 12)
				{
					y++;
					m = 1;
					days[1] = isLeapYear() ? 29 : 28;
				}
			}
			d = increase;
		}
	}
	//判断是闰年吗
	bool isLeapYear()
	{
		return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
	}
	int getyear()
	{
		return y;
	}
	int getmonth()
	{
		return m;
	}
	int getday()
	{
		return d;
	}
};

string adddate(string& op)
{
	string temp = op;
	for (int i = 0; i < temp.size(); i++)
	{
		if ((temp[i] == '(') || (temp[i] == ')') || (temp[i] == '\"') || (temp[i] == '\'') || (temp[i] == ';') || (temp[i] == ',') || (temp[i] == '-'))
		{
			temp[i] = ' ';
		}
	}
	stringstream ss(temp);
	int y, m, d, increase;
	ss >> y >> m >> d >> increase;
	if ((y == 0) && (m == 0) && (d == 0))
	{
		return "0000-00-00";
	}
	Day addday(y, m, d, increase);
	addday.calculate();
	string tmp, year, month, day;
	year = to_string(addday.getyear());
	month = to_string(addday.getmonth());
	day = to_string(addday.getday());
	if (year.size() != 4)
	{
		for (int i = year.size(); i < 4; i++)
		{
			year = "0" + year;
		}
	}
	if (month.size() == 1)
	{
		month = "0" + month;
	}
	if (day.size() == 1)
	{
		day = "0" + day;
	}
	tmp = "";
	tmp = year + "-" + month + "-" + day;
	return tmp;
}

bool legal_date(string& date)
{
	bool ans = true;
	if (date.size() != 10)
	{
		ans = false;
	}
	else
	{
		int count = 0;
		for (int i = 0; i < date.size(); i++)
		{
			if (date[i] == '-')
			{
				date[i] = ' ';
				count++;
			}
		}
		if (count != 2)
		{
			ans = false;
		}
		else
		{
			stringstream ss;
			ss << date;
			string y = "", m = "", d = "";
			ss >> y >> m >> d;
			int iy = atoi(y.c_str());
			int im = atoi(m.c_str());
			int id = atoi(d.c_str());
			if ((im > 12) || (im <= 0))
			{
				ans = false;
			}
			else if ((im == 4) || (im == 6) || (im == 9) || (im == 11))
			{
				if ((id <= 0) || (id > 30))
				{
					ans = false;
				}
			}
			else if (im == 2)
			{
				if ((iy % 400 == 0) || ((iy % 100 != 0) && (iy % 4 == 0)))
				{
					if ((id <= 0) || (id > 29))
					{
						ans = false;
					}
				}
				else
				{
					if ((id <= 0) || (id > 28))
					{
						ans = false;
					}
				}
			}
			else
			{
				if ((id > 31) || (id <= 0))
				{
					ans = false;
				}
			}
		}
	}
	return ans;
}


class Time
{
	int hour;
	int minute;
	int second;
	int adddata;
public:
	Time(int hour, int minute, int second, int adddata) :hour(hour), minute(minute), second(second), adddata(adddata) {};
	int gethour() { return hour; }
	int getminute() { return minute; }
	int getsecond() { return second; }
	void add()
	{
		second = second + adddata;
		adddata = 0;
		while (second > 59)
		{
			second = second - 60;
			minute++;
		}
		while (minute > 59)
		{
			minute = minute - 60;
			hour++;
		}
		return;
	}
};

string addtime(string& op)
{
	for (int i = 0; i < op.size(); i++)
		if ((op[i] == '\'') || (op[i] == ':') || (op[i] == '(') || (op[i] == ')') || (op[i] == ','))
			op[i] = ' ';
	stringstream ss;
	ss << op;
	int h, m, s, at;
	ss >> h >> m >> s;
	string add;
	ss >> add;
	for (int i = add.size(); i < 6; i++)
		add = '0' + add;
	string ah = "", am = "", as = "";
	ah.push_back(add[0]); ah.push_back(add[1]);
	am.push_back(add[2]); am.push_back(add[3]);
	as.push_back(add[4]); as.push_back(add[5]);
	int tah = atoi(ah.c_str());
	int tam = atoi(am.c_str());
	int tas = atoi(as.c_str());
	at = tah * 3600 + tam * 60 + tas;
	Time add_time(h, m, s, at);
	add_time.add();
	string hour = to_string(add_time.gethour());
	string minute = to_string(add_time.getminute());
	string second = to_string(add_time.getsecond());
	if (hour.size() == 1)
		hour = '0' + hour;
	if (minute.size() == 1)
		minute = '0' + minute;
	if (second.size() == 1)
		second = '0' + second;
	string ans = hour + ':' + minute + ':' + second;
	return ans;
}