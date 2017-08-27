#pragma once
#include <string>
#include "smart_ptr.h"

PTR( Movie );

class Movie {
public:
	Movie( );
	virtual ~Movie( );
public:
	bool load( std::string path );
	void draw( ) const;
	void stop( );
	void play( bool is_loop = true );
	bool isPlay( ) const;
private:
	int _handle;
	bool _is_play;
};

