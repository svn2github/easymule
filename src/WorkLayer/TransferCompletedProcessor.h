#ifndef TRANSFERCOMPLETEDPROCESSOR_H
#define TRANSFERCOMPLETEDPROCESSOR_H

#include <vector>

class CPartFile;

class IExecutor
{
public:
	virtual ~IExecutor() {}

	virtual bool execute( CPartFile * partFile ) = 0;
};

class CTransferCompletedProcessor
{
	// 
public:
	CTransferCompletedProcessor();
	~CTransferCompletedProcessor();

	bool OnFileTransferCompleted( CPartFile * partFile );

	std::vector<IExecutor*> m_vecExcutors;
};

#endif //TRANSFERCOMPLETEDPROCESSOR_H
