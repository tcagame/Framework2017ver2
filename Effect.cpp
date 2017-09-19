#include "Effect.h"
#include "Application.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include <assert.h>

const int PARTICLE = 3000; //Effekseer�̍ő�p�[�e�B�N����

EffectPtr Effect::getTask( ) {
	return std::dynamic_pointer_cast< Effect >( Application::getInstance( )->getTask( getTag( ) ) );
}

Effect::Effect( const char * directory ) :
_handle_num( 0 ),
_directory( directory ) {
	// DirectX9���g�p����悤�ɂ���B
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	//SetUseDirect3DVersion(DX_DIRECT3D_9);


	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if ( Effkseer_Init( PARTICLE ) == -1 ) {
		return;
	}

		
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();


	std::string path = _directory;
	path += "/";
	path += "effekseer_fix.png";
	_effekseer_graph = LoadGraph( path.c_str( ) );
	assert( _effekseer_graph >= 0 );
	DrawGraph( 0, 0, _effekseer_graph, TRUE );

	for ( int i = 0; i < MAX_EFFECT_NUM; i++ ) {
		_handles[ i ] = -1;
	}
}

Effect::~Effect( ) {
}

void Effect::initialize( ) {
}

void Effect::finalize( ) {
	for ( int i = 0; i < _handle_num; i++ ) {
		DeleteEffekseerEffect( _handles[ _handle_num ] );
	}
	Effkseer_End( );
}

void Effect::update( ) {
}

int Effect::loadEffect( const char * filename ) {
	std::string path = _directory;
	path += "/";
	path +=  filename;
	int result = LoadEffekseerEffect( path.c_str( ) );
	if ( result < 0 ) {
		path = "../" + path;
		result = LoadEffekseerEffect( path.c_str( ) );
		assert( result >= 0 );
	}
	_handles[ _handle_num ] = result;
	_handle_num = ( _handle_num + 1 ) % MAX_EFFECT_NUM;
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

	// Effekseer��DxLib�̃J�����Ɠ���������
	Effekseer_Sync3DSetting( );

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D( );

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D( );
}

void Effect::stopEffect( int effect_handle ) const {
	StopEffekseer3DEffect( effect_handle );
}
