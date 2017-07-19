#include "Drawer.h"
#include "Application.h"
#include "DxLib.h"

#if EFFECKSEER
	// EffekseerForDXLib.hをインクルードします。
#	include "EffekseerForDXLib.h"
#endif

#include <assert.h>

static const int REFRESH_COUNT = 60;	//平均を取るサンプル数
static const int FPS = 30;

Drawer::Transform::Transform( ) :
sx( 0 ),
sy( 0 ),
tx( 0 ),
ty( 0 ),
tw( 0 ),
th( -1 ) {
}

Drawer::Transform::Transform( int sx_, int sy_, int tx_, int ty_, int tw_, int th_, int sx2_, int sy2_ ) :
sx( sx_ ),
sy( sy_ ),
tx( tx_ ),
ty( ty_ ),
tw( tw_ ),
th( th_ ),
sx2( sx2_ ),
sy2( sy2_ ) {
}




Drawer::Sprite::Sprite( ) :
res( -1 ),
blend( BLEND_NONE ) {

}

Drawer::Sprite::Sprite( Transform trans_ ,int res_, BLEND blend_, double ratio_ ) :
trans( trans_ ),
res( res_ ),
blend( blend_ ),
ratio( ratio_ ) {

}

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
	for ( int i = 0; i < GRAPHIC_ID_NUM; i++ ) {
		_graphic_id[ i ] = -1;
	}

	_refresh_count = 0;
	_fps = FPS;
	_start_time = 0;
#if EFFECKSEER
	_effekseer_fix_graph = LoadGraph( "../Resource/effecksser_fix.png" );
#endif
}

void Drawer::update( ) {
}

void Drawer::drawSprite( const Sprite& sprite ) const {
	switch ( sprite.blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * sprite.ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * sprite.ratio ) );
		break;
	}

	if ( sprite.trans.tw < 0 ) {
		DrawGraph( sprite.trans.sx, sprite.trans.sy, _graphic_id[ sprite.res ], TRUE );
	} else if ( sprite.trans.sy2 < 0 ) {
		DrawRectGraph( sprite.trans.sx, sprite.trans.sy, sprite.trans.tx, sprite.trans.ty, sprite.trans.tw, sprite.trans.th, _graphic_id[ sprite.res ], TRUE, FALSE );
	} else {
		DrawRectExtendGraph(
			sprite.trans.sx , sprite.trans.sy ,
			sprite.trans.sx2, sprite.trans.sy2,
			sprite.trans.tx , sprite.trans.ty ,
			sprite.trans.tw , sprite.trans.th , _graphic_id[ sprite.res ], TRUE );
	}

	if ( sprite.blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}


void Drawer::drawCircle( const Vector& pos, const double radius ) const {
	DrawCircle( ( int )pos.x, ( int )pos.y, ( int )radius, GetColor( 255, 0, 0 ), 0, 3 );
}

void Drawer::drawBillboard( const Vector& pos, double size, int res, BLEND blend, double ratio ) const {
	switch( blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD, ( int )( 255 * ratio ) );
		break;
	}

	int cheak = DrawBillboard3D( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ), 0.5f, 0.5f, ( float )size, 0.0f, _graphic_id[ res ], TRUE );
	
	if ( blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
	
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

void Drawer::createGraph( int res, int width, int height ) {
	unloadGraph( res );
	assert( res < GRAPHIC_ID_NUM );
	_graphic_id[ res ] = MakeScreen( width, height, TRUE );
}

void Drawer::drawSpriteToGraph( int res, const Sprite& sprite ) {
	SetDrawScreen( _graphic_id[ res ] );

	switch ( sprite.blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * sprite.ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * sprite.ratio ) );
		break;
	}

	if ( sprite.trans.tw < 0 ) {
		DrawGraph( sprite.trans.sx, sprite.trans.sy, _graphic_id[ sprite.res ], TRUE );
	} else if ( sprite.trans.sy2 < 0 ) {
		DrawRectGraph( sprite.trans.sx, sprite.trans.sy, sprite.trans.tx, sprite.trans.ty, sprite.trans.tw, sprite.trans.th, _graphic_id[ sprite.res ], TRUE, FALSE );
	} else {
		DrawRectExtendGraph(
			sprite.trans.sx , sprite.trans.sy ,
			sprite.trans.sx2, sprite.trans.sy2,
			sprite.trans.tx , sprite.trans.ty ,
			sprite.trans.tw , sprite.trans.th , _graphic_id[ sprite.res ], TRUE );
	}

	if ( sprite.blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}

	SetDrawScreen( DX_SCREEN_BACK );
}

void Drawer::clearToGraph( int res ) {
	SetDrawScreen( _graphic_id[ res ] );
	ClearDrawScreen( );
	SetDrawScreen( DX_SCREEN_BACK );

}



void Drawer::loadGraph( int res, const char * filename ) {
	unloadGraph( res );
	std::string path = _directory;
	path += "/";
	path +=  filename;
	assert( res < GRAPHIC_ID_NUM );
	_graphic_id[ res ] = LoadGraph( path.c_str( ) );
	if ( _graphic_id[ res ] < 0 ) {
		path = "../" + path;
		_graphic_id[ res ] = LoadGraph( path.c_str( ) );
		assert( _graphic_id[ res ] >= 0 );
	}
}

void Drawer::unloadGraph( int res ) {
	if ( _graphic_id[ res ] >= 0 ) {
		DeleteGraph( _graphic_id[ res ] );
	}
	_graphic_id[ res ] = -1;
}

void Drawer::unloadAllGraph( ) {
	for ( int i = 0; i < GRAPHIC_ID_NUM; i++ ) {
		if ( _graphic_id[ i ] >= 0 ) {
			DeleteGraph( _graphic_id[ i ] );
		}
		_graphic_id[ i ] = -1;
	}
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

int Drawer::getTextureHeight( const int res ) const {
	int height = -1;
	int width = -1;
	GetGraphSize( _graphic_id[ res ], &width, &height );
	return height;
}

int Drawer::getTextureWidth( const int res ) const {
	int height = -1;
	int width = -1;
	GetGraphSize( _graphic_id[ res ], &width, &height );
	return width;
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
