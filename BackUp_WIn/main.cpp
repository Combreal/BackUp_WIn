#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <Lmcons.h>
#include <string>
#include <fstream>
#include <sstream>
#include <shlobj.h>
#include <algorithm>
#include <tchar.h>
#include <curl.h> 

#pragma comment(lib, "libcurl_a.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcB(HWND, UINT, WPARAM, LPARAM);
DWORD dwRet;
HWND hWindowa, hWindowb, hwndSrcdir, hwndDestdir, hwndBufreq, hwndEditB, hwndButton, hwndButtonB, hwndButtonC, hwndButtonD, hwndButtonE, hwndTeam, hWndListBox, hwndLabel, hwndLabelB, hwndLabelC, hwndLabelD, hwndLabelE;
HANDLE hMyIcon, hMyIconB;
bool showWindowB;
TCHAR NPath[MAX_PATH];
TCHAR username[UNLEN + 1];
DWORD size = UNLEN + 1;
std::string theTeam;
void CenterWindow(HWND);
void BackUp();
std::string lpwstrToString(LPWSTR var);
LPWSTR stringToLPWSTR(const std::string& instr);
int CALLBACK BrowseForFolderCallback(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData);
std::string BrowseFolders(HWND hwnd, LPSTR lpszFolder, LPSTR lpszTitle);
void displayExcludeList(HWND hwndEdit, std::string passedString);
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
std::ofstream file("backuprc", std::ios::out | std::ios::trunc);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	showWindowB = false;
	MSG  msg;    
	WNDCLASSW wc = {0};
	wc.lpszClassName = L"BU";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&wc);
	hWindowa = CreateWindowW(wc.lpszClassName, L"BackUp",	WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE, 60, 134, 528, 153, 0, 0, hInstance, 0);
	WNDCLASSW wcB = {0};
    wcB.lpszClassName = L"EditText";
    wcB.hInstance = hInstance;
    wcB.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wcB.lpfnWndProc = WndProcB;
    wcB.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&wcB);
    hWindowb = CreateWindowW(wcB.lpszClassName, L"Liste d'exclusion", WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 360, 234, 500, 212, 0, 0, hInstance, 0);
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(showWindowB)
        {
            ShowWindow(hWindowb, nCmdShow);
        }
        else if(!showWindowB)
        {
            ShowWindow(hWindowb, SW_HIDE);
        }
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static wchar_t *homedir =  L"Sauvegarder la homedir Windown Bash :";
	static wchar_t *srcdir =  L"Dossier source : ";
	static wchar_t *destdir =  L"Dossier de destination : ";
	static wchar_t *bufreq =  L"Frequence de sauvegarde : ";
	HINSTANCE hInstance = GetModuleHandle(NULL);
    std::string desty = "/user/";
    std::string buildy = "/home";
	std::string destyy = "/user/";
	std::string buildyy = "/ses/ses/BackupHome";
	std::string url = "http://fedora-serv/cgi-bin/srv-proj.cgi?user=";
    static wchar_t *pathy = L"C:\\";
	static wchar_t *info = L"Selectionner le dossier pour la sauvegarde";
	std::string source, destination, curlTeam;
	switch(msg)
	{
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
		{
			int ret = MessageBoxW(hwnd, L"Etes vous sur de vouloir quitter?", L"Message", MB_OKCANCEL);                            
			if(ret == IDOK)
			{
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}     
		break;
	case WM_CREATE:
		GetUserNameW((TCHAR*)username, &size);
		CenterWindow(hwnd);
		hMyIcon = LoadImage(hInstance,MAKEINTRESOURCE(IDI_MYICONY), IMAGE_ICON, 23, 23, NULL);
		hMyIconB = LoadImage(hInstance,MAKEINTRESOURCE(IDI_MYICONY), IMAGE_ICON, 23, 23, NULL);
		hwndLabel = CreateWindowW(L"Static", homedir, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 12, 197, 30, hwnd, (HMENU)1, NULL, NULL);	
		hwndLabelB = CreateWindowW(L"Static", srcdir, WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 12, 197, 30, hwnd, (HMENU)ID_LABELB, NULL, NULL);
		hwndSrcdir = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 195, 10, 270, 23, hwnd, (HMENU)ID_EDITB, NULL, NULL);
		desty = desty + lpwstrToString(username) + buildy;
        SetWindowText(hwndSrcdir, stringToLPWSTR(desty));
		hwndButtonB = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM | BS_ICON, 475, 5, 26, 26, hwnd, (HMENU)ID_BUTTONB, hInstance, NULL);
		hwndLabelC = CreateWindowW(L"Static", destdir, WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 40, 197, 30, hwnd, (HMENU)ID_LABELC, NULL, NULL);
		hwndDestdir = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 195, 37, 270, 23, hwnd, (HMENU)ID_EDITC, NULL, NULL);
		destyy = destyy + lpwstrToString(username) + buildyy;
        SetWindowText(hwndDestdir, stringToLPWSTR(destyy));
		hwndButtonC = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM | BS_ICON, 475, 35, 26, 26, hwnd, (HMENU)ID_BUTTONC, hInstance, NULL);		
		hwndLabelD = CreateWindowW(L"Static", bufreq, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 68, 197, 30, hwnd, (HMENU)ID_LABELD, NULL, NULL);
		hwndBufreq = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 195, 65, 40, 23, hwnd, (HMENU)ID_EDITD, NULL, NULL);
		SetWindowText(hwndBufreq, L"2");
		hwndLabelE = CreateWindowW(L"Static", L"H", WS_CHILD | WS_VISIBLE | SS_LEFT, 239, 65, 197, 30, hwnd, (HMENU)ID_LABELE, NULL, NULL);
		hwndButtonD = CreateWindowW(L"Button", L"Liste d'exclusions", WS_VISIBLE | WS_CHILD, 305, 68, 140, 21, hwnd, (HMENU)ID_BUTTOND, NULL, NULL);
		hwndButton = CreateWindowW(L"Button", L"Lancer BackUp", WS_VISIBLE | WS_CHILD, 200, 95, 120, 21, hwnd, (HMENU)ID_BUTTON, NULL, NULL);
		SendMessage(hwndButtonB, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIcon);
		SendMessage(hwndButtonC, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIconB);
		break;
	case WM_COMMAND:
	{
        switch(LOWORD(wParam))
        {
            case ID_BUTTON:
			{
				CURL *curl;
				CURLcode res;
				curl_global_init(CURL_GLOBAL_DEFAULT);
				curl = curl_easy_init();
				if(curl)
				{
					url.append(lpwstrToString(username));
					curl_easy_setopt(curl, CURLOPT_URL, url);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &theTeam);
					res = curl_easy_perform(curl);
					MessageBox(hwnd, stringToLPWSTR(theTeam), L"equipe", MB_OK);
					curl_easy_cleanup(curl);
				}
				//BackUp();
				int iListBox = 0;
                TCHAR buff[1024];
				TCHAR chBuffer[MAX_PATH];
				iListBox = SendMessage(hWndListBox, LB_GETCURSEL, 0, 0L);
				SendMessage(hWndListBox, LB_GETTEXT, iListBox, (LPARAM)chBuffer);
				std::string team = lpwstrToString((TCHAR*)chBuffer);
				std::transform(team.begin(), team.end(), team.begin(), ::tolower);
				OutputDebugString((TCHAR*)chBuffer);
                file<<team;
                file<<"\n";
                GetWindowTextW(hwndSrcdir, buff, 1024);
                file<<lpwstrToString(buff);
                file<<"\n";
				GetWindowTextW(hwndDestdir, buff, 1024);
                file<<lpwstrToString(buff);
                file<<"\n";
				GetWindowTextW(hwndBufreq, buff, 1024);
                file<<lpwstrToString(buff);
                file<<"\n";
                file.close();
                break;
			}
            case ID_BUTTONB:
                source = BrowseFolders(hwnd, (LPSTR)pathy, (LPSTR)info);
				if(!source.empty())
				{
					SetWindowText(hwndSrcdir, stringToLPWSTR(source));
					RedrawWindow(hwndSrcdir, NULL, NULL, RDW_ERASE);
				}
				else
					MessageBoxW(hwnd, L"Chemin incorrect", L"Message", MB_OKCANCEL); 
                break;
            case ID_BUTTONC:
                destination = BrowseFolders(hwnd, (LPSTR)pathy, (LPSTR)info);
				if(!destination.empty())
				{
					SetWindowText(hwndDestdir, stringToLPWSTR(destination));
					RedrawWindow(hwndDestdir, NULL, NULL, RDW_ERASE);
				}
				else
					MessageBoxW(hwnd, L"Chemin incorrect", L"Message", MB_OKCANCEL); 
                break;
			case ID_BUTTOND:
                showWindowB = true;
                SetActiveWindow(hWindowb);
                break;
        }
        break;
    }
	case WM_DESTROY: 
		DestroyIcon((HICON)hMyIcon);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProcB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::string excCont;
    switch(msg)
    {
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            }    
            break;
        case WM_CREATE:
            CenterWindow(hwnd);
            hwndEditB = CreateWindowW(L"Edit", NULL, ES_MULTILINE | ES_WANTRETURN | WS_HSCROLL | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS, 11, 10, 472, 145, hwnd, (HMENU)ID_EDITB, GetModuleHandle(NULL), NULL); //ES_AUTOVSCROLL //ES_WANTRETURN
            hwndButtonE = CreateWindowW(L"Button", L"Sauvegarder", WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, 195, 158, 95, 21, hwnd, (HMENU)ID_BUTTONE, GetModuleHandle(NULL), NULL);
            displayExcludeList(hwndEditB, excCont);
            break;
        case WM_COMMAND:
            if(LOWORD(wParam)==ID_BUTTONE)
            {
                std::ofstream fileB("excludelist.txt", std::ios::out | std::ios::trunc);//std::ios::trunc //std::ios_base::app
                TCHAR buffy[1024];
                GetWindowTextW(hwndEditB, buffy, 1024);
                std::string excludelist = lpwstrToString(buffy);
                //excludelist.erase(std::remove(excludelist.begin(), excludelist.end(), '\n'), excludelist.end());
                fileB<<excludelist;
                fileB<<"\r\n";
                fileB.close();
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            }
            break;
        case WM_CLOSE:
            showWindowB = false;
            SetActiveWindow(hWindowa);
            return 0;
            break;
        default:
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void CenterWindow(HWND hwnd)
{
	RECT rc = {0};
	GetWindowRect(hwnd, &rc);
	int win_w = rc.right - rc.left;
	int win_h = rc.bottom - rc.top;
	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w)/2, (screen_h - win_h)/2, 0, 0, SWP_NOSIZE);
}

