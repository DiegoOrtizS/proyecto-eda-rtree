#include <iostream>
#include <fstream>
#include "RTree.h"
#include <string>
#include <ctime>

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos)
		{
			pos = str.length();
		}
		string token = str.substr(prev, pos - prev);
		if (!token.empty())
		{
			tokens.push_back(token);
		}
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());

	return tokens;
}


// 5 longitude (x) y 6 latitude (y) cols
int main()
{
    RTree *rtree = new RTree(2, 3);
    ifstream file("green_tripdata_2015-01.csv");

    string line;
    int cont = 1;
	int sizeData = 1000000;

	getline(file, line);
    clock_t clock1 = clock();
    while (getline(file, line))
    {
		// if (cont == sizeData) break;
        auto v = split(line, ",");
		rtree->insert(point_t(to_string(cont), {stod(v[5]), stod(v[6])}));
		cout << cont << endl;
		++cont;
    }
    clock_t clock2 = clock();
    cout << "Time: " << (float)(clock2 - clock1)/CLOCKS_PER_SEC << endl;

	cout << cont << endl;
    return 0; 
}