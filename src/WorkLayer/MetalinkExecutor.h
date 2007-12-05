#pragma once

#include "TransferCompletedProcessor.h"

class CMetalinkExecutor : public IExecutor
{
public:
	CMetalinkExecutor(void);
	~CMetalinkExecutor(void);

	virtual bool execute( CPartFile * partFile );
};
