#pragma once
#include <string>
#include "smart_ptr.h"
#include "mathmatics.h"

PTR( Image );

class Image {
public:
	Image( );
	virtual ~Image( );
public:
	enum BLEND {
		BLEND_NONE,
		BLEND_ALPHA,
		BLEND_ADD,
		MAX_BLEND,
	};
public:
	bool load( std::string path );
	void draw( ) const;
	void setRect( int tx, int ty, int tw, int th );
	void setPos( const Vector& pos, const Vector& pos2 = Vector( ) );
	void getImageSize( int& width, int& height ) const;
	void setHandle( int handle );
private:
	int _handle;
	int _sx1;
	int _sy1;
	int _sx2;
	int _sy2;
	int _tx;
	int _ty;
	int _tw;
	int _th;
	BLEND _blend;
	double _ratio;
};

