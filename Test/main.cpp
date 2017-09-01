#include "Application.h"
#include "Drawer.h"
#include "ModelMDL.h"
#include "ModelMV1.h"
#include "Device.h"
#include "Image.h"
#include "Effect.h"

class Test : public Task {
public:
	static std::string getTag( ) { return "TEST"; }
	Test( ) {
	}

	virtual ~Test( ) {

	}
	
	void initialize( ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setCameraUp( Vector( 1, 1, 1 ) );
		drawer->setCamera( Vector( 100, 200, 50 ), Vector( 1,1,1 ) );
		drawer->resetFPS( );
		EffectPtr effect = Effect::getTask( );
		_effect_handle = effect->loadEffect( "laser.efk" );
		effect->playEffect( _effect_handle );
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		EffectPtr effect = Effect::getTask( );
		// •`‰æ
		effect->updateEffectTransform( _effect_handle, Vector( ) );
		drawer->waitForSync( );
		drawer->flip( );
		effect->drawEffect( );
	}
private:
	int _effect_handle;
};

void main( ) {
	TaskPtr drawer = TaskPtr( new Drawer( "Resource" ) );
	TaskPtr effect = TaskPtr( new Effect( "Resource" ) );
	TaskPtr test = TaskPtr( new Test( ) );

	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Effect::getTag( ), effect );
	app->addTask( Test::getTag( ), test );
}