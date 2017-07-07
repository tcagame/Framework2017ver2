#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( ModelMV1 );

class ModelMV1 {
public:
	ModelMV1( );
	virtual ~ModelMV1( );
public:
	void draw( ) const;
	void load( const char* filename );
	void setTexture( const char* filename, bool semi_trans = false );
	void transferTransform( const Matrix& trans_mat );
	void rotateTransform( const Matrix& rot_mat );
	void scaleTransform( const Matrix& scale_mat );
	void reset( );
	double getEndAnimTime( ) const;
private:
	int _mesh;
	int _anim;
	double _time;
	Matrix _trans_mat;
	Matrix _rot_mat;
	Matrix _scale_mat;
};