#include <windows.h>
#include <ctime>
#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HWND hwndTeam;
static HWND hwndSrcdir;
static HWND hwndDestdir;
static HWND hwndBufreq;
static HWND hwndLabel;
HICON hMyIcon;
void CenterWindow(HWND);
void BackUp();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	    srand(time(NULL));
		MSG  msg;    
		WNDCLASSW wc = {0};
		wc.lpszClassName = L"BU";
		wc.hInstance     = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpfnWndProc   = WndProc;
		wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
		wc.hCursor       = LoadCursor(0, IDC_ARROW);
		RegisterClassW(&wc);
		CreateWindowW(wc.lpszClassName, L"BackUp",	WS_OVERLAPPEDWINDOW|WS_VISIBLE, 60, 134, 522, 200, 0, 0, hInstance, 0);  
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
	HWND hwndButton;
	HWND hwndButtonB;
	HWND hwndButtonC;
	HWND hwndButtonD;
	HWND hwndButtonE;
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
		CenterWindow(hwnd);

		hMyIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MYICONY));

		CreateWindowW(L"Static", team, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 12, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndTeam=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 9, 270, 23, hwnd, (HMENU)ID_EDIT, NULL, NULL);
		hwndButton=CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM, 475, 10, 20, 20, hwnd, (HMENU)ID_BUTTON, hInstance, NULL);
		
		CreateWindowW(L"Static", srcdir, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 40, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndSrcdir=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 37, 270, 23, hwnd, (HMENU)ID_EDITB, NULL, NULL);
		hwndButtonB=CreateWindowW(L"Button", L"", WS_VISIBLE|WS_CHILD, 475, 38, 20, 20, hwnd, (HMENU)ID_BUTTONB, NULL, NULL);
		
		CreateWindowW(L"Static", destdir, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 70, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndDestdir=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 67, 270, 23, hwnd, (HMENU)ID_EDITC, NULL, NULL);
		hwndButtonC=CreateWindowW(L"Button", L"", WS_VISIBLE|WS_CHILD, 475, 68, 20, 20, hwnd, (HMENU)ID_BUTTONC, NULL, NULL);
		
		CreateWindowW(L"Static", bufreq, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 100, 197, 30, hwnd, (HMENU)1, NULL, NULL);
		hwndBufreq=CreateWindowW(L"Edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 195, 97, 270, 23, hwnd, (HMENU)ID_EDITD, NULL, NULL);
		hwndButtonD=CreateWindowW(L"Button", L"", WS_VISIBLE|WS_CHILD, 475, 98, 20, 20, hwnd, (HMENU)ID_BUTTOND, NULL, NULL);
		
		//hMyIcon=LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICONY));
		SendMessage(hwndButton, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIcon);

		hwndButtonE=CreateWindowW(L"Button", L"Lancer BackUp", WS_VISIBLE|WS_CHILD, 190, 130, 120, 21, hwnd, (HMENU)ID_BUTTONE, NULL, NULL);
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
