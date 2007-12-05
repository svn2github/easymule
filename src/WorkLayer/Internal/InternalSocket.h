/** @file InternalSocket.h @brief find the file you wanted in the internalnet 
 <pre>
 *	Copyright (c) 2007，Emule
 *	All rights reserved.
 *
 *	当前版本：
 *	作    者：kernel
 *	完成日期：2007-01-11
 *
 *	取代版本：none
 *	作    者：none
 *	完成日期：none
 </pre>*/

#pragma once

//#include "emule.h"
#include "SharedFileList.h"
#include "PartFile.h"
#include "DownloadQueue.h"
#include "sockets.h"
#include "SafeFile.h"
#include "ClientUDPSocket.h"
#include "updownclient.h"
#include "Preferences.h"


#define BUFSIZE 1024
#define MAXADDRSTR 16
#define LOOPCOUNT 100

#define OP_VC_BC_HEADER		0xf1	//Hex "0xBx"

#define OP_BC_REQUESTSOURCE			0xB1
#define OP_BC_ANSWERSOURCE			0xB2

class CInternalSocket : public CAsyncSocket{

	public:

		CInternalSocket();
		~CInternalSocket();

		void Broadcast(const uchar* filehash);
        void Process(char* buffer, uint32 buffersize);

	protected:

		virtual void	OnSend(int nErrorCode);	
		virtual void	OnReceive(int nErrorCode);
		
	private:
		SOCKET m_hSocket2;
		SOCKADDR_IN stSrcAddr, stDestAddr;

};

