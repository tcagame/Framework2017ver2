#include "Model.h"
#include "dxlib.h"

class ModelImpl {
public:
	VERTEX3D *_vertex;
	unsigned int _polygon_num;
};

Model::Model( ) {
	_impl = ModelImplPtr( new ModelImpl );

	_impl->_vertex = NULL;
	_impl->_polygon_num = 0;
	_texture_handle = -1;
}

Model::~Model( ) {
	if ( _impl->_vertex ) {
		delete [] _impl->_vertex;
	}
}

void Model::alloc( int polygon_num ) {
	_impl->_polygon_num = polygon_num;
	if ( _impl->_vertex != NULL ) {
		delete [] _impl->_vertex;
	}
	_impl->_vertex = new VERTEX3D[ polygon_num * 3 ];
}

void Model::setPolygonNum( int num ) {
	_impl->_polygon_num = num;
}

void Model::draw( int texture, bool trans ) const {

	int check = DrawPolygon3D( _impl->_vertex, _impl->_polygon_num, texture, trans ? TRUE : FALSE );
		
}


void Model::draw( bool trans ) const {
	int check = DrawPolygon3D( _impl->_vertex, _impl->_polygon_num, _texture_handle, trans ? TRUE : FALSE );		
}


void Model::set( int n, VERTEX vertex ) {

	VERTEX3D vtx;
	vtx.pos = VGet( ( float )vertex.pos.x, ( float )vertex.pos.y, ( float )vertex.pos.z );
	vtx.norm = VGet( 0, 0, 0 );
	vtx.dif  = GetColorU8( 255, 255, 255, 255 );
	vtx.spc  = GetColorU8( 0, 0, 0, 0 );
	vtx.u    = ( float )vertex.u;
	vtx.v    = ( float )vertex.v;
	vtx.su   = 0.0f;
	vtx.sv   = 0.0f;

	_impl->_vertex[ n ] = vtx;
}

Model::VERTEX Model::getVERTEX( int idx ){
	VERTEX vertex;
	vertex.pos = Vector( _impl->_vertex[ idx ].pos.x, _impl->_vertex[ idx ].pos.y, _impl->_vertex[ idx ].pos.z );
	vertex.u = _impl->_vertex[ idx ].u;
	vertex.v = _impl->_vertex[ idx ].v;
	return vertex;
}


bool Model::load( std::string filename ) {
	int fh = FileRead_open( filename.c_str( ) );
	if ( fh <= 0 ) {
		return false;
	}

	unsigned int polygon_num = 0;
	FileRead_read( &polygon_num, sizeof( unsigned int ), fh );
	
	alloc( polygon_num );

	FileRead_read( _impl->_vertex, sizeof( DxLib::VERTEX3D ) * ( polygon_num * 3 ), fh );
	_origin_pos.x = _impl->_vertex[ 0 ].pos.x;
	_origin_pos.y = _impl->_vertex[ 0 ].pos.y;
	_origin_pos.z = _impl->_vertex[ 0 ].pos.z;

	FileRead_close( fh );
	return true;
}

void Model::translate( Vector move ) {
	for ( int i = 0; i < ( int )_impl->_polygon_num * 3; i++ ) {
		_impl->_vertex[ i ].pos.x += ( float )move.x;
		_impl->_vertex[ i ].pos.y += ( float )move.y;
		_impl->_vertex[ i ].pos.z += ( float )move.z;
	}
}

void Model::setPos( Vector pos ) {
	VECTOR diff;
	diff.x = _impl->_vertex[ 0 ].pos.x - ( float )_origin_pos.x;
	diff.y = _impl->_vertex[ 0 ].pos.y - ( float )_origin_pos.y;
	diff.z = _impl->_vertex[ 0 ].pos.z - ( float )_origin_pos.z;
	
	VECTOR move;
	move.x = ( float )pos.x - diff.x;
	move.y = ( float )pos.y - diff.y;
	move.z = ( float )pos.z - diff.z;

	for ( int i = 0; i < ( int )_impl->_polygon_num * 3; i++ ) {
		_impl->_vertex[ i ].pos.x += move.x;
		_impl->_vertex[ i ].pos.y += move.y;
		_impl->_vertex[ i ].pos.z += move.z;
	}
}

void Model::save( std::string filename ) {
	FILE *fp;
	errno_t err = fopen_s( &fp, filename.c_str( ), "wb" );
	if ( err != 0 ) {
		return;
	}
	
	fwrite( &_impl->_polygon_num, sizeof( unsigned int ), 1, fp );
	fwrite( _impl->_vertex, sizeof( DxLib::VERTEX3D ) * ( _impl->_polygon_num * 3 ), 1, fp );

	fclose( fp );
}

void Model::multiply( Matrix matrix ) {
	int count = ( int )_impl->_polygon_num * 3;
	for ( int i = 0; i < count; i++ ) {
		Vector pos(
			_impl->_vertex[ i ].pos.x,
			_impl->_vertex[ i ].pos.y,
			_impl->_vertex[ i ].pos.z );
		pos = matrix.multiply( pos );
		_impl->_vertex[ i ].pos = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );
	}
	_origin_pos.x = _impl->_vertex[ 0 ].pos.x;
	_origin_pos.y = _impl->_vertex[ 0 ].pos.y;
	_origin_pos.z = _impl->_vertex[ 0 ].pos.z;
}

void Model::mergeModel( ModelConstPtr model ) {
	if ( model == NULL ) {
		return;
	}

	ModelImplConstPtr merge_impl = model->getModelImpl( );
	int polygon_num = _impl->_polygon_num + merge_impl->_polygon_num;
	VERTEX3D* vertex = new VERTEX3D[ sizeof( VERTEX3D ) * polygon_num * 3 ];

	// this のモデルをコピー
	for ( int i = 0; i < ( int )_impl->_polygon_num * 3; i++ ) {
		vertex[ i ] = _impl->_vertex[ i ];
	}
	// modelのモデルをコピー
	for ( int i = 0; i < ( int )merge_impl->_polygon_num * 3; i++ ) {
		int idx = _impl->_polygon_num * 3 + i;
		vertex[ idx ] = merge_impl->_vertex[ i ];
	}

	if ( _impl->_vertex ) {
		delete [] _impl->_vertex;
	}
	_impl->_vertex = vertex;
	_impl->_polygon_num = polygon_num;
}

ModelImplConstPtr Model::getModelImpl( ) const {
	return _impl;
}

Vector Model::getPos( ) const {
	Vector pos;
	pos.x = _impl->_vertex[ 0 ].pos.x - _origin_pos.x;
	pos.y = _impl->_vertex[ 0 ].pos.y - _origin_pos.y;
	pos.z = _impl->_vertex[ 0 ].pos.z - _origin_pos.z;
	return pos;
}

Vector Model::getPoint( int idx ) const {
	return Vector(
		_impl->_vertex[ idx ].pos.x,
		_impl->_vertex[ idx ].pos.y,
		_impl->_vertex[ idx ].pos.z );
}

int Model::getPolygonNum( ) const {
	return _impl->_polygon_num;
}

int Model::getTextureHandle( const char* filename ) {
	return LoadGraph( filename );
}

void Model::reset( ) {
	_impl = ModelImplPtr( new ModelImpl );

	_impl->_vertex = NULL;
	_impl->_polygon_num = 0;
}

void Model::setTexture( const char* filename ) {
	_texture_handle = getTextureHandle( filename );
}
