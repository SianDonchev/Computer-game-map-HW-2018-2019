#pragma once
#include "Map.h"
#include <iomanip>

inline void makeTheNameInDotFormat(char*const& name)
{
	size_t len = strlen(name);
	int i = 0;
	name[len+(i++)] = '.';
	name[len + (i++)] = 'd';
	name[len + (i++)] = 'o';
	name[len + (i++)] = 't';
	name[len + i] = '\0';
}

inline void menu()
{
	std::cout << "---------------------\n"
		<< "The expected format of the input file is:\n"
		<< "[zones]\n"
		<< "zone -> zone [key];\n"
		<< ".\n" << ".\n" << ".\n" << '\n'
		<< "[keys]\n"
		<< "key -> zone;\n"
		<< ".\n" << ".\n" << ".\n"<<'\n';
	

	char fileName[MAX];
	char startZoneName[MAX];
	
	std::cout << "Please enter the file you want to read the map from:";
	std::cin >> std::setw(MAX) >> fileName;

	std::ifstream ifs(fileName);

	if (!ifs.good())
	{
		std::cout << "Fail in file opening!\n";
		return;
	}
	

	Map map(ifs);
	std::cout << "Map loaded!\n";

	std::cout << "Please enter a file name in which you want to save the graph:\n->";
	std::cin >> std::setw(MAX) >> fileName;

	makeTheNameInDotFormat(fileName);

	std::ofstream ofs(fileName);
	if (!ofs.good())
	{
		std::cout << "There was a mistake in the saving!\n";
		return;
	}

	std::cout << "Enter a name for the zone you want to start from:\n->";
	//for the '\n'
	std::cin.get();

	std::cin.getline(startZoneName, MAX);

	map.saveToFileDot(ofs,startZoneName);

}


