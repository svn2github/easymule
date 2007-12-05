#pragma once

#include "CxImage\xImage.h"
#include "TabItem_Normal.h"
class CTabItem_Cake : public CTabItem_Normal
{
public:
	CTabItem_Cake(void);
	~CTabItem_Cake(void);

	virtual	int	GetDesireLength(void){return 36;}
	virtual void Paint(CDC* pDC);

};
