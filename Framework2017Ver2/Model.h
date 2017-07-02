#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( Model );
PTR( ModelImpl );

class Model {
public:
	Model( );
	virtual ~Model( );
public:
	struct VERTEX {
		Vector pos;
		double u;
		double v;
		VERTEX( ) { }
		VERTEX( 
			Vector pos_,
			double u_,
			double v_ ) {
				pos = pos_;
				u = u_;
				v = v_;
		}
	};
public:
	void draw( bool trans = false ) const;
	void draw( int texture, bool trans = false ) const;
	void multiply( Matrix matrix );
	void alloc( int polygon_num );
	void setPolygonNum( int num );
	int  getPolygonNum( ) const;
	void set( int n, VERTEX vertex ); // n * 3
	bool load( std::string filename );
	void save( std::string filename );
	void mergeModel( ModelConstPtr model );
	void translate( Vector move );
	void setPos( Vector pos );
	void reset( );
	void setTexture( const char* texture_path );
	VERTEX getVERTEX( int idx );
	Vector getPoint( int idx ) const;
	Vector getPos( ) const;
	ModelImplConstPtr getModelImpl( ) const;
	int getTextureHandle( const char* filename );
private:
	ModelImplPtr _impl;
	Vector _origin_pos;
	int _texture_handle;
};

