// Win32++   Version 8.1
// Release Date: 4th January 2016
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2016  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// wxx_wincore0.h
// This file contains the declaration of the CWnd class.
//

#ifndef _WIN32XX_WINCORE0_H_
#define _WIN32XX_WINCORE0_H_


#include "wxx_rect.h"
#include "wxx_cstring.h"
#include "wxx_appcore0.h"



// Messages defined by Win32++
// WM_APP range: 0x8000 through 0xBFFF
// Note: The numbers defined for window messages don't need to be unique. View windows defined by users for example,
//  could use other user defined messages with the same number as those below without issue. 
#define UWM_DOCKACTIVATE     (WM_APP + 0x3F01)  // Message - sent to dock ancestor when a docker is activated or deactivated.
#define UWM_DOCKDESTROYED	 (WM_APP + 0x3F02)	// Message - posted when docker is destroyed
#define UWM_DRAWRBBKGND      (WM_APP + 0x3F03)	// Message - sent by rebar to parent to perform background drawing. Return TRUE if handled.
#define UWM_DRAWSBBKGND      (WM_APP + 0x3F04)	// Message - sent by statusbar to parent to perform background drawing. Return TRUE if handled.
#define UWM_GETFRAMEVIEW     (WM_APP + 0x3F05)  // Message - returns the HWND of the frame's view window
#define UWM_GETMBTHEME       (WM_APP + 0x3F06)	// Message - returns a pointer to MenuBarTheme
#define UWM_GETRBTHEME       (WM_APP + 0x3F07)	// Message - returns a pointer to ReBarTheme
#define UWM_GETSBTHEME       (WM_APP + 0x3F08)	// Message - returns a pointer to StatusBarTheme
#define UWM_GETTBTHEME       (WM_APP + 0x3F09)  // Message - returns a pointer to ToolBarTheme
#define UWM_POPUPMENU		 (WM_APP + 0x3F0A)	// Message - creates the menubar popup menu
#define UWM_TBRESIZE         (WM_APP + 0x3F0B)  // Message - sent by toolbar to parent. Used by the rebar
#define UWM_TBWINPOSCHANGING (WM_APP + 0x3F0C)	// Message - sent to parent. Toolbar is resizing
#define UWM_UPDATECOMMAND    (WM_APP + 0x3F0D)  // Message - sent before a menu is displayed. Used by OnMenuUpdate
#define UWM_ISDOCKER		 (WM_APP + 0x3F0E)  // Message - a Docker window returns TRUE
#define UWM_ISDOCKCONTAINER	 (WM_APP + 0x3F0F)  // Message - a DockContainer window returns TRUE
#define UWM_ISFRAME			 (WM_APP + 0x3F10)  // Message - a Frame window returns TRUE
#define UWM_ISMDIFRAME		 (WM_APP + 0x3F11)  // Message - a MDIFrame window returns TRUE
#define UWM_ISTABBEDMDI		 (WM_APP + 0x3F12)  // Message - a TabbedMDI window returns TRUE
#define UWM_ISTOOLBAR		 (WM_APP + 0x3F13)  // Message - a ToolBar window returns TRUE
#define UWN_BARSTART		 (WM_APP + 0x3F20)	// Notification - docker bar selected for move
#define UWN_BARMOVE			 (WM_APP + 0x3F21)	// Notification - docker bar moved
#define UWN_BAREND			 (WM_APP + 0x3F22)	// Notification - end of docker bar move
#define UWN_DOCKSTART		 (WM_APP + 0x3F23)	// Notification - about to start undocking
#define UWN_DOCKMOVE		 (WM_APP + 0x3F24)	// Notification - undocked docker is being moved
#define UWN_DOCKEND			 (WM_APP + 0x3F25)	// Notification - docker has been docked
#define UWN_TABCHANGED       (WM_APP + 0x3F26)	// Notification - tab size or position changed
#define UWN_TABDRAGGED       (WM_APP + 0x3F27)	// Notification - tab is being dragged
#define UWN_UNDOCKED		 (WM_APP + 0x3F28)	// Notification - sent by docker when undocked
#define UWN_TABCLOSE		 (WM_APP + 0x3F29)	// Notification - sent by CTab when a tab is about to be closed


namespace Win32xx
{
	////////////////////////////////
	// Declaration of the CWnd class
	//
	class CWnd : public CObject
	{
	friend class CMDIChild;
	friend class CDialog;
	friend class CPropertyPage;
	friend class CTaskDialog;
	friend class CWinApp;
	friend class CWinThread;

	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor

