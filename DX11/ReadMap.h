#pragma once

struct DataInfo
{
	std::string name;
	std::vector<std::string> info;
};

class CReadMap
{
public:
	static std::vector<DataInfo> LoadMap(std::string& filename);
};
