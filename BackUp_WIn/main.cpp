#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <Lmcons.h>
#include <string>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HWND hwndTeam;
static HWND hwndSrcdir;
static HWND hwndDestdir;
static HWND hwndBufreq;
static HWND hwndLabel;
HANDLE hMyIcon;
HANDLE hMyIconB;
void CenterWindow(HWND);
void BackUp();
/*static wchar_t username[UNLEN+1];
DWORD username_len = UNLEN+1;
GetUserNameW(username, &username_len);*/
TCHAR username[UNLEN + 1];
DWORD size = UNLEN + 1;
//int size = UNLEN + 1;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
		MSG  msg;    
		WNDCLASSW wc = {0};
		wc.lpszClassName = L"BU";
		wc.hInstance     = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpfnWndProc   = WndProc;
		wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
		wc.hCursor       = LoadCursor(0, IDC_ARROW);
		RegisterClassW(&wc);
		CreateWindowW(wc.lpszClassName, L"BackUp",	WS_OVERLAPPEDWINDOW|WS_VISIBLE, 60, 134, 528, 200, 0, 0, hInstance, 0);  
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static wchar_t *team =  L"Nom de l'equipe :";
	static wchar_t *srcdir =  L"Dossier source : ";
	static wchar_t *destdir =  L"Dossier de destination : ";
	static wchar_t *bufreq =  L"Frequence de sauvegarde : ";
	/*wchar_t srcPath[256];
	wchar_t *srcPathB = L"/user/";
	wchar_t *srcPathC = L"/home";
	wchar_t destPath[256];*/
	std::wstring a = L"/user/";
    std::wstring b = L"/home";
    std::wstring c = a + b;
	HWND hwndButton;
	HWND hwndButtonB;
	HWND hwndButtonC;
	HWND hwndButtonD;
	HWND hwndButtonE;
	HWND hWndListBox;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	switch(msg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			int ret = MessageBoxW(hwnd, L"Etes vous sur de vouloir quitter?", 
				L"Message", MB_OKCANCEL);                            
			if (ret == IDOK) {
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}     
		break; 
	case WM_CREATE:
		GetUserNameW((TCHAR*)username, &size);
		//wcscat(srcPathB, username);
		//wcscat(srcPathB, srcPathC);
		CenterWindow(hwnd);
		hMyIcon=LoadImage(hInstance,MAKEINTRESOURCE(IDI_MYICONY), IMAGE_ICON, 23, 23, NULL);
		hMyIconB=LoadImage(hInstance,MAKEINTRESOURCE(IDI_MYICONY), IMAGE_ICON, 23, 23, NULL);
		CreateWindowW(L"Static", team, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 12, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hWndListBox=CreateWindowW(L"Listbox", NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_NOTIFY, 195, 9, 120, 23, hwnd, (HMENU)ID_LISTBOX_TEXT, NULL, NULL);
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 0, (LPARAM)L"ABS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 1, (LPARAM)L"ACUMES");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 2, (LPARAM)L"AROMATH");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 3, (LPARAM)L"ATHENA");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 4, (LPARAM)L"BIOCORE");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 5, (LPARAM)L"BIOVISION");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 6, (LPARAM)L"CAMIN");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 7, (LPARAM)L"CASTOR");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 8, (LPARAM)L"COATI");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 9, (LPARAM)L"COFFEE");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 10, (LPARAM)L"DATASHAPE");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 11, (LPARAM)L"DIANA");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 12, (LPARAM)L"ECUADOR");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 13, (LPARAM)L"EPIONE");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 14, (LPARAM)L"FACTAS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 15, (LPARAM)L"FOCUS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 16, (LPARAM)L"GRAPHDECO");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 17, (LPARAM)L"GRAPHIK");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 18, (LPARAM)L"HEPHAISTOS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 19, (LPARAM)L"INDES");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 20, (LPARAM)L"KAIROS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 21, (LPARAM)L"LEMON");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 22, (LPARAM)L"MARELLE");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 23, (LPARAM)L"MATHNEURO");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 24, (LPARAM)L"MCTAO");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 25, (LPARAM)L"MORPHEME");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 26, (LPARAM)L"NACHOS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 27, (LPARAM)L"NEO");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 28, (LPARAM)L"STARS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 29, (LPARAM)L"TITANE");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 30, (LPARAM)L"TOSCA");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 31, (LPARAM)L"WIMMICS");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 32, (LPARAM)L"DREAM");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 33, (LPARAM)L"DSI");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 34, (LPARAM)L"SAF");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 35, (LPARAM)L"SEMIR");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 36, (LPARAM)L"SMGRH");
		SendMessage(GetDlgItem(hwnd, ID_LISTBOX_TEXT), LB_ADDSTRING, 37, (LPARAM)L"SRH");	
		CreateWindowW(L"Static", srcdir, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 40, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndSrcdir=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 37, 270, 23, hwnd, (HMENU)ID_EDITB, NULL, NULL);
		hwndButtonB=CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM | BS_ICON, 475, 35, 26, 26, hwnd, (HMENU)ID_BUTTONB, hInstance, NULL);
		SetWindowText(hwndSrcdir, username);
		CreateWindowW(L"Static", destdir, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 70, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndDestdir=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 67, 270, 23, hwnd, (HMENU)ID_EDITC, NULL, NULL);
		hwndButtonC=CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM | BS_ICON, 475, 65, 26, 26, hwnd, (HMENU)ID_BUTTONC, hInstance, NULL);		
		CreateWindowW(L"Static", bufreq, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 100, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndBufreq=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 97, 40, 23, hwnd, (HMENU)ID_EDITD, NULL, NULL);
		CreateWindowW(L"Static", L"H", WS_CHILD|WS_VISIBLE|SS_LEFT, 239, 100, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndButtonE=CreateWindowW(L"Button", L"Lancer BackUp", WS_VISIBLE|WS_CHILD, 200, 130, 120, 21, hwnd, (HMENU)ID_BUTTONE, NULL, NULL);
		SendMessage(hwndButtonB, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIcon);
		SendMessage(hwndButtonC, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIconB);
		break;   
	case WM_COMMAND:
		if (LOWORD(wParam)==ID_BUTTON) {
			//SetWindowText(hwndLabel, str);
			RedrawWindow(hwndLabel, NULL, NULL, RDW_ERASE);
		}
		break;
	case WM_DESTROY: 
		DestroyIcon((HICON)hMyIcon);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void CenterWindow(HWND hwnd) {
	RECT rc = {0};
	GetWindowRect(hwnd, &rc);
	int win_w = rc.right - rc.left;
	int win_h = rc.bottom - rc.top;
	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w)/2, (screen_h - win_h)/2, 0, 0, SWP_NOSIZE);
}

void BackUp() {

}
