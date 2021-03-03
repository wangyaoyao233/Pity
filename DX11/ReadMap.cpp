#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ReadMap.h"

using namespace std;
std::vector<DataInfo> CReadMap::LoadMap(std::string& filename)
{
	fstream fs(filename);

	string line, word;
	vector<DataInfo> data;

	while (getline(fs, line))
	{
		DataInfo di;
		istringstream record(line);
		record >> di.name;
		while (record >> word)
		{
			di.info.push_back(word);
		}
		data.push_back(di);
	}

	fs.close();
	return data;
}