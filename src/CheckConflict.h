#pragma once

#include <psapi.h>
#include <dbghelp.h>

class CCheckConflict
{
public:
	CCheckConflict(void);
	~CCheckConflict(void);

	BOOL	CheckConflict(struct _EXCEPTION_POINTERS* pExceptionInfo);	//return TRUE : 已处理，不需发送。 FALSE : 没有处理，需弹出发送对话框。
protected:
	enum {DESC_MAX = 128};

	typedef BOOL (*CONFLICT_PROC) (int iModuleIndex, HMODULE hMod);
	typedef struct _CONFILCT_MOD_ENTRY{
		TCHAR			szModuleName[DESC_MAX];
		TCHAR			szModuleDescription[DESC_MAX];
		int				iStackSearchLevel;						//0:遍历call stack直到结束 | 1--n:向上查找n层call stack。
		CONFLICT_PROC	conflictProc;							//查找的地址在Module里的时处理函数。
	} CONFILCT_MOD_ENTRY;
	const static CONFILCT_MOD_ENTRY	s_conflictModuleList[];

	static BOOL SimplyPrompt(int iModuleIndex, HMODULE hMod);
	static BOOL ThirdPartyError(int iModuleIndex, HMODULE hMod);
	static BOOL WebBrowserProblem(int iModuleIndex, HMODULE hMod);
public:
	static BOOL FlashProblem(int iModuleIndex, HMODULE hMod);

protected:
	typedef BOOL (__stdcall *PFN_GetModuleInformation) (HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);
	typedef BOOL (__stdcall *PFN_StackWalk64) (DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord,
									PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
									PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
	typedef BOOL (__stdcall *PFN_EnumProcessModules) (HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);


protected:
	BOOL	IsFuncitonsReady();
	BOOL	IsAddressInModule(PVOID pvAddress, HMODULE hModule);
	BOOL	IsCrashInEmuleExe(struct _EXCEPTION_POINTERS* pExceptionInfo);

	BOOL	PreCheck(struct _EXCEPTION_POINTERS* pExceptionInfo);
	BOOL	CheckModules(struct _EXCEPTION_POINTERS* pExceptionInfo);
	BOOL	PostCheck(struct _EXCEPTION_POINTERS* pExceptionInfo);
public:
	BOOL	UserAnalyseDiy(struct _EXCEPTION_POINTERS* pExceptionInfo);
protected:
	HMODULE								m_hPsapiDll;
	PFN_GetModuleInformation			m_pfnGetModuleInformation;

	HMODULE								m_hDbgHelpDll;
	PFN_StackWalk64						m_pfnStackWalk64;
	PFUNCTION_TABLE_ACCESS_ROUTINE64	m_pfnSymFunctionTableAccess64;
	PGET_MODULE_BASE_ROUTINE64			m_pfnSymGetModuleBase64;
	PFN_EnumProcessModules				m_pfnEnumProcessModules;

protected:
	static BOOL	GetAppPath(LPTSTR lpszBuffer, DWORD dwBufferCch);
	static BOOL	GetIniPathName(LPTSTR lpszBuffer, DWORD dwBufferCch);
	static BOOL	GetIniBool(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL *pbValue);
	static BOOL	WriteIniBool(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL bValue);
};
