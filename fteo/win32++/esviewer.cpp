
#include "stdafx.h"
//#include "fteo_win32/esviewer.h" // ядро win32++
#include "esviewer.h" // ядро win32++


namespace fteo
{
  namespace  Win32xx
{
	 
ESView::ESView() 
	{
	}
	
ESView::~ESView()
	{
	}



// как всегда времяночка. ДЛя ESView определение 
#define IDW_MAIN                         41


// Definitions for the CView class
int ESView::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	UNREFERENCED_PARAMETER(cs);

	// Set the window's icon
	SetIconSmall(IDW_MAIN);
	SetIconLarge(IDW_MAIN);

	// Set the window title
	SetWindowText(LoadString(IDW_MAIN));

	TRACE("OnCreate\n");
	return 0;
}


// Здесь будет главная развертка отображения ИПД
void ESView::OnDraw(CDC &dc)
	{
// OnPaint is called automatically whenever a part of the
	// window needs to be repainted.

	// Centre some text in our view window
	CRect rc = GetClientRect();
	rc.top = 180;

	CString cs = "win32 Wrapper Entity Viewer";//LoadString(IDW_MAIN);
	CString cs2 = "@Fixosoft 2019 re build";//LoadString(IDW_MAIN);
	dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    rc.top = 220;
    dc.DrawText(cs2, cs2.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    dc.MoveTo(20,20);
	dc.LineTo(150, 20);
    dc.LineTo(150, 150);
    dc.LineTo(20, 20);

	} // end OnDraw
	  
	  

void ESView::OnSize()
{
	// Force the window to be repainted during resizing
	Invalidate();
}


void ESView::PreCreate(CREATESTRUCT& cs)
{
	// This function will be called automatically by Create. It provides an
	// opportunity to set various window parameters prior to window creation.
	// You are not required to set these parameters, any paramters which
	// aren't specified are set to reasonable defaults.

	// Set some optional parameters for the window
	cs.dwExStyle = WS_EX_CLIENTEDGE;		// Extended style
	cs.lpszClass = _T("View Window");		// Window Class
	cs.x = 50;								// top x
	cs.y = 50;								// top y
	cs.cx = 400;							// width
	cs.cy = 300;							// height
}

void ESView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to be done after the window is created go here.

	TRACE("OnInitialUpdate\n");
}


void ESView::OnDestroy()
{
	// End the application when the window is destroyed
	::PostQuitMessage(0);
}

// Святая святых win32 api - оконная процедура
LRESULT ESView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// This function is our message procedure. We process the messages for
	// the view window here.  Unprocessed messages are passed on for
	//  default processing.

	switch(uMsg)
	{
	case WM_SIZE:  OnSize();// определен в WinUser.h :#define WM_SIZE                         0x0005

	case WM_LBUTTONDOWN: return OnLButtonDown(uMsg, wParam, lParam);
	case WM_MOUSEMOVE:	 return OnMouseMove(uMsg, wParam, lParam);     
    case WM_LBUTTONUP:	 return OnLButtonUp(uMsg, wParam, lParam);
	case WM_MOUSEHWHEEL: ;
	case WM_MOUSEWHEEL:  ;

		break;	// and also do default processing for this message
	}

	// отправим  необработанные на default processing
	return WndProcDefault(uMsg, wParam, lParam);
}


void ESView::DrawLine(int x, int y) 
	{
	CClientDC dc(*this);
	/*//Прикрыл, как будем рисовать:
	  dc.MoveTo(m_OldPt.x, m_OldPt.y);
	  dc.LineTo(x, y);
	*/
	};
	  
void ESView::Point(int x, int y, int Radius, bool Filled) 
	{
	CClientDC dc(*this);

	//dc.Ellipse(

	};

LRESULT ESView::OnLButtonDown(UINT, WPARAM, LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();

	// StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);

	return 0L;
}

LRESULT ESView::OnLButtonUp(UINT, WPARAM, LPARAM lParam)
{
	//Release the capture on the mouse
	ReleaseCapture();

   //	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
	
	return 0L;
}


LRESULT ESView::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
	{
	//	DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	//	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
	}

	// По координатам мыши отобразим текст
	CRect rc = GetClientRect();
	rc.top = GET_Y_LPARAM(lParam);
	rc.left = GET_X_LPARAM(lParam);
	char* MouseY = (char*) malloc (sizeof(char)*1);
	itoa(rc.top, MouseY, 10);
	CString csY = (CString) MouseY;

	char* MouseX = (char*) malloc (sizeof(char)*1);
	itoa(rc.left, MouseX, 10);
	CString csX = (CString) MouseX;
	CString cs2 = csY + "; " + csX;

	
	CString cs = "-**0**-" ;//LoadString(IDW_MAIN);
    CClientDC dc(*this); // device context
	dc.DrawText(cs, cs.GetLength(), rc, DT_LEFT|DT_TOP|DT_SINGLELINE);
	
	

	//CRect rc2 = GetClientRect();
	//rc2.top = 0; rc.left = 0;
	//dc.DrawText(cs2 , cs2.GetLength(), rc2, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
