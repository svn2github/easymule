#include "StdAfx.h"
#include ".\statforserver.h"
#include "opcodes.h"
#include "Preferences.h"
#include "eMule.h"
#include "DownloadQueue.h"

CStatForServer	theStatForServer;

CStatForServer::CStatForServer(void)
{
	ResetAverageSpeeds();
}

CStatForServer::~CStatForServer(void)
{
}

void CStatForServer::RecordCurrentRate(float uploadrate, float downloadrate)
{
	//	if reach record interval
	DWORD	dwCurTime = GetTickCount();
	if (dwCurTime - m_dwLastRecordTime < RECORD_INTERVAL_MS)
		return;
	m_dwLastRecordTime = dwCurTime;


	
	if (ULONG_MAX != m_ulRecordTimes_Download
		&& IsAnyTaskRunning())
	{
		m_ulRecordTimes_Download++;
		//MODIFIED by fengwen on 2007/06/19 <begin>	: 原来的算法错了。
		//m_fAverageDownloadSpeed = (ULONG) ( ((float)m_fAverageDownloadSpeed + downloadrate) / m_ulRecordTimes_Download );
		m_fAverageDownloadSpeed = m_fAverageDownloadSpeed + ((downloadrate - m_fAverageDownloadSpeed) / m_ulRecordTimes_Download );
		//MODIFIED by fengwen on 2007/06/19 <end>	: 原来的算法错了。
	}

	if (ULONG_MAX != m_ulRecordTimes_Upload)
	{
		m_ulRecordTimes_Upload++;
		//MODIFIED by fengwen on 2007/06/19 <begin>	: 原来的算法错了。
		//m_fAverageUploadSpeed = (ULONG) ( ((float)m_fAverageUploadSpeed + uploadrate) / m_ulRecordTimes_Upload );
		m_fAverageUploadSpeed = m_fAverageUploadSpeed + ( (uploadrate - m_fAverageUploadSpeed) / m_ulRecordTimes_Upload );
		//MODIFIED by fengwen on 2007/06/19 <end>	: 原来的算法错了。
	}

}

void CStatForServer::GetAverageSpeeds(ULONG &ulUpload, ULONG &ulDownload)
{
	ulUpload	= (ULONG) m_fAverageUploadSpeed;
	ulDownload	= (ULONG) m_fAverageDownloadSpeed;
}

void CStatForServer::ResetAverageSpeeds()
{
	m_dwLastRecordTime			= 0;
	m_ulRecordTimes_Upload		= 0;
	m_fAverageUploadSpeed		= 0;
	m_ulRecordTimes_Download	= 0;
	m_fAverageDownloadSpeed	= 0;
}

void CStatForServer::GetCurrentSpeedLimit(ULONG &ulUpload, ULONG &ulDownload)
{
	ulDownload = (thePrefs.maxdownload != UNLIMITED) ? thePrefs.maxdownload : 0;
	ulUpload = (thePrefs.maxupload != UNLIMITED) ? thePrefs.maxupload : 0;
}

BOOL CStatForServer::IsAnyTaskRunning()
{
	if (NULL == CGlobalVariable::downloadqueue)
		return FALSE;
	else
		return CGlobalVariable::downloadqueue->IsAnyTaskRunning();
}
