#pragma once

#include "Task.h"
#include "Network.h"
#include <array>

const int MAX_JOYPAD_USE_NUM = 16;

PTR( Device );

class Device : public Task {
public:
	static std::string getTag( ) { return "DEVICE"; }
	static DevicePtr getTask( );
public:
	Device( );
	virtual ~Device( );
public:
	void initialize( );
	virtual void update( );
public:
	int getDeviceNum( ) const;
	char getDirX( int idx = 0 ) const;
	char getDirY( int idx = 0 ) const;
	char getRightDirX( int idx = 0 ) const;
	char getRightDirY( int idx = 0 ) const;
	
	unsigned char getButton( int idx = 0 ) const;
	unsigned char getPush( int idx = 0 ) const;
public:
	void resetup( );
private:
	struct DATA {
		char x;
		char y;
		char rx;
		char ry;
		unsigned char button;
		unsigned char push;
	};
private:
	void updateNoJoypad();
	void updateJoypad();
private:
	std::array< DATA, MAX_JOYPAD_USE_NUM > _data;
	int _num;
};