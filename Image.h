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
	void setRect( int tx = -1, int ty = -1, int tw = -1, int th = -1 );
	void setPos( const int sx1, const int sy1, const int sx2 = -1, const int sy2 = -1 );
	void getImageSize( int& width, int& height ) const;
	int getHandle( ) const;
	void setBlend( BLEND blend, double ratio );
protected:
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

