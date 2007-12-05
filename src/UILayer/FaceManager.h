#pragma once

#include <vector>
#include "CxImage\xImage.h"

typedef struct tagBUTTONFACE
{
	//  normal state
	HBITMAP left, mid, right;
	//  active state
	HBITMAP left_a, mid_a, right_a;
	//  hover state
	HBITMAP left_h, mid_h, right_h;
}BUTTONFACE;

typedef struct tagImageFace
{
	CxImage		left;
	CxImage		mid;
	CxImage		right;
} IMAGEBAR;

enum EFaceIndex
{
	FI_SHARE_FILE_COUNT,
	// 尽量使用下面的Image，which support png format and so on.

	FI_MAX
};

enum EImageIndex
{
	II_DETAILTAB_N,
	II_DETAILTAB_H,
	II_DETAILTAB_A,
	II_SEARCHBTN_N,
	II_SEARCHBTN_H,
	II_SEARCHBTN_P,
	II_MAINTAB_BK,
	II_PAGETAB_BK,
	II_VERYCDSEARCH,
	II_MAINTABMORE_N,
	II_MAINTABMORE_H,
	II_MAINTABMORE_P,
	II_SPLITER_H,
	II_SPLITER_V,

	II_MAX
};

enum EImageBarIndex
{
	IBI_MAINBTN_A,
	IBI_MAINBTN_N,
	IBI_MAINBTN_H,
	IBI_SEARCHBAR_EDIT,
	IBI_PAGETAB_N,
	IBI_PAGETAB_A,
	IBI_PAGETAB_H,

	IBI_MAX
};

//  face state
#define FS_NORMAL		0
#define FS_ACTIVE		1
#define FS_HOVER		2

class CFaceManager
{
public:
	CFaceManager(void);
	~CFaceManager(void);

	void Init();
	static CFaceManager * GetInstance();

	void DrawFace(int nFaceIndex, HDC hDC, int nState, const CRect & rect, bool bVertDraw = false);
	void DrawImage(int nImageIndex, HDC hDC, const CRect & rect, int iMode = 0); // Mode: 0.Normal, 1.Tile, 2.Stretch
	BOOL GetImageSize(int nImageIndex, CSize &size);
	void DrawImageBar(int nImageBarIndex, HDC hDC, const CRect & rect, bool bVertDraw = false);
private:
	void ClearAllRes();
	std::vector<BUTTONFACE *> m_ButtonFaces;
	CxImage					m_arrImages[II_MAX];
	IMAGEBAR				m_arrImageBars[IBI_MAX];
public:

	CDC m_DC;
};
