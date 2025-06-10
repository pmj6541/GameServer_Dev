#pragma once
#include <winsock2.h>
#include <iostream>

struct RecvContext {
	OVERLAPPED overlapped;	// IO 상태 추적
	SOCKET socket;			// 해당 소켓
	uint8_t buffer[4096];	// 실제 데이터 수신 버퍼
};