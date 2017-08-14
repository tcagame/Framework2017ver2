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
		drawer->setCameraUp( Vector( 0, 2, 0 ) );
		drawer->setCamera( Vector( 30, 130, 30 ), Vector( ) );
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
			drawer->drawEffect( 0, Vector( ), 1.0 );
		}		
	}
private:
};

void main( ) {
	TaskPtr test = TaskPtr( new Test( ) );
	TaskPtr drawer = TaskPtr( new Drawer( "Resource" ) );

	ApplicationPtr app = Application::getInstance( );
	app->addTask( Test::getTag( ), test );
	app->addTask( Drawer::getTag( ), drawer );
}