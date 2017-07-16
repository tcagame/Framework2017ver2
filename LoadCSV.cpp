#include "LoadCSV.h"
#include <cstdio>
#include <assert.h>

const int MAX_BUFFER = 100;

LoadCSV::LoadCSV( char* filename ) {
	std::string path = filename;
	path += ".csv";

	FILE* fp;
	errno_t file_load;
	file_load = fopen_s( &fp, path.c_str(), "r" );

	assert( !file_load );

	char buffer[ MAX_BUFFER ];
	while ( fgets( buffer, MAX_BUFFER, fp ) != NULL ) {
		char* context;
		_csv_data.push_back( strtok_s( buffer, ",", &context ) );
	}
}

std::string LoadCSV::getCSVData( int index ) {
	return _csv_data[index];
}

LoadCSV::~LoadCSV( ) {
}
