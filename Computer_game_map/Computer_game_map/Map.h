#pragma once
#include <iostream>
#include "Zone.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>

const int MAX = 124;

class Map
{

public:
	Map(std::ifstream& ifs);

	void saveToFileDot(std::ofstream& ofs,const std::string startZone);


private:
	void loadMap(std::ifstream & ifs);

	void readFromFile(std::ifstream&ifs, char* const& str,char delim)const;

	bool alreadyAdded(const std::string& name,const std::vector<std::string>& names)const;
	
	void printAdjacentZones(std::ofstream& ofs, const std::string& name, const std::vector<std::pair<std::string, std::string>>& adjacentZones)const;

	void printZoneInfo(std::ofstream& ofs, const std::string& name)const;


private:

	void takeKeys(std::unordered_set<std::string>& keys,const std::vector<std::string>& keysOfZone)const;

	void insertKeysFromThatZone(const std::string & name, std::unordered_map<std::string, std::unordered_set<std::string>>& visited, const std::vector<std::string>& keys)const;

	void BFS_wrapper(std::string start);

	void BFS(const std::string & start,std::unordered_set<std::string>& keysInTheBag);

	void insert(const std::string& name, std::unordered_map<std::string, std::unordered_set<std::string>>& visited,const std::unordered_set<std::string>& keys)const;//to insert the start

	bool differentKeysThanThePreviousVisit(const std::unordered_set<std::string>& previousKeys, const std::unordered_set<std::string>& currKeys)const;

	void takeKeysFromThePreviousVisit( std::unordered_set<std::string>& previousKeys, const std::unordered_set<std::string>& currKeys)const;

	void insertKeysFromTheBag(const std::string & name, std::unordered_map<std::string, std::unordered_set<std::string>>& visited, const std::unordered_set<std::string>& keys)const;

	bool isValidZone(const std::string& key)const;

private:
	std::vector<std::string> allZones;
	std::unordered_map<std::string, Zone> map;
};