		// These virtual functions can be overridden
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, HWND hwndParent);
		virtual void CenterWindow() const;
		virtual HWND Create(HWND hParent = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
		                      DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, 
							  HMENU nIDorHMenu, LPVOID lpParam = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
		                      DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT nID, LPVOID lpParam = NULL);
		virtual void Destroy();
		virtual HWND Detach();
		static  CWnd* GetCWndPtr(HWND hWnd);
		virtual HICON SetIconLarge(int nIcon);
		virtual HICON SetIconSmall(int nIcon);

		// For Data Exchange
	#ifndef _WIN32_WCE
		virtual void DoDataExchange(CDataExchange& DX);
		virtual BOOL UpdateData(CDataExchange& DX, BOOL bRetrieveAndValidate);
	#endif

		// Attributes
		HWND GetHwnd() const				{ return m_hWnd; }
		WNDPROC GetPrevWindowProc() const	{ return m_PrevWindowProc; }

		// Wrappers for Win32 API functions
		// These functions aren't virtual, and shouldn't be overridden
		HDC   BeginPaint(PAINTSTRUCT& ps) const;
		BOOL  BringWindowToTop() const;
		LRESULT CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		BOOL  CheckDlgButton(int nIDButton, UINT uCheck) const;
		BOOL  CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) const;
		CWnd  ChildWindowFromPoint(POINT pt) const;
		BOOL  ClientToScreen(POINT& pt) const;
		BOOL  ClientToScreen(RECT& rc) const;
		LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		HDWP  DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		HDWP  DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		BOOL  DrawMenuBar() const;
		BOOL  EnableWindow(BOOL bEnable = TRUE) const;
		BOOL  EndPaint(PAINTSTRUCT& ps) const;
		CWnd  GetActiveWindow() const;
		CWnd  GetAncestor(UINT gaFlag = 3 /*= GA_ROOTOWNER*/) const;
		CWnd  GetCapture() const;
		ULONG_PTR GetClassLongPtr(int nIndex) const;
		CString GetClassName() const;
		CRect GetClientRect() const;
		CDC   GetDC() const;
		CDC   GetDCEx(HRGN hrgnClip, DWORD flags) const;
		CWnd  GetDesktopWindow() const;
		int	  GetDlgCtrlID() const;
		CWnd  GetDlgItem(int nIDDlgItem) const;
		UINT  GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const;
		CString GetDlgItemText(int nIDDlgItem) const;
		CWnd  GetFocus() const;
		CFont GetFont() const;
		HICON GetIcon(BOOL bBigIcon) const;
		CWnd  GetNextDlgGroupItem(HWND hCtl, BOOL bPrevious) const;
		CWnd  GetNextDlgTabItem(HWND hCtl, BOOL bPrevious) const;
		CWnd  GetParent() const;
		BOOL  GetScrollInfo(int fnBar, SCROLLINFO& si) const;
		CRect GetUpdateRect(BOOL bErase) const;
		int GetUpdateRgn(HRGN hRgn, BOOL bErase) const;
		CWnd  GetWindow(UINT uCmd) const;
		CDC   GetWindowDC() const;
		LONG_PTR GetWindowLongPtr(int nIndex) const;
		CRect GetWindowRect() const;
		CString GetWindowText() const;
		int   GetWindowTextLength() const;
		void  Invalidate(BOOL bErase = TRUE) const;
		BOOL  InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) const;
		BOOL  InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE) const;
		BOOL  IsChild(HWND hwndChild) const;
		BOOL  IsDialogMessage(MSG& Msg) const;
		UINT  IsDlgButtonChecked(int nIDButton) const;
		BOOL  IsWindow() const;
		BOOL  IsWindowEnabled() const;
		BOOL  IsWindowVisible() const;
		BOOL  KillTimer(UINT_PTR uIDEvent) const;
		int   MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const;
		void  MapWindowPoints(HWND hWndTo, POINT& pt) const;
		void  MapWindowPoints(HWND hWndTo, RECT& rc) const;
		void  MapWindowPoints(HWND hWndTo, LPPOINT ptArray, UINT nCount) const;
		BOOL  MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) const;
		BOOL  MoveWindow(const RECT& rc, BOOL bRepaint = TRUE) const;
		BOOL  PostMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		BOOL  PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		BOOL  RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgn = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN) const;
		int   ReleaseDC(HDC hDC) const;
		BOOL  ScreenToClient(POINT& Point) const;
		BOOL  ScreenToClient(RECT& rc) const;
		LRESULT SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		LRESULT SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		BOOL  SendNotifyMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const;
		CWnd  SetActiveWindow() const;
		CWnd  SetCapture() const;
		ULONG_PTR SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		LONG_PTR SetDlgCtrlID(int idCtrl) const;
		BOOL  SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const;
		BOOL  SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const;
		CWnd  SetFocus() const;
		void  SetFont(HFONT hFont, BOOL bRedraw = TRUE) const;
		BOOL  SetForegroundWindow() const;
		HICON SetIcon(HICON hIcon, BOOL bBigIcon) const;
		CWnd  SetParent(HWND hWndParent) const;
		BOOL  SetRedraw(BOOL bRedraw = TRUE) const;
		int   SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const;
		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) const;
		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL  SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		BOOL  SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		int   SetWindowRgn(CRgn& Rgn, BOOL bRedraw = TRUE) const;
		BOOL  SetWindowText(LPCTSTR lpString) const;
		HRESULT SetWindowTheme(LPCWSTR pszSubAppName, LPCWSTR pszSubIdList) const;
		BOOL  ShowWindow(int nCmdShow = SW_SHOWNORMAL) const;
		BOOL  UpdateWindow() const;
		BOOL  ValidateRect(LPCRECT prc) const;
		BOOL  ValidateRgn(HRGN hRgn) const;
		static CWnd WindowFromPoint(POINT pt);

  #ifndef _WIN32_WCE
		BOOL  CloseWindow() const;
		int   DlgDirList(LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType) const;
		int   DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype) const;
		BOOL  DlgDirSelectEx(LPTSTR lpString, int nCount, int nIDListBox) const;
		BOOL  DlgDirSelectComboBoxEx(LPTSTR lpString, int nCount, int nIDComboBox) const;
		BOOL  DrawAnimatedRects(int idAni, RECT& rcFrom, RECT& rcTo) const;
		BOOL  DrawCaption(HDC hDC, RECT& rc, UINT uFlags) const;
		BOOL  EnableScrollBar(UINT uSBflags, UINT uArrows) const;
		CWnd  GetLastActivePopup() const;
		CMenu GetMenu() const;
		int   GetScrollPos(int nBar) const;
		BOOL  GetScrollRange(int nBar, int& MinPos, int& MaxPos) const;
		CMenu GetSystemMenu(BOOL bRevert) const;
		CWnd  GetTopWindow() const;
		BOOL  GetWindowPlacement(WINDOWPLACEMENT& pWndpl) const;
		BOOL  HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) const;
		BOOL  IsIconic() const;
		BOOL  IsZoomed() const;
		BOOL  LockWindowUpdate() const;
		BOOL  OpenIcon() const;
		void  Print(HDC hDC, DWORD dwFlags) const;
		BOOL  SetMenu(HMENU hMenu) const;
		BOOL  ScrollWindow(int XAmount, int YAmount, LPCRECT lprcScroll, LPCRECT lprcClip) const;
		int   ScrollWindowEx(int dx, int dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT flags) const;
		int   SetScrollPos(int nBar, int nPos, BOOL bRedraw) const;
		BOOL  SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const;
		BOOL  SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const;
		BOOL  ShowOwnedPopups(BOOL fShow) const;
		BOOL  ShowScrollBar(int nBar, BOOL bShow) const;
		BOOL  ShowWindowAsync(int nCmdShow) const;
		BOOL  UnLockWindowUpdate() const;
		CWnd  WindowFromDC(HDC hDC) const;

    #ifndef WIN32_LEAN_AND_MEAN
		void  DragAcceptFiles(BOOL fAccept) const;
    #endif
  #endif

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		operator HWND() const { return GetHwnd(); }

	protected:
		// Override these functions as required
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void OnAttach();
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnClose();
		virtual int  OnCreate(CREATESTRUCT& cs);
		virtual void OnDestroy();
		virtual void OnDraw(CDC& dc);
		virtual BOOL OnEraseBkgnd(CDC& dc);
		virtual void OnInitialUpdate();
		virtual void OnMenuUpdate(UINT nID);
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual BOOL PreTranslateMessage(MSG& Msg);
		virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;					// handle to this object's window

	private:
		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&);	// Disable assignment operator
		CWnd(HWND hWnd);				// Private constructor used internally

		void AddToMap();
		void Cleanup();
		LRESULT MessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL RegisterClass(WNDCLASS& wc);
		BOOL RemoveFromMap();
		void SetHwnd(HWND hWnd)			{ m_hWnd = hWnd; }
		void Subclass(HWND hWnd);

		WNDPROC	m_PrevWindowProc;

	}; // class CWnd

}

#endif // _WIN32XX_WINCORE0_H_

