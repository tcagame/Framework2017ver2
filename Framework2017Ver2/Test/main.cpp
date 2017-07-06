#include "Application.h"
#include "Drawer.h"
#include "ModelMDL.h"
#include "ModelMV1.h"
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

		/*MDL
		_model_mdl = ModelMDLPtr( new ModelMDL );
		_model_mdl->load( "../Test/Resource/sample.mdl" );
		_model_mdl->setTexture( "../Test/Resource/mdl_texture.jpg" );
		_model_mdl->multiply( Matrix::makeTransformTranslation( Vector( 0, 0, 0 ) ) );
		*/

		_model_mv1 = ModelMV1Ptr( new ModelMV1 );
		_model_mv1->load( "../Test/Resource/sample.mv1" );
		_model_mv1->transferTransform( Matrix::makeTransformTranslation( Vector( 2, 1, 0 ) ) );
	}

	void update( ) {
		DrawerPtr drawer = Drawer::getTask( );
		DevicePtr device = Device::getTask( );
		Vector stick( device->getDirX( ), device->getDirY( ) );
		_image_pos += stick;
		Matrix trans_rot = Matrix::makeTransformTranslation( stick * 0.001 );
		_model_mv1->transferTransform( trans_rot );
		_model_mv1->rotateTransform( Matrix::makeTransformTranslation( stick * 0.001 ) );
		_model_mv1->draw( );
		/*MDL
		_model_mdl->multiply( Matrix::makeTransformTranslation( stick * 0.001 ) );
		_model_mdl->draw( );
		*/
		Drawer::Transform trans( (int)_image_pos.x, (int)_image_pos.y );
		drawer->drawSprite( Drawer::Sprite( trans, 0 ) );
		drawer->flip( );
	}
private:
	Vector _image_pos;
	int _count;
	//ModelMDLPtr _model_mdl;
	ModelMV1Ptr _model_mv1;
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