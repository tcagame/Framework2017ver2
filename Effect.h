#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>
#include "mathmatics.h"
#include <array>

PTR( Effect );

class Effect : public Task {
public:
	static std::string getTag( ) { return "EFFECT"; };
	static EffectPtr getTask( );
public:
	Effect( const char * directory );
	virtual ~Effect( );
public:
	void update( );
	void initialize( );
	void finalize( );
	int loadEffect( const char * filename ); // ���[�h�����G�t�F�N�g��id��Ԃ�
	int playEffect( const int effect_handle ); // �Đ������G�t�F�N�g��handle��Ԃ�
	void updateEffectTransform( const int effect_handle, const Vector& pos, const double size = 1.0, const Vector& rotate = Vector( ) ) const; // �Đ����̃G�t�F�N�g��trans��ύX
	void drawEffect( ) const;
	void stopEffect( int effect_handle ) const;
private:
	const char * _directory;
	int _effekseer_graph;
	static const int MAX_EFFECT_NUM = 1000;
	std::array< int, MAX_EFFECT_NUM > _handles;
	int _handle_num;
};

