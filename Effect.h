#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>
#include "mathmatics.h"

PTR( Effect );

class Effect : public Task {
public:
	static std::string getTag( ) { return "EFFECT"; };
	static EffectPtr getTask( );
public:
	Effect( const char * directory );
	virtual ~Effect( );
public:
	struct Effec {
		int id;
		int handle;
		Vector pos;
		Vector rotate;
		double size;
		Effec( );
		Effec( int id_, const Vector& pos_, double size_ = 1.0, const Vector& rotate_ = Vector( ) );
	};
public:
	void update( );
	void initialize( );
	void finalize( );
	int loadEffect( int effect_id, const char * filename ) const; // ���[�h�����G�t�F�N�g��id��Ԃ�
	int playEffect( const int effect_handle ); // �Đ������G�t�F�N�g��handle��Ԃ�
	void updateEffectTransform( const int effect_handle, const Vector& pos, const double size = 1.0, const Vector& rotate = Vector( ) ) const; // �Đ����̃G�t�F�N�g��trans��ύX
	void drawEffect( ) const;
private:
	const char * _directory;
};

