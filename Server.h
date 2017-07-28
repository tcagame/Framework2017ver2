#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include "Network.h"
#include <array>

PTR( Server );

class Server : public Task {
public:
	static std::string getTag( ) { return "SERVER"; }
	static ServerPtr getTask( );
public:
	Server( );
	virtual ~Server( );
public:
	virtual void update( );
	void saveIP( );
	std::string getMachineIPStr( int index );
	void sendClientData( void* data, int size );
	void* getData( ) const;
private:
	void listenForAcceptNewClient( );
	void listenForPackets( );
	void sendCondition( );
	void executeNetData( const SERVERDATA& data );
private:
	std::array< int, MAX_MACHINE > _machine;
	int _udp_handle;
	int _network_state_time;
	int _send_network_condition_num;
	void* _data;
	unsigned int _get_boss_data;
};