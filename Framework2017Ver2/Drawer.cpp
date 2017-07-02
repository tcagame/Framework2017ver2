#include "Drawer.h"
#include "Application.h"
#include "Model.h"
#include "DxLib.h"

#if EFFECKSEER
	// EffekseerForDXLib.hをインクルードします。
#	include "EffekseerForDXLib.h"
#endif

#include <assert.h>

static const int REFRESH_COUNT = 60;	//平均を取るサンプル数
static const int FPS = 30;
static const double SHADOW_RADIUS = 0.5;

const char* SHADOW_TEXTURE_PATH = "../Resource/Images/Shadow/shadow.png";

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

Drawer::ModelMV1::ModelMV1( ) :
mesh( -1 ),
anime( -1 ),
time( 0 ){
}

Drawer::ModelMV1::ModelMV1( Matrix matrix_, int mesh_, int anime_, double time_ ) :
matrix( matrix_ ),
mesh( mesh_ ),
anime( anime_ ),
time( time_ ) {
}

Drawer::ModelMDL::ModelMDL( ) :
type( -1 ){
}

Drawer::ModelMDL::ModelMDL( Vector pos_, int type_ ) :
pos( pos_ ),
type( type_ ) {

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

Drawer::Billboard::Billboard( ) :
size( 0 ),
res( 0 ),
blend( BLEND_NONE ),
ratio( 0.0 ) {

}

Drawer::Billboard::Billboard( Vector pos_, double size_, int res_, BLEND blend_, double ratio_ ) :
pos( pos_ ),
size( size_ ),
res( res_ ),
blend( blend_ ),
ratio( ratio_ ) {

}

Drawer::Effect::Effect( ) :
id( -1 ),
handle( -1 ) {
}

Drawer::Effect::Effect( int id_, const Vector& pos_, double size_, const Vector& rotate_ ) :
id( id_),
pos( pos_ ),
size( size_ ),
rotate( rotate_ ),
handle( -1 ) {

}

Drawer::Circle::Circle( ) {

}

Drawer::Circle::Circle( const Vector& pos_, const double radius_ ) :
pos( pos_ ),
radius( radius_ ) {

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

	for ( int i = 0; i < MODEL_ID_NUM; i++ ) {
		_mv1_id[ i ] = -1;
	}

	_sprite_idx = 0;
	_model_mv1_idx = 0;
	_billboard_idx = 0;

	_refresh_count = 0;
	_fps = FPS;
	_start_time = 0;
#if EFFECKSEER
	_effekseer_fix_graph = LoadGraph( "../Resource/effecksser_fix.png" );
#endif
}

void Drawer::update( ) {
	
	flip( );

	drawModelMV1( );
	drawModelMDL( );
	drawModelSelf( );
	drawBillboard( );
	drawEffect( );
	drawSprite( );
	drawCircle( );
}

void Drawer::drawModelMDL( ) {
	for ( int i = 0; i < _model_mdl_idx; i++ ) {
		int type = _model_mdl[ i ].type;
		Vector pos = _model_mdl[ i ].pos;
		_model[ type ]->setPos( pos );
		_model[ type ]->draw( );
	}
	_model_mdl_idx = 0;
}

void Drawer::drawModelSelf( ) {
	for ( int i = 0; i < _model_self_idx; i++ ) {
		if ( !_model_self[ i ].z_buffer ) {
			SetWriteZBuffer3D( FALSE );
		}
		if ( _model_self[ i ].add ) {
			SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
		}

		_model_self[ i ].model->draw( _graphic_id[ _model_self[ i ].graph ], true );
		_model_self[ i ].model.reset( );
		
		if ( _model_self[ i ].add ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
		if ( !_model_self[ i ].z_buffer ) {
			SetWriteZBuffer3D( TRUE );
		}
	}
	_model_self_idx = 0;
}

void Drawer::drawModelMV1( ) {
	for ( int i = 0; i < _model_mv1_idx; i++ ) {
		int mesh = _mv1_id[ _model_mv1[ i ].mesh ];
		int anim = _mv1_id[ _model_mv1[ i ].anime ];
		double time = _model_mv1[ i ].time;
		Matrix mat = _model_mv1[ i ].matrix;
		MATRIX matrix = MGetIdent( );
		for( int j = 0; j < 3 * 3; j++ ) {
			int n = j % 3;
			int m = j / 3;
			matrix.m[ m ][ n ] = ( float )mat.data[ n ][ m ];
		}
		for ( int j = 0; j < 4; j++ ) {
			matrix.m[ j ][ 3 ] = ( float )mat.data[ j ][ 3 ];
			matrix.m[ 3 ][ j ] = ( float )mat.data[ 3 ][ j ];
		}

		MV1SetMatrix( mesh, matrix );
		//アニメーション設定
		int idx = MV1AttachAnim( mesh, 0, anim, TRUE );
		MV1SetAttachAnimTime( mesh, idx, ( float )time );
		// ３Ｄモデルの描画
		MV1DrawModel( mesh );
		MV1DetachAnim( mesh, idx );
	}
	_model_mv1_idx = 0;
}

void Drawer::drawSprite( ) {

	for ( int i = 0; i < _sprite_idx; i++ ) {
		const Sprite& sprite = _sprite[ i ];

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
	_sprite_idx = 0;
}


void Drawer::drawCircle( ) {

	for ( int i = 0; i < _circle_idx; i++ ) {
		const Circle& circle = _circle[ i ];

		DrawCircle( ( int )circle.pos.x, ( int )circle.pos.y, ( int )circle.radius, GetColor( 255, 0, 0 ), 0, 3 );
	}
	_circle_idx = 0;
}

void Drawer::drawBillboard( ) {
	for ( int i = 0; i < _billboard_idx; i++ ) {
		const Billboard& billboard = _billboard[ i ];
		switch( billboard.blend ) {
		case BLEND_ALPHA:
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * billboard.ratio ) );
			break;
		case BLEND_ADD:
			SetDrawBlendMode( DX_BLENDMODE_ADD, ( int )( 255 * billboard.ratio ) );
			break;
		}

		int cheak = DrawBillboard3D( VGet( ( float )billboard.pos.x, ( float )billboard.pos.y, ( float )billboard.pos.z ), 0.5f, 0.5f, ( float )billboard.size, 0.0f, _graphic_id[ billboard.res ], TRUE );
		
		if ( billboard.blend != BLEND_NONE ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
	_billboard_idx = 0;
}

void Drawer::drawEffect( ) {
# if EFFECKSEER
		DrawGraph( 0, 0, _effekseer_fix_graph, TRUE );
		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D( );

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D( );
#	endif
}

void Drawer::loadMV1Model( int motion, const char* filename ) {
	std::string path = _directory;
	path += "/";
	path += filename;
	assert( motion < MODEL_ID_NUM );
	int& id = _mv1_id[ motion ];
	id = MV1LoadModel( path.c_str( ) );
	assert( id > 0 );
	int num = MV1GetMaterialNum( id ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( id, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}
}

void Drawer::loadMDLModel( int type, const char* model_filename, const char* texture_filename, Matrix matrix ) {
	assert( type < MODEL_NUM );
	std::string path = _directory;
	path += "/";
	std::string tex_path = path;
	path += model_filename;
	tex_path += texture_filename;
	
	_model[ type ] = ModelPtr( new Model );
	_model[ type ]->load( path.c_str( ) );
	_model[ type ]->setTexture( tex_path.c_str( ) );
	_model[ type ]->multiply( matrix );
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

void Drawer::setSprite( const Sprite& sprite ) {
	assert( _sprite_idx < SPRITE_NUM );
	_sprite[ _sprite_idx ] = sprite;
	_sprite_idx++;
}

void Drawer::setModelMV1( const ModelMV1& model ) {
	assert( _model_mv1_idx < MODEL_MV1_NUM );
	_model_mv1[ _model_mv1_idx ] = model;
	_model_mv1_idx++;
}

void Drawer::setModelMDL( const ModelMDL& model_mdl ) {
	assert( _model_mdl_idx < MODEL_MDL_NUM );
	_model_mdl[ _model_mdl_idx ] = model_mdl;
	_model_mdl_idx++;
}

void Drawer::setModelSelf( const ModelSelf& model_self ) {
	assert( _model_self_idx < MODEL_SELF_NUM );
	_model_self[ _model_self_idx ] = model_self;
	_model_self_idx++;
}

void Drawer::setBillboard( const Billboard& billboard ) {
	assert( _billboard_idx < GRAPHIC_ID_NUM );
	_billboard[ _billboard_idx ] = billboard;
	_billboard_idx++;
}

void Drawer::setEffectPos( const Effect& effect ) {
#	if EFFECKSEER
		if ( effect.handle >= 0 ) { 
			SetPosPlayingEffekseer3DEffect( effect.handle,
				( float )effect.pos.x, ( float )effect.pos.y, ( float )effect.pos.z );
		}
#	endif
}

void Drawer::setEffect( Effect& effect ) {
#	if EFFECKSEER
		effect.handle = PlayEffekseer3DEffect( effect.id );
		float size = ( float )effect.size;
		SetScalePlayingEffekseer3DEffect( effect.handle,
			size, size, size );
		SetRotationPlayingEffekseer3DEffect( effect.handle,
			( float )effect.rotate.x, ( float )effect.rotate.y, ( float )effect.rotate.z );
		SetPosPlayingEffekseer3DEffect( effect.handle,
			( float )effect.pos.x, ( float )effect.pos.y, ( float )effect.pos.z );
#	endif
}

void Drawer::setCircle( const Circle& circle ) {
	assert( _circle_idx < CIRCLE_NUM );
	_circle[ _circle_idx ] = circle;
	_circle_idx++;
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

double Drawer::getEndAnimTime( int anime ) {
	return MV1GetAnimTotalTime( _mv1_id[ anime ], 0 );
}

void Drawer::drawLine( int x1, int y1, int x2, int y2 ) {
	DrawLine( x1, y1, x2, y2, 0xFFFFFF ) ;
}

void Drawer::drawLine( const Vector& pos1, const Vector& pos2 ) {
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

void Drawer::drawString( int x, int y, const char* string, ... ) {
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
