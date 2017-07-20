#include "Application.h"
#include "Drawer.h"
#include "ModelMDL.h"
#include "ModelMV1.h"
#include "Device.h"
#include "Image.h"

class Test : public Task {
public:
	static std::string getTag( ) { return "TEST"; }
	Test( ) {
	}

	virtual ~Test( ) {

	}


	void initialize( ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setCameraUp( Vector( 0, 0, 1 ) );
		drawer->setCamera( Vector( 0, -30, 30 ), Vector( ) );
		_image = drawer->createImage( "sample.png" );
		_image->setRect( 20, 20, 60, 40 ); // tx ty tw th
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		DevicePtr device = Device::getTask( );
		Vector stick( device->getDirX( ), device->getDirY( ) );
		_image_pos += stick;
		
		_image->setPos( _image_pos );
		_image->draw( );
		drawer->flip( );
	}
private:
	Vector _image_pos;
	int _count;
	ImagePtr _image;
};

void main( ) {
	TaskPtr test = TaskPtr( new Test( ) );
	TaskPtr drawer = TaskPtr( new Drawer( "Resource" ) );
	TaskPtr device = TaskPtr( new Device( ) );

	ApplicationPtr app = Application::getInstance( );
	app->addTask( Test::getTag( ), test );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Device::getTag( ), device );
}