void BackUp()
{

}

std::string lpwstrToString(LPWSTR input)
{
    int cSize = WideCharToMultiByte(CP_ACP, 0, input, wcslen(input), NULL, 0, NULL, NULL);
    std::string output(static_cast<size_t>(cSize), '\0');
    WideCharToMultiByte (CP_ACP, 0, input, wcslen(input), reinterpret_cast<char*>(&output[0]), cSize, NULL, NULL);
    return output;
}
 
LPWSTR stringToLPWSTR(const std::string& instr)
{
    int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);
    LPWSTR widestr = new WCHAR[bufferlen + 1];
    ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);
    widestr[bufferlen] = 0;
    return widestr;
}
 
int CALLBACK BrowseForFolderCallback(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData)
{
    TCHAR szPath[MAX_PATH];
    switch(uMsg)
    {
        case BFFM_INITIALIZED:
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
            break;
        case BFFM_SELCHANGED:
            if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szPath))
            {
                SendMessage(hwnd, BFFM_SETSTATUSTEXT,0,(LPARAM)szPath);
            }
            break;
    }
    return 0;
}
 
std::string BrowseFolders(HWND hwnd, LPSTR lpszFolder, LPSTR lpszTitle)
{
    char szPath[MAX_PATH + 1];
    BROWSEINFO bi;
    LPITEMIDLIST pidl;
    std::string lResult;
    LPMALLOC pMalloc;
    if(SUCCEEDED(SHGetMalloc(&pMalloc)))
    {
        bi.hwndOwner = hwnd;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = NULL;
        bi.lpszTitle = (LPWSTR)lpszTitle;
        bi.ulFlags = BIF_STATUSTEXT;
        bi.lpfn = BrowseForFolderCallback;
        bi.lParam = (LPARAM)lpszFolder;    
        pidl = SHBrowseForFolder(&bi);
        if (pidl)
        {
            if (SHGetPathFromIDList(pidl,(LPWSTR)szPath))
            {
                OutputDebugString((LPWSTR)szPath);
                lResult = lpwstrToString((LPWSTR)szPath);
            }
           pMalloc->Free(pidl);
           pMalloc->Release();                     
        }
    }
    return lResult;
}
 
void displayExcludeList(HWND hwndEdit, std::string passedString)
{
    std::ifstream inFileB("excludelist.txt");//, std::ios::in
	std::string reBuildString;
    while(getline(inFileB, passedString, '\n'))
    {
        OutputDebugString(stringToLPWSTR(passedString));
		reBuildString = reBuildString + passedString + "\n";
    }
	SetWindowText(hwndEdit, stringToLPWSTR(reBuildString));
    inFileB.close();
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}