// сообщение StatusBar'у:
	int Widths[3] = {150,100,100};
	SendMessage(this->OwnerStatusBarHandle, SB_SETPARTS , 3, (LPARAM) &Widths);
	SendMessage(this->OwnerStatusBarHandle, SB_SETTEXT , 0 , (LPARAM) cs2.c_str());
	SendMessage(this->OwnerStatusBarHandle, SB_SETTEXT , 2, (LPARAM) L"[2]");
	

	return 0L;
}


CViewerApp::CViewerApp()
{
}

BOOL CViewerApp::InitInstance()
{
    //Create the Window типа ESView
    m_View.Create();
    //
	 //Create the Window
    //m_Frame.Create();

	
	return TRUE;
	}



// Definitions for the CStatusBarApp class
CViewerApp2::CViewerApp2()
{
}

CViewerApp2::~CViewerApp2()
{
}

BOOL CViewerApp2::InitInstance()
{
	//Create the Frame Window
	if (m_Frame.Create() == 0)
	{
		// We get here if the Frame creation fails

		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}



#define STATUS_ID 1211

//--------------------------- Definitions for the CMainFrame class--------------------------------
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\StatusBar Demo"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
//	case IDM_FILE_OPEN:		 return OnFileOpen();
//	case IDM_FILE_SAVE:		 return OnFileSave();
	//case IDM_FILE_SAVEAS:	 return OnFileSave();
//	case IDM_FILE_PRINT:	 return OnFilePrint();
	case IDM_FILE_EXIT:		 return OnFileExit();
	case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:	 return OnViewToolBar();
//	case IDM_HELP_ABOUT:	 return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	//SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	//SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	CFrame::OnCreate(cs);

	GetStatusBar().SetWindowLongPtr(GWLP_ID, STATUS_ID);
	m_View.OwnerStatusBarHandle = this->m_MyStatusBar.GetHwnd(); // Спустим хэндл вниз на View
	return 0;
}

LRESULT CMainFrame::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        UNREFERENCED_PARAMETER(uMsg);
   
	
	// Perform the owner draw for Part 3 in the status bar
	LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT) lParam;


	if (pDrawItem->CtlID == STATUS_ID) // Message comes from the status bar
	{
       TRACE("STATUS_ID detected\n");
	   	return TRUE;
		} 
	/*

		CRect rcPart = pDrawItem->rcItem;
		CDC dc;
		dc.Attach(pDrawItem->hDC);
	
		// Set the font to italic
		CFont Font = GetStatusBar().GetFont();
		LOGFONT lf = Font.GetLogFont();
		lf.lfItalic = TRUE;
		dc.CreateFontIndirect(&lf);
		
		// Display the gradient background and text
		dc.GradientFill(RGB(230, 180, 0), RGB(240, 210, 90), rcPart, TRUE);
		dc.SetTextColor(RGB(10,20,250));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rcPart.left,rcPart.top,_T("s-Owner Draw"), 10);
 
	
	}
  */
	// Allow the frame to perform owner drawing menu items.
	return CFrame::OnDrawItem(uMsg, wParam, lParam);
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);

	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileOpen()
{
	// Bring up the dialog, and open the file
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = *this;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
//		Add case statements for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	// This function is called before the frame is created.
	// It provides an opportunity to modify the various CREATESTRUCT
	// parameters used in the frame window's creation.
	
	// Call base clase to set defaults
	CFrame::PreCreate(cs);
	
	// cs.style &= ~WS_VISIBLE;	// Remove the WS_VISIBLE style. The frame will be initially hidden.
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
//	AddToolBarButton( IDM_FILE_NEW   );
  AddToolBarButton( IDM_FILE_EXIT  );
	//AddToolBarButton( IDM_FILE_SAVE  );
	
//	AddToolBarButton( 0 );						// Separator
//	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
//	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
//	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
//	AddToolBarButton( 0 );						// Separator
//	AddToolBarButton( IDM_FILE_PRINT );
	
//	AddToolBarButton( 0 );						// Separator
//	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}


	  } //namespace api
	}