#include "Application.h"
#include "Drawer.h"
#include "ModelMDL.h"
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
		drawer->setCameraUp( Vector( 0, 0, 1 ) );
		drawer->setCamera( Vector( 0, -30, 30 ), Vector( ) );
		drawer->loadGraph( 0, "sample.png" );

		_model_mdl = ModelMDLPtr( new ModelMDL );
		_model_mdl->load( "../Test/Resource/sample.mdl" );
		_model_mdl->setTexture( "../Test/Resource/texture.jpg" );
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		DevicePtr device = Device::getTask( );
		Vector stick( device->getDirX( ), device->getDirY( ) );
		_image_pos += stick;
		Drawer::Transform trans( (int)_image_pos.x, (int)_image_pos.y );
		_model_mdl->multiply( Matrix::makeTransformTranslation( stick * 0.002 ) );
		_model_mdl->draw( );

		drawer->drawSprite( Drawer::Sprite( trans, 0 ) );
		drawer->flip( );
	}
private:
	Vector _image_pos;
	int _count;
	ModelMDLPtr _model_mdl;
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