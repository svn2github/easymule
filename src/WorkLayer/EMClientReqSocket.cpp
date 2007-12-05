#include "StdAfx.h"
#include ".\emclientreqsocket.h"
#include "GlobalVariable.h"
#include "updownclient.h"

IMPLEMENT_DYNCREATE(CEMClientReqSocket, CClientReqSocket)

CEMClientReqSocket::CEMClientReqSocket(CUpDownClient* in_client) : CClientReqSocket(in_client)
{
	
}

CEMClientReqSocket::~CEMClientReqSocket(void)
{
	
}
