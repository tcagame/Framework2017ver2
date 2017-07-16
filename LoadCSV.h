#pragma once
#include <vector>
#include <string>

class LoadCSV {
public:
	LoadCSV( char* filename );
	virtual ~LoadCSV( );
public:
	std::string getCSVData( int index );
private:
	std::vector<std::string> _csv_data;
};

