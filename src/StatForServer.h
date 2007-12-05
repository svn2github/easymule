#pragma once

class CStatForServer
{
public:
	CStatForServer(void);
	~CStatForServer(void);

	enum{RECORD_INTERVAL_MS = 60000};

	void	RecordCurrentRate(float uploadrate, float downloadrate);

	void	GetAverageSpeeds(ULONG &ulUpload, ULONG &ulDownload);
	void	ResetAverageSpeeds();
	void	GetCurrentSpeedLimit(ULONG &ulUpload, ULONG &ulDownload);
protected:
	DWORD	m_dwLastRecordTime;
	ULONG	m_ulRecordTimes_Upload;
	float	m_fAverageUploadSpeed;
	ULONG	m_ulRecordTimes_Download;
	float	m_fAverageDownloadSpeed;

protected:
	BOOL	IsAnyTaskRunning();
};

extern CStatForServer	theStatForServer;
