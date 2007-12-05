#include "StdAfx.h"
#include ".\localizee.h"
#include "LocalizeMgr.h"

CLocalizee::CLocalizee(void)
{
	theLocalizeMgr.Register(this);
}

CLocalizee::~CLocalizee(void)
{
	theLocalizeMgr.UnRegister(this);
}
