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
    RTree *rtree = new RTree(1000, 3000);
    ifstream file("green_tripdata_2015-02.csv");

    string line;
    int cont = 1;
	// int sizeData = 100000;
	// vector<int> sizeData = {100000, 250000, 500000, 700000, 1000000};
	// int i = 0;

	getline(file, line);
    clock_t clock1 = clock();
    while (getline(file, line))
    {
        auto v = split(line, ",");
		rtree->insert(point_t(to_string(cont), {stod(v[5]), stod(v[6])}));
		++cont;
    }
    clock_t clock2 = clock();
    cout << "Time: " << (float)(clock2 - clock1)/CLOCKS_PER_SEC << endl;
	cout << cont << endl;
	clock1 = clock();
	auto v = rtree->rangeSearch(MBR{-73.5, 40, -73, 41});
	clock2 = clock();
    cout << "Time: " << (float)(clock2 - clock1)/CLOCKS_PER_SEC << endl;
	cout << v.size() << endl;

	/* COMPROBAR POR FUERZA BRUTA (línea por línea) que la cantidad de taxis traídos por el Rtree en el RangeSearch es correcta*/
	// int cont2 = 0;
	// getline(file,line);
	// while (getline(file,line))
	// {
	// 	auto v = split(line, ",");
	// 	if (-73.5 <= stod(v[5]) && stod(v[5]) <= -73 && 40 <= stod(v[6]) && stod(v[6]) <= 41)
	// 	{
	// 		++cont2;
	// 	}
	// }
	// clock_t clock3 = clock();
    // cout << "Time: " << (float)(clock3 - clock1)/CLOCKS_PER_SEC << endl;
	// cout << cont2 << endl;
    return 0; 
}