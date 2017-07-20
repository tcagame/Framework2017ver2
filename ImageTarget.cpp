#include "ImageTarget.h"
#include "DxLib.h"


ImageTarget::ImageTarget( ) {
}


ImageTarget::~ImageTarget( ) {
}

void ImageTarget::create( int width, int height ) {
	setHandle( MakeScreen( width, height, TRUE ) );
}