#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

typedef long long ll;

struct Date
{
    ll day, month;
    Date() : day(0), month(0) {}
    Date(ll day, ll month) : day(day), month(month) {}

    bool operator<(const Date &other)
    {
        return month < other.month || (month == other.month && day < other.day);
    }
    bool operator<=(const Date &other)
    {
        return month < other.month || (month == other.month && day <= other.day);
    }
};

vector<string> split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

void process_group(string name, ll cur_day, ll cur_month)
{
    cout << "Group " << name << ":" << endl;
    ifstream in(name + ".csv");
    string line;
    getline(in, line);
    getline(in, line);
    getline(in, line);
    vector<Date> deadlines;
    string mains = line.substr(5);
    vector<string> parts = split(mains, ",,,");
    for (ll idx = 0; idx < ll(parts.size()); idx++)
    {
        string s = parts[idx].substr(string{"Дедлайн: "}.length());
        Date d;
        if (idx == ll(parts.size()) - 1)
            d = {1, 6};
        else
        {
            vector<string> date = split(s, ".");
            d = {stoll(date[0]), stoll(date[1])};
        }
        deadlines.push_back(d);
    }
    getline(in, line);
    while (getline(in, line))
    {
        if (line[0] == ',')
            break;
        vector<string> parts = split(line, ",");
        string name = parts[1];
        ll cur = 6;
        bool f = true;
        for (ll idx = 0; idx < ll(deadlines.size()); idx++, cur += 3)
        {
            Date deadline = deadlines[idx];
            if (!(deadline < Date{cur_day, cur_month}))
                continue;
            if (cur >= ll(parts.size()) || parts[cur].length() == 0)
            {
                f = false;
                break;
            }
            vector<string> date = split(parts[cur], ".");
            Date d = {stoll(date[0]), stoll(date[1])};
            f &= d <= deadlines[idx];
        }
        if (f)
            cout << name << endl;
    }
}

int main()
{
    time_t cur_time = time(NULL);
    struct tm *atime = localtime(&cur_time);
    ll cur_day = atime->tm_mday, cur_month = atime->tm_mon + 1;
    process_group("250502", cur_day, cur_month);
    process_group("250503", cur_day, cur_month);
}