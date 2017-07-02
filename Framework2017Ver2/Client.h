#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include "Network.h"
#include <string>

PTR( Client );
PTR( Device );
PTR( Sound );
PTR( Inputter );

class Client : public Task {
public:
	static std::string getTag( ) { return "CLIENT"; }
	static ClientPtr getTask( );
public:
	Client( );
	virtual ~Client( );
public:
	virtual void update( );
public:
	std::string getPhase( ) const;
	void send( const SERVERDATA& data );
	CLIENTDATA getClientData( );
private:
	enum PHASE {
		PHASE_READY,
		PHASE_CONNECTING,
	};
private:
	void updateReady( );
	void updateConnecting( );
	void updateOffline( );
	bool load( );
	bool connect( );
	void recieveStatus( );
	void responseOfState( );
private:
	PHASE _phase;
	unsigned char _ip[ 4 ];
	int _tcp_handle;
	int _udp_handle;
	int _network_state_time;
	CLIENTDATA _status;
};

