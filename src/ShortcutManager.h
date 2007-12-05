#pragma once

////////////////////////////////////////////////////////////////////////////
#define WC_HOTKEYA              "msctls_hotkey32"	// ansi
#define WC_HOTKEYW              L"msctls_hotkey32"	// wide

#ifdef UNICODE
#define WC_HOTKEY               WC_HOTKEYW
#else
#define WC_HOTKEY               WC_HOTKEYA
#endif

////////////////////////////////////////////////////////////////////////////
#define WC_RICHEDITA              "Richedit"	// ansi
#define WC_RICHEDITW              L"Richedit"	// wide

#ifdef UNICODE
#define WC_RICHEDIT               WC_RICHEDITW
#else
#define WC_RICHEDIT               WC_RICHEDITA
#endif

/////////////////////////////////////////////////////////////////////////////
#define WC_RICHEDIT20A              "RichEdit20A"	// ansi
#define WC_RICHEDIT20W              L"RichEdit20W"	// wide

#ifdef UNICODE
#define WC_RICHEDIT20               WC_RICHEDIT20W
#else
#define WC_RICHEDIT20               WC_RICHEDIT20A
#endif
enum 
{
	HKCOMB_EXFKEYS		= 0x0100,
	HKCOMB_EDITCTRLS	= 0x0200, // 防止在编辑框中快捷键冲突
};

class CShortcutManager
{
public:
	CShortcutManager(BOOL bAutoSendCmds = 1);
	virtual ~CShortcutManager(void);

//Class
protected:
	HWND			m_hWndHooked;
	BOOL			m_bPreMFCSubclass;

protected:
	static CString GetClass(HWND hWnd);
	static BOOL IsEditControl(HWND hWnd);
	static BOOL IsRichEditControl(HWND hWnd);

	virtual BOOL HookWindow(HWND hRealWnd);
	virtual BOOL IsHooked() const { return m_hWndHooked != NULL; }

	inline CWnd* GetCWnd() const { return CWnd::FromHandle(m_hWndHooked); }


	inline BOOL IsWindowEnabled() const { return ::IsWindowEnabled(m_hWndHooked); }
	inline BOOL IsWindowVisible() const { return ::IsWindowVisible(m_hWndHooked); }

	static BOOL IsRichEditControl(LPCTSTR szClass);
	static BOOL IsEditShortcut(DWORD dwShortcut);

	static BOOL IsClass(HWND hWnd, LPCTSTR szClass);
	static BOOL IsClass(LPCTSTR szClass, LPCTSTR szWndClass) { return (lstrcmpi(szClass, szWndClass) == 0); }

public:
	BOOL Initialize(CWnd* pOwner, WORD wInvalidComb = HKCOMB_EDITCTRLS, WORD wFallbackModifiers = 0);
	BOOL Release();

	BOOL AddShortcut(UINT nCmdID, WORD wVirtKeyCode, WORD wModifiers = HOTKEYF_CONTROL); 
	BOOL AddShortcut(UINT nCmdID, DWORD dwShortcut);

	void SetShortcut(UINT nCmdID, WORD wVirtKeyCode, WORD wModifiers = HOTKEYF_CONTROL); 
	void SetShortcut(UINT nCmdID, DWORD dwShortcut); 

	WORD ValidateModifiers(WORD wModifiers, WORD wVirtKeyCode) const;

	//在PreTranslateMessage函数中调用，返回CmdID，或者0
	UINT ProcessMessage(const MSG* pMsg, DWORD* pShortcut) const;

protected:

	DWORD GetShortcut(WORD wVirtKeyCode, BOOL bExtended) const;
protected:
	CMap<DWORD, DWORD, UINT, UINT&> m_mapShortcut2ID;
	WORD m_wInvalidComb;
	WORD m_wFallbackModifiers;

	BOOL m_bAutoSendCmds;
};
