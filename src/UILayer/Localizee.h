#pragma once

class CLocalizee
{
public:
	CLocalizee(void);
	~CLocalizee(void);

	virtual void Localize() = 0;
	virtual BOOL CanLocalize() = 0;
};

#define LOCALIZEE_WND_CANLOCALIZE()	BOOL CanLocalize(){return (GetSafeHwnd() != NULL);}