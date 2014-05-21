#pragma once

#include "fileinfo.h"
#include <shlobj.h>     // For IShellExtInit and IContextMenu


class FileContextMenuExt : public IShellExtInit, public IContextMenu
{
public:
    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellExtInit
	// pidlFolder holds a folder's pointer to an item identifier list (PIDL). This is an absolute PIDL. For property sheet extensions, this value is NULL.
	// pDataObject holds a pointer to a data object's IDataObject interface. The data object holds one or more file names in CF_HDROP format.
	// hRegKey holds a registry key for the file object or folder type.
	IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);
	
    // IContextMenu
    IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
    IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	
    FileContextMenuExt(void);

protected:
    ~FileContextMenuExt(void);

private:
    // Reference count of component.
    long m_cRef;

    // The name of the selected file.

    // The method that handles the "display" verb.
    void OnVerbDisplayFileName(HWND hWnd);

    PWSTR m_pszMenuText;
    HANDLE m_hMenuBmp;
    PCSTR m_pszVerb;
    PCWSTR m_pwszVerb;
    PCSTR m_pszVerbCanonicalName;
    PCWSTR m_pwszVerbCanonicalName;
    PCSTR m_pszVerbHelpText;
    PCWSTR m_pwszVerbHelpText;
	LPWSTR m_pwszLogFileName;
	 
	UINT m_nFiles;
	filemap m_fMap;
};