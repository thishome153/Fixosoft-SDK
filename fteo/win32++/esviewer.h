////////////////////////////////////////////////////////////////////
// esviewer.h


#ifndef  ESVIEWER_H
#define ESVIEWER_H

#include "MyStatusBar.h"
#include "wxx_resource.h"
#include "wxx_wincore.h" // ���� win32++
#include "wxx_frame.h" // ���� win32++

namespace fteo
{
  namespace  Win32xx
{

///////////////////////////////////////////////////////////////////
//     ESView
class ESView :   public CWnd

	{
public:
	ESView();
	~ESView();
	HWND OwnerStatusBarHandle;
	//virtual void OnDraw(CDC* pDC); // MFC

    // ���������� �� win32++ ������:
    virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnDestroy();
	virtual void OnDraw(CDC& dc);	
	virtual void OnInitialUpdate();
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//������������� ����������
	void DrawLine(int x, int y);
	void Point   (int x, int y, int Radius, bool Filled); 
	// ����������� ������� ����:
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp  (UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove  (UINT uMsg, WPARAM wParam, LPARAM lParam);
	};


//     CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	BOOL	OnFileExit();
	BOOL	OnFileOpen();
	BOOL	OnFilePrint();
	BOOL	OnFileSave();

protected:
	virtual CMyStatusBar& GetStatusBar() const	{ return const_cast<CMyStatusBar&>(m_MyStatusBar); }
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int		OnCreate(CREATESTRUCT& cs);
	virtual LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void	PreCreate(CREATESTRUCT& cs);
	virtual void	SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	ESView m_View;
	CMyStatusBar m_MyStatusBar;
};



////////////////////////////////////////////////////////////////////
//     Applications
// win32 Application. ��� ���� ESView �������� �� �������....���� ���
class CViewerApp : public CWinApp
{
public:
    CViewerApp();
    virtual ~CViewerApp() {}
	virtual BOOL InitInstance();
   // CMainFrame& GetMainFrame() { return m_Frame; }
private:
    ESView m_View;
	
	};

///////////////////////////////////////////////////////////////////////
//     ���������� v2 . ����� ������ ��� � ESView-��������� CFrame
class CViewerApp2 : public CWinApp
{
public:
	CViewerApp2();
	virtual ~CViewerApp2();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
	CMainFrame m_Frame;
};
	  }
	}


#endif //