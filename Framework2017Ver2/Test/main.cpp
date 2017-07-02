#include "Application.h"
#include "Drawer.h"
#include "Device.h"

class Test : public Task {
public:
	static std::string getTag( ) { return "TEST"; }
	Test( ) {
	}

	virtual ~Test( ) {

	}


	void initialize( ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->loadGraph( 0, "sample.png" );
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		DevicePtr device = Device::getTask( );
		Vector stick( device->getDirX( ), device->getDirY( ) );
		image_pos += stick;
		Drawer::Transform trans( (int)image_pos.x, (int)image_pos.y );
		drawer->drawSprite( Drawer::Sprite( trans, 0 ) );
		drawer->flip( );
	}
private:
	Vector image_pos;
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