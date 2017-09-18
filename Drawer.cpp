#include "Drawer.h"
#include "Application.h"
#include "DxLib.h"
#include "Image.h"
#include "Movie.h"
#include "ImageTarget.h"

#if EFFECKSEER
	// EffekseerForDXLib.hをインクルードします。
#	include "EffekseerForDXLib.h"
#endif

#include <assert.h>

static const int REFRESH_COUNT = 60;	//平均を取るサンプル数
static const int FPS = 30;
static const double FRAME_TIME = 1000.0 / FPS;

DrawerPtr Drawer::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Drawer >( fw->getTask( getTag( ) ) );
}

Drawer::Drawer( const char * directory ) :
_directory( directory ){
}

Drawer::~Drawer( ) {
}

void Drawer::initialize( ) {
	_refresh_count = 0;
	_fps = FPS;
	_start_time = GetNowCount( );
	_fps_start_time = _start_time;
	_before_time = 0;
	_fps_count = 0;
#if EFFECKSEER
	_effekseer_fix_graph = LoadGraph( "Resource/effekseer_fix.png" );
	DrawGraph( 0, 0, _effekseer_fix_graph, TRUE );
#endif
}

void Drawer::update( ) {
	if ( _fps_count >= FPS ) {
		int now = GetNowCount( );
		//0フレーム目からの時間
		int frame_time_sum = now - _fps_start_time;
		//平均の時間
		double frame_time_avg = ( double )frame_time_sum / FPS;
		//fpsの計算
		if ( frame_time_avg != 0.0 ) {
			_fps = 1000.0 / frame_time_avg;
		}
		_fps_start_time = now;
		_fps_count = 0;
	} else {
		_fps_count++;
	}
}

void Drawer::drawCircle( const Vector& pos, const double radius ) const {
	DrawCircle( ( int )pos.x, ( int )pos.y, ( int )radius, GetColor( 255, 0, 0 ), 0, 3 );
}

void Drawer::drawSphere( const Vector& pos, const double radius ) const {
	DxLib::VECTOR dx_pos = VGet( float( pos.x ), float( pos.y ), float( pos.z ) );
	DrawSphere3D( dx_pos, ( float )radius, 10, GetColor( 255, 0, 0 ), GetColor( 255, 255, 255 ), FALSE );
}

void Drawer::drawEffect( int id, const Vector& pos, double size, const Vector& rotate ) const {
# if EFFECKSEER
		int handle = PlayEffekseer3DEffect( id );
		SetScalePlayingEffekseer3DEffect( handle,
			( float )size, ( float )size, ( float )size );
		SetRotationPlayingEffekseer3DEffect( handle,
			( float )rotate.x, ( float )rotate.y, ( float )rotate.z );
		SetPosPlayingEffekseer3DEffect( handle,
			( float )pos.x, ( float )pos.y, ( float )pos.z );
		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D( );

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D( );
#	endif
}

void Drawer::loadEffect( int id, const char* filename ) {
#	if EFFECKSEER
		assert( id < EFFECT_ID_NUM );
		std::string path = _directory;
		path += "/";
		path +=  filename;
		_effect_id[ id ] = LoadEffekseerEffect( path.c_str( ) );
		if ( _effect_id[ id ] < 0 ) {
			path = "../" + path;
			_effect_id[ id ] = LoadEffekseerEffect( path.c_str( ) );
			assert( _effect_id[ id ] >= 0 );
		}
#	endif
}

ImagePtr Drawer::createImage( const char* filename ) const {
	ImagePtr result = ImagePtr( );
	std::string path = _directory;
	path += "/";
	path +=  filename;
	ImagePtr image = ImagePtr( new Image );
	if ( image->load( path ) ) {
		result = image;
	}
	if ( !result ) {
		path = "../" + path;
		if ( image->load( path ) ) {
			result = image;
		}
	}
		
	return result;
}

MoviePtr Drawer::createMovie( const char* filename ) const {
	MoviePtr result = MoviePtr( );
	std::string path = _directory;
	path += "/";
	path +=  filename;
	MoviePtr movie = MoviePtr( new Movie );
	if ( movie->load( path ) ) {
		result = movie;
	}
	if ( !result ) {
		path = "../" + path;
		if ( movie->load( path ) ) {
			result = movie;
		}
	}
		
	return result;
}


void Drawer::resetFPS( ) {
	_refresh_count = 0;
}

void Drawer::waitForSync( ) {
	//現在の時間
	int now_time = GetNowCount( );
	if ( _refresh_count == 0 ) {
		_start_time = now_time;
	}

	//60フレーム目なら平均を計算する
	if ( _refresh_count >= REFRESH_COUNT ) { 
		//再度0フレーム目から
		_refresh_count = 0;
		_start_time = now_time;
	}

	//0フレーム目からの時間
	int took_time = now_time - _start_time;

	//0フレーム目からの予想される時間
	int game_time = ( int )( _refresh_count * FRAME_TIME );

	//待つべき時間
	int wait_time = game_time - took_time;

	if ( wait_time > 0 ) {
		//待機
		Sleep( wait_time );
	}

	_refresh_count++;
}

void Drawer::flip( ) {
	ScreenFlip( );
	ClearDrawScreen( );
}

void Drawer::drawLine( int x1, int y1, int x2, int y2 ) const {
	DrawLine( x1, y1, x2, y2, 0xFFFFFF ) ;
}

void Drawer::drawLine( const Vector& pos1, const Vector& pos2 ) const {
	VECTOR p1;
	p1.x = ( float )pos1.x;
	p1.y = ( float )pos1.y;
	p1.z = ( float )pos1.z;
	VECTOR p2;
	p2.x = ( float )pos2.x;
	p2.y = ( float )pos2.y;
	p2.z = ( float )pos2.z;
	DrawLine3D( p1, p2, 0xFFFFFF );
}

void Drawer::drawString( int x, int y, const char* string, ... ) const {
	char buf[ 1024 ];
	va_list ap;
	unsigned int color = 0xFFFFFF;
	va_start( ap, string );
	vsprintf_s( buf, 1024, string, ap );
	DrawString( x, y, buf, color );
	va_end( ap );
}

void Drawer::setCameraUp( const Vector& up ) {
	_camera_up = up;
}

void Drawer::setCamera( const Vector& pos, const Vector& target ) {
	DxLib::VECTOR dx_pos = VGet( float( pos.x ), float( pos.y ), float( pos.z ) );
	DxLib::VECTOR dx_target = VGet( float( target.x ), float( target.y ), float( target.z ) );
	DxLib::VECTOR dx_up = VGet( float( _camera_up.x ), float( _camera_up.y ), float( _camera_up.z ) );
	SetCameraPositionAndTargetAndUpVec( dx_pos, dx_target, dx_up );

#	if EFFECKSEER
		// DXライブラリのカメラとEffekseerのカメラを同期する。
		Effekseer_Sync3DSetting();
#	endif
}

double Drawer::getFps( ) {
	return _fps;
}

bool Drawer::isInCamera( const Vector& pos ) const {
	DxLib::VECTOR dx_pos = VGet( float( pos.x ), float( pos.y ), float( pos.z ) );
	return ( CheckCameraViewClip( dx_pos ) == FALSE ); // TRUE：画面外　FALSE：画面内が返ってくる
}

void Drawer::setImageTarget( ImageTargetPtr image ) {
	if ( image == ImageTargetPtr( ) ) {
		SetDrawScreen( DX_SCREEN_BACK );
	} else {
		SetDrawScreen( image->getHandle( ) );
	}
}
