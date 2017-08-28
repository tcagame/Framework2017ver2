#include "Effect.h"
#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include <assert.h>

const int PARTICLE = 2000; //Effekseer�̍ő�p�[�e�B�N����

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
	// DirectX9���g�p����悤�ɂ���B
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	//SetUseDirect3DVersion(DX_DIRECT3D_9);
		
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions( );

	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	assert( DxLib_Init( ) == 0 );
	assert( Effkseer_Init( PARTICLE ) == 0 );

	std::string path = _directory;
	path += "/";
	path += "effekseer_fix.png";
	int effekseer_graph = LoadGraph( path.c_str( ) );
	assert( effekseer_graph >= 0 );
	DrawGraph( 0, 0, effekseer_graph, TRUE );
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
	Effekseer_Sync3DSetting( );
	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D( );

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D( );
}

void Effect::stopEffect( int effect_handle ) const {
	StopEffekseer3DEffect( effect_handle );
}
