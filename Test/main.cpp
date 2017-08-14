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
		drawer->loadEffect( 0, "laser.efk" );
		drawer->resetFPS( );
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		// •`‰æ
		drawer->flip( );
		if ( drawer->isOverFPS( ) ) {
			drawer->skipFlipping( );
		} else {
			_image->setPos( ( int )_image_pos.x, ( int )_image_pos.y );
			for ( int i = 0; i < 1; i++ ) {
				_image->draw( );
			}
			drawer->drawEffect( 0, Vector( ), 1.0 );
		}
		
		// ˆ—
		DevicePtr device = Device::getTask( );
		Vector stick( device->getDirX( ), device->getDirY( ) );
		_image_pos += stick * 0.1;
		
		
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