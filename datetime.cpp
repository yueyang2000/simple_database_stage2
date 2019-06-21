#include <time.h>
#include <sstream>
#include <stack>
using namespace std;
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
	tmp = year + "-" + month + "-" + day ;
	return tmp;
}

/*Time类和string addtime(string&)打包在一起，string addtime(string&)接受ADDTIME后面的语句，返回一个以string类型储存的时间*/
/*接受样例格式：('01:00:00.999999', '02:00:00.999998')*/
/*输出样例格式：'03:00:01.999997'*/
/*接受样例格式：('01:00:00.999999', '00:00.000001')*/
/*输出样例格式：'01:00:01:000000'*/

class Time
{
private:
	int hour;
	int min;
	int second;
	int pointsecond;
	string add_time;
public:
	Time(int hour, int min, int second, int pointsecond, string add_time)
		:hour(hour), min(min), second(second), pointsecond(pointsecond), add_time(add_time){}
	void calculate()
	{
		stringstream ss(add_time);
		stack<int> s;
		int nowtime;
		while (ss >> nowtime)
		{
			s.push(nowtime);
		}
		int times[4] = { 0, 0, 0, 0 };
		int size0 = s.size();
		for (int i = 3; i >= 4 - size0; i--)
		{
			times[i] = s.top();
			s.pop();
		}
		hour = hour + times[0];
		min = min + times[1];
		second = second + times[2];
		pointsecond = pointsecond + times[3];
		while (pointsecond > 999999)
		{
			pointsecond = pointsecond - 1000000;
			second++;
		}
		while (second > 59)
		{
			second = second - 60;
			min++;
		}
		while (min > 59)
		{
			min = min - 60;
			hour++;
		}
	}
	string thans()
	{
		string temp;
		string hour0 = to_string(hour);
		string min0 = to_string(min);
		string second0 = to_string(second);
		string pointsecond0 = to_string(pointsecond);
		if (hour0.size() == 1)
		{
			hour0 = "0" + hour0;
		}
		if (min0.size() == 1)
		{
			min0 = "0" + min0;
		}
		if (second0.size() == 1)
		{
			second0 = "0" + second0;
		}
		if (pointsecond0.size() != 6)
		{
			for (int i = pointsecond0.size(); i < 6; i++)
			{
				pointsecond0 = "0" + pointsecond0;
			}
		}
		temp = hour0 + ":" + min0 + ":" + second0 + "." + pointsecond0;
		return temp;
	}
};

string addtime(string& op)
{
	string temp = op;
	for (int i = 0; i < temp.size(); i++)
	{
		if ((temp[i] == '(') || (temp[i] == ')') || (temp[i] == '\"') || (temp[i] == '\'') || (temp[i] == ';') || (temp[i] == ','))
		{
			temp[i] = ' ';
		}
	}
	stringstream ss(temp);
	string org, plus;
	ss >> org >> plus;
	for (int i = 0; i < org.size(); i++)
	{
		if ((org[i] == '.') || (org[i] == ':'))
		{
			org[i] = ' ';
		}
	}
	for (int i = 0; i < plus.size(); i++)
	{
		if ((plus[i] == '.') || (plus[i] == ':'))
		{
			plus[i] = ' ';
		}
	}
	int hour, minute, second, pointsecond;
	stringstream ss1(org);
	ss1 >> hour >> minute >> second >> pointsecond;
	Time addti(hour, minute, second, pointsecond, plus);
	addti.calculate();
	return addti.thans();
}
