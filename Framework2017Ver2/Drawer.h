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

	struct ModelMV1 {
		Matrix matrix;
		int mesh;
		int anime;
		double time;
		bool dx_mat;
		ModelMV1( );
		ModelMV1( Matrix matrix_, int mesh_, int anime_ = -1, double time_ = 0 );
	};

	struct ModelSelf {
		ModelPtr model;
		int graph;
		bool add;
		bool z_buffer;
	};

	struct Billboard {
		Vector pos;
		double size;
		int res;
		BLEND blend;
		double ratio;
		Billboard( );
		Billboard( Vector pos_, double size_, int res_, BLEND blend_, double ratio_ );
	};
	
	struct Effect {
		int id;
		int handle;
		Vector pos;
		Vector rotate;
		double size;
		Effect( );
		Effect( int id_, const Vector& pos_, double size_ = 1.0, const Vector& rotate_ = Vector( ) );
	};

	struct Circle {
		Vector pos;
		double radius;
		Circle( );
		Circle( const Vector& pos_, const double radius_ );
	};

public:
	void initialize( );
	void update( );
	void drawModelMV1( const ModelMV1& model );
	void drawSprite( const Sprite& sprite );
	void drawCircle( const Circle& circle );
	void drawBillboard( const Billboard& billboard );
	void drawEffect( const Effect& effect );
	void loadMV1Model( int motion, const char* filename );
	void loadGraph( int res, const char* filename );
	void loadEffect( int id, const char* filename );
	void createGraph( int res, int width, int height );
	void drawSpriteToGraph( int res, const Sprite& sprite );
	void clearToGraph( int res );
	void unloadGraph( int res );
	void unloadAllGraph( );
	double getEndAnimTime( int res );
	void drawString( int x, int y, const char* string, ... );
	void drawLine( int x1, int y1, int x2, int y2 );
	void drawLine( const Vector& pos1, const Vector& pos2 );
	void setCameraUp( const Vector& up );
	void setCamera( const Vector& pos, const Vector& target );
	void resetFPS( );
	void flip( );

private:
	const char* _directory;

	static const int MODEL_ID_NUM = 1000;
	std::array< int, MODEL_ID_NUM > _mv1_id;
	
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
