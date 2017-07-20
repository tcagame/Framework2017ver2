#pragma once
#include "Image.h"

class ImageTarget: public Image {
public:
	ImageTarget( );
	virtual ~ImageTarget( );
public:
	void create( int width, int height );
};

