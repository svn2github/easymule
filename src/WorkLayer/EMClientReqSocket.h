#pragma once
#include "clientreqsocket.h"
class CUpDownClient;

class CEMClientReqSocket :	public CClientReqSocket
{
	DECLARE_DYNCREATE(CEMClientReqSocket)
public:
	CEMClientReqSocket(CUpDownClient* in_client = NULL);
	~CEMClientReqSocket(void);

	bool	CheckTimeOut();
};
