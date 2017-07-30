#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include <array>

PTR( Server );
PTR( Data );

class Server : public Task {
public:
	static std::string getTag( ) { return "SERVER"; }
	static ServerPtr getTask( );
public:
	static const int MAX_MACHINES = 10;
public:
	Server( DataPtr data );
	virtual ~Server( );
public:
	virtual void update( );
	void saveIP( );
	std::string getMachineIPStr( int index );
	void sendUdp( DataPtr data );
	bool isRecieving( ) const;
private:
	void listenForAcceptNewClient( );
	void listenForPackets( );
	void lostMachine( );
	void recieveTcp( );
	void sendCondition( );
private:
	std::array< int, MAX_MACHINES > _machine;
	int _udp_handle;
	DataPtr _data;
	int _target_michine;
	bool _recieving;
	int _condition_time;
};