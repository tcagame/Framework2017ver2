#include "Drawer.h"
#include "Application.h"
#include "DxLib.h"
#include "Image.h"

#if EFFECKSEER
	// EffekseerForDXLib.hをインクルードします。
#	include "EffekseerForDXLib.h"
#endif

#include <assert.h>

static const int REFRESH_COUNT = 60;	//平均を取るサンプル数
static const int FPS = 30;

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
	_start_time = 0;
#if EFFECKSEER
	_effekseer_fix_graph = LoadGraph( "../Resource/effecksser_fix.png" );
#endif
}

void Drawer::update( ) {
}

void Drawer::drawCircle( const Vector& pos, const double radius ) const {
	DrawCircle( ( int )pos.x, ( int )pos.y, ( int )radius, GetColor( 255, 0, 0 ), 0, 3 );
}

void Drawer::drawEffect( int id, const Vector& pos, double size, const Vector& rotate ) const {
# if EFFECKSEER
		DrawGraph( 0, 0, _effekseer_fix_graph, TRUE );
		int handle = PlayEffekseer3DEffect( id );
		float size = ( float )size;
		SetScalePlayingEffekseer3DEffect( handle,
			size, size, size );
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
	ImagePtr result( ImagePtr( new Image ) );
	std::string path = _directory;
	path += "/";
	path +=  filename;
	bool check_image = result->load( path );
	assert( check_image );
	return result;
}

void Drawer::resetFPS( ) {
	_refresh_count = 0;
	_start_time = GetNowCount( );
}

void Drawer::flip( ) {
	if ( _refresh_count == 0 ) {
		_start_time = GetNowCount( );
	}
	if ( _refresh_count == REFRESH_COUNT ){ //60フレーム目なら平均を計算する
		int frame_time_sum = GetNowCount( ) - _start_time;//かかった時間
		double frame_time_avg = frame_time_sum / REFRESH_COUNT;//平均
		if ( frame_time_avg != 0.0 ) {
			_fps = 1000.0 / frame_time_avg;
		}
		_refresh_count = 0;
		_start_time = GetNowCount( );
	}
	_refresh_count++;

	int took_time = GetNowCount( ) - _start_time;	//かかった時間
	int wait_time = _refresh_count * 1000 / FPS - took_time;	//待つべき時間
	if ( wait_time > 0 ) {
		Sleep( wait_time );	//待機
	}
	

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
