#include "Movie.h"
#include "DxLib.h"

int WAIT_TIME = 17;

Movie::Movie( ) {
	_is_play = false;
}


Movie::~Movie( ) {
}

bool Movie::load( std::string path ) {
	_handle = LoadGraph( path.c_str( ) );
	if ( _handle < 0 ) {
		return false;
	}
	return true;
}

void Movie::draw( ) const {
	if ( _is_play ) {
		DrawGraph( 0, 0, _handle, FALSE );
	};
}

void Movie::stop( ) {
	_is_play = false;
	PauseMovieToGraph( _handle );
}

void Movie::play( bool is_loop ){
	if ( is_loop ) {
		PlayMovieToGraph( _handle, DX_PLAYTYPE_LOOP );
	} else {
		PlayMovieToGraph( _handle );
	}
	_is_play = true;
}

bool Movie::isPlay( ) const {
	return _is_play;
}
