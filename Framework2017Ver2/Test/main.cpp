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
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		DevicePtr device = Device::getTask( );

		drawer->drawString(0, 100 * 1, "LEFT  X : %d", device->getDirX());
		drawer->drawString(0, 100 * 2, "LEFT  Y : %d", device->getDirY());
		drawer->drawString(0, 100 * 4, "RIGHT X : %d", device->getRightDirX());
		drawer->drawString(0, 100 * 5, "RIGHT Y : %d", device->getRightDirY());
	}
};

void main( ) {
	TaskPtr test = TaskPtr( new Test );
	TaskPtr drawer = TaskPtr( new Drawer( "Resource" ) );
	TaskPtr device = TaskPtr( new Device( ) );

	ApplicationPtr app = Application::getInstance( );
	app->addTask( Test::getTag( ), test );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Device::getTag( ), device );
}
