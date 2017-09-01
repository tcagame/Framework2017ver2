#include "Effect.h"
#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include <assert.h>

const int PARTICLE = 2000; //Effekseerの最大パーティクル数

EffectPtr Effect::getTask( ) {
	return std::dynamic_pointer_cast< Effect >( Application::getInstance( )->getTask( getTag( ) ) );
}

Effect::Effec::Effec( ) :
id( -1 ),
handle( -1 ),
size( -1 ),
pos( Vector( ) ),
rotate( Vector( ) ) {
}

Effect::Effec::Effec( int id_, const Vector& pos_, double size_, const Vector& rotate_ ) :
id( id_),
pos( pos_ ),
size( size_ ),
rotate( rotate_ ),
handle( -1 ) {
}

Effect::Effect( const char * directory ) :
_directory( directory ) {
}

Effect::~Effect( ) {
}

void Effect::initialize( ) {
	// DirectX9を使用するようにする。
	// Effekseerを使用するには必ず設定する。
	//SetUseDirect3DVersion(DX_DIRECT3D_9);


	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	//if ( DxLib_Init( ) == -1 ) {
	//	return;
	//}
	if ( Effkseer_Init( PARTICLE ) == -1 ) {
		return;
	}

		
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();


	std::string path = _directory;
	path += "/";
	path += "effekseer_fix.png";
	_effekseer_graph = LoadGraph( path.c_str( ) );
	assert( _effekseer_graph >= 0 );
	DrawGraph( 0, 0, _effekseer_graph, TRUE );
}

void Effect::finalize( ) {
}

void Effect::update( ) {
}

int Effect::loadEffect( const char * filename ) const {
	std::string path = _directory;
	path += "/";
	path +=  filename;
	int result = LoadEffekseerEffect( path.c_str( ) );
	if ( result < 0 ) {
		path = "../" + path;
		result = LoadEffekseerEffect( path.c_str( ) );
		assert( result >= 0 );
	}
	return result;
}

int Effect::playEffect( const int effect_handle ) {
	return PlayEffekseer3DEffect( effect_handle );
}

void Effect::updateEffectTransform( const int effect_handle, const Vector& pos, const double size, const Vector& rotate ) const {
	SetScalePlayingEffekseer3DEffect( effect_handle,
		( float )size, ( float )size, ( float )size );
	SetRotationPlayingEffekseer3DEffect( effect_handle,
		( float )rotate.x, ( float )rotate.y, ( float )rotate.z );
	SetPosPlayingEffekseer3DEffect( effect_handle,
		( float )pos.x, ( float )pos.y, ( float )pos.z );
}

void Effect::drawEffect( ) const {
	DrawGraph( 0, 0, _effekseer_graph, TRUE );
	RenderVertex( );

	// EffekseerをDxLibのカメラと同期させる
	Effekseer_Sync3DSetting( );

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D( );

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D( );
}

void Effect::stopEffect( int effect_handle ) const {
	StopEffekseer3DEffect( effect_handle );
}
