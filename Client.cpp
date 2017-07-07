#include "Client.h"
#include "Application.h"
#include "DxLib.h"

ClientPtr Client::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Client >( fw->getTask( Client::getTag( ) ) );
}

Client::Client( ) {
	ApplicationPtr fw = Application::getInstance( );

	_phase = PHASE_READY;

	// Server IP 読み込み
	load( );

	_network_state_time = 0;
	_tcp_handle = -1;
	_udp_handle = MakeUDPSocket( UDP_PORT_NUM );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_status.player[ i ].x = 0;
		_status.player[ i ].y = 0;
		_status.player[ i ].button = BUTTON_NONE;
	}
}

Client::~Client( ) {
}

void Client::update( ) {
	switch( _phase ) {
	case PHASE_READY:
		updateReady( );
		break;
	case PHASE_CONNECTING:
		updateConnecting( );
		break;
	}
}

void Client::updateReady( ) {
	// 接続
	if ( connect( ) ) {
		_phase = PHASE_CONNECTING;
	}
}

void Client::updateConnecting( ) {
	if ( _tcp_handle < 0 ) {
		_phase = PHASE_READY;
		return;
	}

	if ( GetLostNetWork( ) == _tcp_handle ) {
		CloseNetWork( _tcp_handle );	// 接続を断つ
		_tcp_handle = -1;
	}
	
	recieveStatus( );
	responseOfState( );
}

bool Client::load( ) {
	int fh = FileRead_open( "IP.ini" );
	if ( fh == 0 ) {
		return false;
	}

	IPDATA ip;
	if ( FileRead_read( &ip, sizeof( IPDATA ), fh ) == -1 ) {
		FileRead_close( fh ) ;
		return false;
	}

	_ip[ 0 ] = ip.d1;
	_ip[ 1 ] = ip.d2;
	_ip[ 2 ] = ip.d3;
	_ip[ 3 ] = ip.d4;

    FileRead_close( fh ) ;
	return true;
}

std::string Client::getPhase( ) const {
	std::string str;
	switch( _phase ) {
	case PHASE_READY:
		return str = "PHASE_READY";
		break;
	case PHASE_CONNECTING:
		return str = "PHASE_CONNECTING";
		break;
	default:
		return str = "PHASE_NONE";
		break;
	}
}

bool Client::connect( ) {

	IPDATA dip;
	dip.d1 = _ip[ 0 ];
	dip.d2 = _ip[ 1 ];
	dip.d3 = _ip[ 2 ];
	dip.d4 = _ip[ 3 ];

	_tcp_handle = ConnectNetWork( dip, TCP_PORT_NUM );

	return _tcp_handle >= 0;
}

void Client::send( const SERVERDATA& data ) {
	if ( _tcp_handle < 0 ) {
		return;
	}

	NetWorkSend( _tcp_handle, &data, sizeof( SERVERDATA ) );
}

CLIENTDATA Client::getClientData( ) {
	return _status;
}

void Client::recieveStatus( ) {
	while ( CheckNetWorkRecvUDP( _udp_handle ) == TRUE ) {
		NetWorkRecvUDP( _udp_handle, NULL, NULL, &_status, sizeof( CLIENTDATA ), FALSE );
	}
}

void Client::responseOfState( ) {
	if ( _tcp_handle < 0 ) {
		return;
	}

	if ( GetNetWorkDataLength( _tcp_handle ) <= 0 ) {
		return;
	}

	// 受信
	SERVERDATA data;
	NetWorkRecv( _tcp_handle, &data, sizeof( data ) );

	// データ処理
	switch ( data.command ) {
	case COMMAND_CONDITION:
		break;
	}
}