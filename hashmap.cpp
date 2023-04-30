#include <unordered_map>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    ifstream in("top-1m.txt");
    unordered_map<string, string> m;
    string s;
    vector<pair<string, string>> v;
    while (getline(in, s))
    {
        size_t idx = s.find(" ");
        string key = s.substr(0, idx);
        string value = s.substr(idx + 1);
        v.push_back({key, value});
    }
    in.close();
    clock_t start = clock();
    for (auto &i : v)
        m[i.first] = i.second;
    clock_t end = clock();
    cout << "unordered_map: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
}