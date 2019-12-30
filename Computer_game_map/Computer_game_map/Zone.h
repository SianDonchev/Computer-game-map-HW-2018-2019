#pragma once
#include <string>
#include <vector>
#include <utility>

class Zone
{

public:
	Zone() {}
	Zone(std::string name)
		:name(name), isVisited(false) {}

	void addAdjacentZone(std::pair<std::string, std::string> adjacent)
	{
		adjacentZones.push_back(adjacent);
	}

	void addKey(std::string key)
	{
		keys.push_back(key);
	}

	const  std::vector<std::pair<std::string, std::string>>& getAdjacentZones()const
	{
		return adjacentZones;
	}

	 const std::vector<std::string>& getKeys()const
	 {
		 return keys;
	 }

	 void markAsVisited()
	 {
		 isVisited = true;
	 }

	 bool hasTheZoneKeysInIt()const
	 {
		 return !keys.empty();
	 }

	 bool isTheZoneVisited()const
	 {
		 return isVisited;
	 }

private:
	std::string name;
	bool isVisited;

	std::vector<std::string> keys;

	std::vector<std::pair<std::string, std::string>> adjacentZones;//{zoneName,keyNeeded}
	
};