#include "Map.h"
#include "Menu.h"

Map::Map(std::ifstream & ifs)
{
	loadMap(ifs);
}

void Map::BFS_wrapper(std::string start)
{
	std::unordered_set<std::string> keysInTheBag;

	BFS(start,keysInTheBag);
}

void Map::printAdjacentZones(std::ofstream & ofs, const std::string & name, const std::vector<std::pair<std::string, std::string>>& adjacentZones)const
{
	for (const std::pair<std::string, std::string>& zone : adjacentZones)
	{
		ofs << '"' << name << '"' << " -> " << '"' << zone.first << '"';
		if (!zone.second.empty())
		{
			ofs << " [label=" <<'"' <<zone.second<<'"' <<"];\n";
		}
		else
		{
			ofs << '\n';
		}
	}
}

void Map::printZoneInfo(std::ofstream & ofs, const std::string & name)const
{
  std::unordered_map<std::string, Zone>::const_iterator it = map.find(name);
	
	bool hasKeys = it->second.hasTheZoneKeysInIt();
	bool isVisited = it->second.isTheZoneVisited();

	if (hasKeys)
	{
		ofs << '"' << name << '"';
		const std::vector<std::string>& keys = it->second.getKeys();

		ofs << "[label=" << '"' << name;
		for (const std::string& key : keys)
		{
			ofs << '\\' << 'l' << key;
		}
		ofs << '"';
		if (isVisited)
		{
			ofs << "];\n";
		}
	}

	if (hasKeys && !isVisited)
	{
		ofs << ", color=red, style=filled, fillcolor=" << '"' << "#ffefef" << '"' << "];\n";
	}

	if (!hasKeys && !isVisited)
	{
		ofs << '"' << name << '"' <<  "[color=red, style=filled, fillcolor=" << '"' << "#ffefef" << '"' << "];\n";
	}


}


void Map::saveToFileDot(std::ofstream & ofs, const std::string startZone)
{
	if (!isValidZone(startZone))
	{
		std::cout << "Invalid start zone!\n";
		ofs.close();
		return;
	}

	BFS_wrapper(startZone);

	ofs << "digraph {\n";

	for (const std::string& zone : allZones)
	{
		printZoneInfo(ofs, zone);
		printAdjacentZones(ofs, zone, map.find(zone)->second.getAdjacentZones());
	}
	ofs << '}';

	ofs.close();
}


void Map::takeKeys(std::unordered_set<std::string>& keys, const std::vector<std::string>& keysOfZone)const
{
	for (const std::string& key : keysOfZone)
	{
		keys.insert(key);
	}
}

void Map::loadMap(std::ifstream & ifs)
{

	char zone[MAX];
	char adjacentZone[MAX];
	char key[MAX];
	bool hasKey = false;

	ifs.getline(zone, MAX);
	if (!strcmp(zone, "[zones]"))
	{
		while (!ifs.eof())
		{
			ifs.getline(zone, MAX, '-');
			//for the space
			zone[strlen(zone) - 1] = '\0';
			ifs.ignore(MAX, ' ');

			readFromFile(ifs, adjacentZone, '[');
			size_t adjLen = strlen(adjacentZone);
			//does not has a key
			if (adjacentZone[adjLen - 1] == ';')
			{
				adjacentZone[adjLen - 1] = '\0';
			}
			//has a key
			else
			{     //for the space
				adjacentZone[adjLen - 1] = '\0';

				ifs.getline(key, MAX, ']');
				hasKey = true;

				ifs.ignore(MAX, '\n');
				if (ifs.peek() == '\n')
				{
					ifs.get();
				}
			}

			//add the zone
			Zone* vertex = new Zone(zone);

			map.insert({ zone, *vertex });

			if (hasKey)
			{
				map[zone].addAdjacentZone(std::make_pair(adjacentZone, key));
			}
			else
			{
				map[zone].addAdjacentZone(std::make_pair(adjacentZone, std::string{}));
			}

			delete vertex;
			//add it's adjacent zone
			vertex = new Zone(adjacentZone);
			map.insert({ adjacentZone,*vertex });

			delete vertex;

			if (!alreadyAdded(zone, allZones))
			{
				allZones.push_back(zone);
			}
			if (!alreadyAdded(adjacentZone, allZones))
			{
				allZones.push_back(adjacentZone);
			}
			hasKey = false;

			//if it's time to read the keys
			if (ifs.peek() == ' ' || ifs.peek() == '\n' || ifs.peek() == '[')
			{
				break;
			}
		}

		ifs.getline(zone, MAX);
		while (strcmp(zone, "[keys]") && !ifs.eof())
		{
			ifs.getline(zone, MAX);
		}
		if (ifs.peek() == '\n')
		{
			ifs.get();
		}
		//read the keys
		while (!ifs.eof())
		{
			ifs.getline(key, MAX, '-');
			if (ifs.eof())
			{
				break;
			}
			//for the space
			key[strlen(key) - 1] = '\0';

			ifs.ignore(MAX, ' ');
			ifs.getline(zone, MAX, ';');

			map[zone].addKey(key);
			ifs.ignore(MAX, '\n');
			if (ifs.peek() == '\n')
			{
				ifs.get();
			}
		}
	}
	ifs.close();
}

