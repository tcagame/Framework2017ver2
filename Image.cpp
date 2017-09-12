#include "Image.h"
#include "DxLib.h"

Image::Image( ) :
_sx1( 0 ),
_sy1( 0 ),
_tx( -1 ),
_ty( -1 ),
_tw( -1 ),
_th( -1 ),
_sx2( -1 ),
_sy2( -1 ),
_blend( BLEND_NONE ),
_ratio( 1.0 ) {
}


Image::~Image( ) {
	if ( _handle >= 0 ) {
		DeleteGraph( _handle );
	}
}

bool Image::load( std::string path ) {
	_handle = LoadGraph( path.c_str( ) );
	if ( _handle < 0 ) {
		return false;
	}
	return true;
}

void Image::draw( ) const{
	switch ( _blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * _ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * _ratio ) );
		break;
	}

	if ( _tw < 0 ) {
		DrawGraph( _sx1, _sy1, _handle, TRUE );
	} else if ( _sy2 < 0 ) {
		DrawRectGraph( _sx1, _sy1, _tx, _ty, _tw, _th, _handle, TRUE, FALSE );
	} else {
		DrawRectExtendGraph(
			_sx1, _sy1,
			_sx2, _sy2,
			_tx , _ty ,
			_tw , _th , _handle, TRUE );
	}

	if ( _blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}

void Image::setRect( int tx, int ty, int tw, int th ) {
	_tx = tx;
	_ty = ty;
	_tw = tw;
	_th = th;
}

void Image::setPos( const int sx1, const int sy1, const int sx2, const int sy2 ) {
	_sx1 = sx1;
	_sy1 = sy1;
	_sx2 = sx2;
	_sy2 = sy2;
}


void Image::getImageSize( int& width, int& height ) const {
	GetGraphSize( _handle, &width, &height );
}

int Image::getHandle( ) const {
	return _handle;
}

void Image::setHandle( int handle ) {
	_handle = handle;
}

void Image::createGraph( int width, int height ) {
	_handle = MakeScreen( width, height, TRUE );
}

void Image::drawSpriteToGraph( ImagePtr image ) {
	SetDrawScreen( _handle );

	switch ( image->_blend ) {
	case BLEND_ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( 255 * image->_ratio ) );
		break;
	case BLEND_ADD:
		SetDrawBlendMode( DX_BLENDMODE_ADD  , ( int )( 255 * image->_ratio ) );
		break;
	}

	if ( image->_tw < 0 ) {
		DrawGraph( image->_sx1, image->_sy1, _handle, TRUE );
	} else if ( image->_sy2 < 0 ) {
		DrawRectGraph( image->_sx1, image->_sy1, image->_tx, image->_ty, image->_tw, image->_th, image->_handle, TRUE, FALSE );
	} else {
		DrawRectExtendGraph( image->_sx1, image->_sy1, image->_sx2, image->_sy2, image->_tx, image->_ty, image->_tw, image->_th, image->_handle, TRUE );
	}

	if ( image->_blend != BLEND_NONE ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}

	SetDrawScreen( DX_SCREEN_BACK );
}

void Image::clearToGraph( ) {
	SetDrawScreen( _handle );
	ClearDrawScreen( );
	SetDrawScreen( DX_SCREEN_BACK );
}
