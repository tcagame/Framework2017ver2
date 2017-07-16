#pragma once
#include "Task.h"
#include "mathmatics.h"
#include <string>
#include <array>

PTR( Drawer );
PTR( Model );

class Drawer : public Task {
public:
	static DrawerPtr getTask( );
	static std::string getTag( ) { return "DRAWER"; }
public:
	Drawer( const char* directory );
	virtual ~Drawer( );
public:
	enum BLEND {
		BLEND_NONE,
		BLEND_ALPHA,
		BLEND_ADD,
		MAX_BLEND,
	};
	struct Transform {
		int sx;
		int sy;
		int tx;
		int ty;
		int tw;
		int th;
		int sx2;
		int sy2;
		Transform( );
		Transform( int sx_, int sy_, int tx_ = -1, int ty_ = -1, int tw_ = -1, int th_ = -1, int sx2 = -1, int sy2 = -1 );
	};
	struct Sprite {
		Transform trans;
		int res;
		BLEND blend;
		double ratio;
		Sprite( );
		Sprite( Transform trans_, int res_, BLEND blend_ = BLEND_NONE, double ratio_ = 1.0 );
	};

public:
	void initialize( );
	void update( );
	void drawSprite( const Sprite& sprite );
	void drawCircle( const Vector& pos, const double radius );
	void drawBillboard( const Vector& pos, double size, int res, BLEND blend, double ratio );
	void drawEffect( int id_, const Vector& pos_, double size_ = 1.0, const Vector& rotate_ = Vector( ) );
	void loadGraph( int res, const char* filename );
	void loadEffect( int id, const char* filename );
	void createGraph( int res, int width, int height );
	void drawSpriteToGraph( int res, const Sprite& sprite );
	void clearToGraph( int res );
	void unloadGraph( int res );
	void unloadAllGraph( );
	void drawString( int x, int y, const char* string, ... );
	void drawLine( int x1, int y1, int x2, int y2 );
	void drawLine( const Vector& pos1, const Vector& pos2 );
	void setCameraUp( const Vector& up );
	void setCamera( const Vector& pos, const Vector& target );
	void resetFPS( );
	void flip( );
	int getTextureHeight( const int res );
	int getTextureWidth( const int res );
private:
	const char* _directory;
	
	static const int GRAPHIC_ID_NUM = 1000;
	std::array< int, GRAPHIC_ID_NUM > _graphic_id;
	
	static const int EFFECT_ID_NUM = 1000;
	std::array< int, EFFECT_ID_NUM > _effect_id;

	int _refresh_count;
	int _start_time;
	int _effekseer_fix_graph;
	double _fps;

	Vector _camera_up;
};