void Map::readFromFile(std::ifstream&ifs,char * const & str,char delim)const
{
	char c;
	ifs.get(c);
	
	int i = 0;
	while (c != delim && c!='\n')
	{
		str[i] = c;
		ifs.get(c);
		++i;
	}
	str[i] = '\0';
	
	//to extract the '\n'
	if (ifs.peek() == '\n')
	{
		ifs.get();
	}
}

bool Map::alreadyAdded(const std::string& name,const std::vector<std::string>& names)const
{
	size_t size = names.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (name == names[i])
		{
			return true;
		}
	}
	return false;
}

void Map::insertKeysFromThatZone(const std::string & name, std::unordered_map<std::string, std::unordered_set<std::string>>& visited, const std::vector<std::string>& keys)const
{
	visited[name];//for the first call
	for (const std::string& key : keys)
	{
		visited[name].insert(key);
	}
}

void Map::BFS(const std::string & start, std::unordered_set<std::string>& keysInTheBag)
{
	map[start].markAsVisited();

	//unordered_map to save the keys we have on the first visit to the zone
	//if we visit it with diferent keys than the first time
	//call the BFS again with that zone as a start and the new keys
	std::unordered_map<std::string, std::unordered_set<std::string>> visited;

	//take keys from the current zone
	takeKeys(keysInTheBag, map[start].getKeys());

	//makr as visited with the keys from the current zone
	insert(start, visited, keysInTheBag);


	std::queue<std::string> wave;

	wave.push(start);

	while (!wave.empty())
	{
		std::unordered_map<std::string, Zone>::iterator it = map.find(wave.front());
		const std::vector<std::pair<std::string, std::string>>& adjacentZones = it->second.getAdjacentZones();

		//take the keys with which we visited the zone (if it's not the start zone)
		if (it->first != start) {
			takeKeysFromThePreviousVisit(keysInTheBag, visited[it->first]);
		}

		for (const std::pair<std::string, std::string>& adjacentZone : adjacentZones)
		{
			//check if it's visited
			if (visited.find(adjacentZone.first) == visited.end())
			{
				const Zone* curZone = &(map.at(adjacentZone.first));
				//check if we need a key to enter the zone
				if (adjacentZone.second.empty())
				{
					//mark it as visited
					insertKeysFromTheBag(adjacentZone.first, visited, keysInTheBag);
					insertKeysFromThatZone(adjacentZone.first,visited,curZone->getKeys());
			
					wave.push(adjacentZone.first);
					//mark as globally visited(not coloured in red in the output file)
					map[adjacentZone.first].markAsVisited();
				}
				else
				{
					//check if we have the key
					if (keysInTheBag.find(adjacentZone.second) != keysInTheBag.end())
					{
						//mark it as visited
						insertKeysFromTheBag(adjacentZone.first, visited, keysInTheBag);
						insertKeysFromThatZone(adjacentZone.first, visited, curZone->getKeys());

						wave.push(adjacentZone.first);
						map[adjacentZone.first].markAsVisited();
					}
					else
					{
						continue;
					}
				}
			}
			//check if we had different keys the last time we visited the zone
			//if we did,call the BFS again with start that zone,because it may take us more further away(take us to more zones)
			else
			{
				if (differentKeysThanThePreviousVisit(visited[adjacentZone.first], keysInTheBag))
				{
					if (!adjacentZone.second.empty())
					{
						if (keysInTheBag.find(adjacentZone.second) != keysInTheBag.end())
						{
							BFS(adjacentZone.first, keysInTheBag);
						}
					}
					else
					{
						BFS(adjacentZone.first, keysInTheBag);
					}
				}
			}
		}
		wave.pop();
	}
}

void Map::insert(const std::string & name, std::unordered_map<std::string, std::unordered_set<std::string>>& visited,const std::unordered_set<std::string>& keys)const
{
	visited[name] = keys;
}

bool Map::differentKeysThanThePreviousVisit(const std::unordered_set<std::string>& previousKeys, const std::unordered_set<std::string>& currKeys)const
{
	//to check if we have a new key this time
	for (const std::string& key : currKeys)
	{
		if (previousKeys.find(key) == previousKeys.end())
		{
			return true;
		}
	}

	return false;
}

void Map::takeKeysFromThePreviousVisit( std::unordered_set<std::string>& keysInTheBag, const std::unordered_set<std::string>& previousKeys)const
{
	keysInTheBag = previousKeys;
}

void Map::insertKeysFromTheBag(const std::string & name, std::unordered_map<std::string, std::unordered_set<std::string>>& visited, const std::unordered_set<std::string>& keys)const
{
	visited[name];//for the first ccall
	for (const std::string& key : keys)
	{
		visited[name].insert(key);
	}
}

bool Map::isValidZone(const std::string & key) const
{
	for (const std::string& zone : allZones)
	{
		if (key == zone)
		{
			return true;
		}
	}
	return false;
}
