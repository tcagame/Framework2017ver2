#include "ModelMV1.h"
#include "DxLib.h"
#include <assert.h>


ModelMV1::ModelMV1( ) :
	_mesh( -1 ),
	_anim( -1 ),
	_time( 0 ) {
}


ModelMV1::~ModelMV1( ) {
}

void ModelMV1::load( const char* filename ) {
	_mesh = MV1LoadModel( filename );
	
	assert( _mesh > 0 );
	int num = MV1GetMaterialNum( _mesh ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( _mesh, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}
}

void ModelMV1::setTexture( const char* filename, bool semi_trans ) {
	int graph = MV1LoadTexture( filename );
	if ( semi_trans ) {
		MV1SetTextureGraphHandle( _mesh, 0, graph, TRUE );
	} else {
		MV1SetTextureGraphHandle( _mesh, 0, graph, FALSE );
	}
}


void ModelMV1::draw( ) const {
	Matrix mat = _rot_mat.multiply( _scale_mat ).multiply( _trans_mat );
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
	MV1SetMatrix( _mesh, matrix );
	//�A�j���[�V�����ݒ�
	int idx = MV1AttachAnim( _mesh, 0, _anim, TRUE );
	MV1SetAttachAnimTime( _mesh, idx, ( float )_time );
	// �R�c���f���̕`��
	MV1DrawModel( _mesh );
	MV1DetachAnim( _mesh, idx );
}

void ModelMV1::transferTransform( const Matrix& trans_mat ) {
	_trans_mat = _trans_mat.multiply( trans_mat );
}

void ModelMV1::rotateTransform( const Matrix& rot_mat ) {
	_rot_mat = _rot_mat.multiply( rot_mat );
}

void ModelMV1::scaleTransform( const Matrix& scale_mat ) {
	_rot_mat = _rot_mat.multiply( scale_mat );
}

double ModelMV1::getEndAnimTime( ) const {
	return MV1GetAnimTotalTime( _anim, 0 );
}

void ModelMV1::reset( ) {
	MV1DeleteModel( _mesh );
	_mesh = -1;
	_anim = -1;
	_time = 0;
}