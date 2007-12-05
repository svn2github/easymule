class CFindWnd 
{
private:
	static BOOL CALLBACK FindChildClassHwnd(HWND hwndParent, LPARAM lParam) 
	{
		CFindWnd *pfw = (CFindWnd*)lParam;
		HWND hwnd = FindWindowEx(hwndParent, NULL, pfw->m_classname, NULL);

		if (hwnd) 
		{
			pfw->m_hWnd = hwnd;
			return FALSE;
		}
		EnumChildWindows(hwndParent, FindChildClassHwnd, lParam);
		return TRUE;
	}

public:
	LPCTSTR m_classname;
	HWND m_hWnd;

	CFindWnd(HWND hwndParent, LPCTSTR classname) : m_hWnd(NULL), m_classname(classname)
	{
		FindChildClassHwnd(hwndParent, (LPARAM)this);
	}
};