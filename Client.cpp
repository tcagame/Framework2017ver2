#include "Client.h"
#include "Application.h"
#include "Data.h"
#include "Network.h"
#include "DxLib.h"

ClientPtr Client::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Client >( fw->getTask( Client::getTag( ) ) );
}

Client::Client( DataPtr data_udp, DataPtr data_tcp ) :
_data_udp( data_udp ),
_data_tcp( data_tcp ),
_recieving_udp( false ) {
	_phase = PHASE_READY;

	// Server IP 読み込み
	load( );

	_network_state_time = 0;
	_tcp_handle = -1;
	_udp_handle = MakeUDPSocket( UDP_PORT_NUM );
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
	
	recieveUdp( );
	recieveTcp( );
}

bool Client::load( ) {
	int fh = FileRead_open( "IP.ini" );
	if ( fh == 0 ) {
		fh = FileRead_open( "../IP.ini" );
	}
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

void Client::sendTcp( DataPtr data ) {
	if ( _tcp_handle < 0 ) {
		return;
	}

	NetWorkSend( _tcp_handle, data->getPtr( ), data->getSize( ) );
}

void Client::recieveUdp( ) {
	_recieving_udp = false;
	while ( CheckNetWorkRecvUDP( _udp_handle ) == TRUE ) {
		NetWorkRecvUDP( _udp_handle, NULL, NULL, _data_udp->getPtr( ), _data_udp->getSize( ), FALSE );
		_recieving_udp = true;
	}
}

void Client::recieveTcp( ) {
	if ( _tcp_handle < 0 ) {
		return;
	}
	
	if ( GetNetWorkDataLength( _tcp_handle ) <= 0 ) {
		return;
	}
	
	// タイプとデータ長を読み込む(データは破棄しない)
	char fourcc[ 4 ];
	NetWorkRecvToPeek( _tcp_handle, ( void * )fourcc, sizeof( fourcc ) );
	if ( fourcc[ 0 ] == 'C' && 
		 fourcc[ 1 ] == 'O' &&
		 fourcc[ 2 ] == 'N' &&
		 fourcc[ 3 ] == 'D' ) {
		// 破棄して終了
		NetWorkRecv( _tcp_handle, ( void * )fourcc, sizeof( fourcc ) );
		return;
	}

	// 受信
	NetWorkRecv( _tcp_handle, _data_tcp->getPtr( ), _data_tcp->getSize( ) );
}

bool Client::isRecievingUDP( ) const {
	return _recieving_udp;
}
