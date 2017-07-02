#pragma once

#include "smart_ptr.h"
#include <string>

PTR( Parser );
PTR( Model );

class Parser {
public:
	Parser();
	virtual ~Parser();
public:
	ModelPtr makeModel( std::string filename );
};