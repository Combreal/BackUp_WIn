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
LRESULT CALLBACK WndProcC(HWND, UINT, WPARAM, LPARAM);
DWORD dwRet;
HWND hWindowa, hWindowb, hWindowc, hwndSrcdir, hwndDestdir, hwndBufreq, hwndEditB, hwndEditF, hwndEditG, hwndButton, hwndButtonB, hwndButtonC, hwndButtonD, hwndButtonE, hwndButtonF, hwndTeam, hwndCheckbox, hwndLabel, hwndLabelB, hwndLabelC, hwndLabelD, hwndLabelE, hwndLabelF, hwndLabelG;
HANDLE hMyIcon, hMyIconB;
bool showWindowB, showWindowC, backupHomedir;
TCHAR NPath[MAX_PATH];
TCHAR username[UNLEN + 1];
DWORD size = UNLEN + 1;
std::string theTeam, buHomedir, password, theUsername;
void CenterWindow(HWND);
void BackUp(HWND hwnd, bool passedBuhomedir, std::string passedSrc, std::string passedDest, int passedFreq, std::string password);
std::string lpwstrToString(LPWSTR var);
LPWSTR stringToLPWSTR(const std::string& instr);
int CALLBACK BrowseForFolderCallback(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData);
std::string BrowseFolders(HWND hwnd, LPSTR lpszFolder, LPSTR lpszTitle);
void displayExcludeList(HWND hwndEdit, std::string passedString);
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
std::ofstream file("backuprc", std::ios::out | std::ios::trunc);
bool DismGetFeatureInfo(std::string passedFeature);
bool dirExists(const std::string& dirName);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	backupHomedir = false;
	showWindowB = false;
	showWindowC = false;
	MSG  msg;    
	WNDCLASSW wc = {0};
	wc.lpszClassName = L"BU";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&wc);
	hWindowa = CreateWindowW(wc.lpszClassName, L"BackUp",	WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE, 60, 134, 528, 184, 0, 0, hInstance, 0);
	WNDCLASSW wcB = {0};
    wcB.lpszClassName = L"EditText";
    wcB.hInstance = hInstance;
    wcB.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wcB.lpfnWndProc = WndProcB;
	wcB.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
    wcB.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&wcB);
    hWindowb = CreateWindowW(wcB.lpszClassName, L"Liste d'exclusion", WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 360, 234, 500, 212, 0, 0, hInstance, 0);
	WNDCLASSW wcC = {0};
    wcC.lpszClassName = L"Password";
    wcC.hInstance = hInstance;
    wcC.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wcC.lpfnWndProc = WndProcC;
	wcC.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
    wcC.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&wcC);
    hWindowc = CreateWindowW(wcC.lpszClassName, L"Identification", WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 360, 234, 358, 135, 0, 0, hInstance, 0);
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(showWindowB)
        {
            ShowWindow(hWindowb, nCmdShow);
        }
        if(!showWindowB)
        {
            ShowWindow(hWindowb, SW_HIDE);
        }
		if(showWindowC)
        {
            ShowWindow(hWindowc, nCmdShow);
        }
        if(!showWindowC)
        {
            ShowWindow(hWindowc, SW_HIDE);
        }
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static wchar_t *homedir =  L"Sauvegarder la homedir Windows Bash";
	static wchar_t *srcdir =  L"Dossier source : ";
	static wchar_t *destdir =  L"Dossier de destination : ";
	static wchar_t *bufreq =  L"Frequence de sauvegarde : ";
	HINSTANCE hInstance = GetModuleHandle(NULL);
    std::string desty = "C:\\Users\\";
    std::string buildy = "/home";
	std::string destyy = "/user/";
	std::string buildyy = "/ses/ses/BackupHome";
	std::string url = "http://fedora-serv/cgi-bin/srv-proj.cgi?user=";
	std::string homedirb = "C:\\Users\\";
	std::string homedirc = "\\AppData\\Local\\lxss\\home\\";
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
		homedirb = homedirb.append(lpwstrToString(username));// + homedirc + lpwstrToString(username)
		homedirb = homedirb.append(homedirc);
		homedirb = homedirb.append(lpwstrToString(username));
		theUsername = lpwstrToString(username);
		CenterWindow(hwnd);
		hMyIcon = LoadImage(hInstance,MAKEINTRESOURCE(IDI_MYICONY), IMAGE_ICON, 23, 23, NULL);
		hMyIconB = LoadImage(hInstance,MAKEINTRESOURCE(IDI_MYICONY), IMAGE_ICON, 23, 23, NULL);
		hwndLabelB = CreateWindowW(L"Static", srcdir, WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 10, 197, 30, hwnd, (HMENU)ID_LABELB, NULL, NULL);
		hwndSrcdir = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 195, 7, 270, 23, hwnd, (HMENU)ID_EDITB, NULL, NULL);
		hwndButtonB = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM | BS_ICON, 475, 4, 26, 26, hwnd, (HMENU)ID_BUTTONB, hInstance, NULL);
		desty = desty + lpwstrToString(username);
        SetWindowText(hwndSrcdir, stringToLPWSTR(desty));
		hwndLabelC = CreateWindowW(L"Static", destdir, WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 40, 197, 30, hwnd, (HMENU)ID_LABELC, NULL, NULL);
		hwndDestdir = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 195, 37, 270, 23, hwnd, (HMENU)ID_EDITC, NULL, NULL);
		hwndButtonC = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_TEXT | BS_BOTTOM | BS_ICON, 475, 34, 26, 26, hwnd, (HMENU)ID_BUTTONC, hInstance, NULL);
		destyy = destyy + lpwstrToString(username) + buildyy;
        SetWindowText(hwndDestdir, stringToLPWSTR(destyy));				
		hwndLabelD = CreateWindowW(L"Static", bufreq, WS_CHILD|WS_VISIBLE|SS_LEFT, 10, 70, 197, 30, hwnd, (HMENU)ID_LABELD, NULL, NULL);
		hwndBufreq = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 195, 67, 40, 23, hwnd, (HMENU)ID_EDITD, NULL, NULL);
		SetWindowText(hwndBufreq, L"2");
		hwndLabelE = CreateWindowW(L"Static", L"H", WS_CHILD | WS_VISIBLE | SS_LEFT, 239, 70, 197, 30, hwnd, (HMENU)ID_LABELE, NULL, NULL);
		hwndButtonD = CreateWindowW(L"Button", L"Liste d'exclusions", WS_VISIBLE | WS_CHILD, 305, 69, 140, 21, hwnd, (HMENU)ID_BUTTOND, NULL, NULL);
		hwndCheckbox = CreateWindowW(L"Button", homedir, BS_CHECKBOX| WS_CHILD | WS_VISIBLE, 9, 95, 295, 30, hwnd, (HMENU)ID_CHECKBOX, NULL, NULL);
		CheckDlgButton(hwnd, ID_CHECKBOX, BST_CHECKED);
		//hwndLabel = CreateWindowW(L"Static", homedir, WS_CHILD|WS_VISIBLE|SS_LEFT, 90, 100, 265, 26, hwnd, (HMENU)1, NULL, NULL);
		hwndButton = CreateWindowW(L"Button", L"Lancer BackUp", WS_VISIBLE | WS_CHILD, 200, 123, 120, 21, hwnd, (HMENU)ID_BUTTON, NULL, NULL);
		SendMessage(hwndButtonB, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIcon);
		SendMessage(hwndButtonC, BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)hMyIconB);
		break;
	case WM_COMMAND:
	{
		
        switch(LOWORD(wParam))
        {
			case ID_CHECKBOX:
			{
				BOOL checked = IsDlgButtonChecked(hwnd, ID_CHECKBOX);
				if(checked)
				{
					CheckDlgButton(hwnd, ID_CHECKBOX, BST_UNCHECKED);
				}	
				else
				{
					CheckDlgButton(hwnd, ID_CHECKBOX, BST_CHECKED);
				}
				break;
			}
            case ID_BUTTON:
			{
				std::string src, dest, frequ, retoo;
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
					//MessageBox(hwnd, stringToLPWSTR(theTeam), L"equipe", MB_OK);
					curl_easy_cleanup(curl);
				}
				int iListBox = 0, freq = 0;
                TCHAR buff[1024];
				TCHAR chBuffer[MAX_PATH];
                GetWindowTextW(hwndSrcdir, buff, 1024);
				src = lpwstrToString(buff);
				GetWindowTextW(hwndDestdir, buff, 1024);
				dest = lpwstrToString(buff);
				GetWindowTextW(hwndBufreq, buff, 1024);
				frequ = lpwstrToString(buff);
				freq = std::stoi(frequ);
				file<<"# serveur destinataire\n";
				retoo = "BKSV=";
				retoo.append(theTeam);
				retoo.append("\n\n");
				file<<retoo;
				file<<"# repertoire recepteur. Le creer sur BKSV s il n existe pas.\n";
				retoo = "BKDESTDIR='";
				retoo.append(dest);
				retoo.append("'\n\n");
				file<<retoo;
				file<<"# repertoires locaux a synchroniser vers BKDESTDIR\n";
				retoo = "BKSRCDIR='";
				retoo.append(src);
				retoo.append("'\n\n");
				file<<retoo;
				file<<"# BKDESTUSER=${USERNAME}			    # loginname a faire paraitre dans l article syslog.\n";
				file<<"# MAILDEST=${USERNAME}        			# Adresse mail recepteur des Mails\n";
				file<<"# SMTPRELAY='smtp.inria.fr' 			# serveur smtp pour envoie du mail d'erreur\n";
				file<<"# MAILDOMAIN='inria.fr'\n";
				file<<"# MAILDEST=${USERNAME}@sophia.inria.fr  #\n";
				file<<"# MAILFROM=${USERNAME}@sophia.inria.fr  #\n";
				if(IsDlgButtonChecked(hwnd, ID_CHECKBOX))
				{
					file<<homedirb;
					file<<"\n";
					backupHomedir = true;
				}
				file.close();
				BackUp(hwnd, backupHomedir, src, dest, freq, password);
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
	std::ifstream fileD("bkexclude_list");
	if(!fileD.good())
	{
		std::ofstream fileF("bkexclude_list", std::ios::out | std::ios::trunc);
		fileF<<"NTUSER.DAT"<<std::endl;
		fileF<<"ntuser.dat*"<<std::endl;
		fileF<<"UsrClass.dat*"<<std::endl;
		fileF<<"UsrClass"<<std::endl;
		fileF<<"pagefile.sys"<<std::endl;
		fileF<<"pagefil2.sys"<<std::endl;
		fileF<<"*.lnk"<<std::endl;
		fileF<<"*.lock"<<std::endl;
		fileF<<"System*/"<<std::endl;
		fileF<<"/**/Cache/"<<std::endl;
		fileF<<"/**/Cache.Trash"<<std::endl;
		fileF.close();
	}
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
                std::ofstream fileB("bkexclude_list", std::ios::out | std::ios::trunc);//std::ios::trunc //std::ios_base::app
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

LRESULT CALLBACK WndProcC(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
			hwndLabelF = CreateWindowW(L"Static", L"Identifiant : ", WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 10, 152, 30, hwnd, (HMENU)ID_LABELF, NULL, NULL);
			hwndEditF = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 8, 220, 23, hwnd, (HMENU)ID_EDITF, NULL, NULL);
			hwndLabelG = CreateWindowW(L"Static", L"Mot de passe : ", WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 40, 152, 30, hwnd, (HMENU)ID_LABELG, NULL, NULL);
			hwndEditG = CreateWindowW(L"Edit", NULL, ES_PASSWORD |WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 37, 220, 23, hwnd, (HMENU)ID_EDITG, NULL, NULL);
			hwndButtonF = CreateWindowW(L"Button", L"S'identifier", WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, 120, 68, 120, 26, hwnd, (HMENU)ID_BUTTONF, NULL, NULL);
			SetWindowText(hwndEditF, stringToLPWSTR(theUsername));
            break;
        case WM_COMMAND:
            if(LOWORD(wParam)==ID_BUTTONF)
            {
				TCHAR buffy[1024];
                GetWindowTextW(hwndEditG, buffy, 1024);
                password = lpwstrToString(buffy);
				showWindowC = false;
				SetActiveWindow(hWindowa);
            }
            break;
        case WM_CLOSE:
            showWindowC = false;
            SetActiveWindow(hWindowa);
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

void BackUp(HWND hwnd, bool passedBuhomedir, std::string passedSrc, std::string passedDest, int passedFreq, std::string password)
{
	showWindowC = true;
	SetActiveWindow(hWindowc);
	std::stringstream ss;
	ss << passedFreq;
	std::string freq = ss.str();
	std::string command = "\"xcopy /r /d /i /s /y /exclude:bkexclude_list ";
	command.append(passedSrc);
	command.append(" ");
	command.append(passedDest);
	command.append("\"");
	std::string task = "schtasks /Create /SC HOURLY /MO  /TN \"BackUp\" /TR  /ST 16:00"; //MO= passedFreq TR= command
	task.insert(32, freq);
	task.insert(task.find("TR")+3, command);
	if(!dirExists(passedSrc))
	{
		MessageBoxW(hwnd, L"Le dossier source est inaccessible", L"Message", MB_OKCANCEL);
	}
	if(!dirExists(passedDest))
	{
		//MessageBoxW(hwnd, L"Le dossier de destination est inaccessible", L"Message", MB_OKCANCEL);
	}
	if(!(1 <= passedFreq <= 744))
	{
		MessageBoxW(hwnd, L"La frequence spécifiée est incorrecte", L"Message", MB_OKCANCEL);
	}
	if(passedSrc == passedDest)
	{
		MessageBoxW(hwnd, L"Les dossiers source et destination sont identique", L"Message", MB_OKCANCEL);
	}
	if(!DismGetFeatureInfo("Microsoft-Windows-Subsystem-Linux")) //poussez les tests plus loin - compte par default configure
	{
		MessageBoxW(hwnd, L"Windows Bash n'est pas installé. XCOPY va être utilisé", L"Message", MB_OKCANCEL);
	}
}

std::string lpwstrToString(LPWSTR input)
{
    int cSize = WideCharToMultiByte (CP_ACP, 0, input, wcslen(input), NULL, 0, NULL, NULL);
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
    std::ifstream inFileB("bkexclude_list");
	std::string reBuildString;
    while(getline(inFileB, passedString, '\n'))
    {
        OutputDebugString(stringToLPWSTR(passedString));
		reBuildString = reBuildString + passedString + "\r\n";
    }
	SetWindowText(hwndEdit, stringToLPWSTR(reBuildString));
    inFileB.close();
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool DismGetFeatureInfo(std::string passedFeature)
{
	bool enabled = false;
	std::string status, command;
	command = " /C dism /online /get-featureinfo /featurename: | findstr /R /C:\"State\" > featureStatus.txt";
	command.insert(47,passedFeature);
	ShellExecuteW(NULL, L"open", L"cmd.exe", stringToLPWSTR(command), NULL, SW_HIDE);
	std::ifstream file("featureStatus.txt");
	getline(file, status, '\n');
	if(status.find("Enabled") != std::string::npos)
	{
		enabled = true;
	}
	file.close();
	remove("featureStatus.txt");
	return enabled;
}

bool dirExists(const std::string& dirName)
{
	DWORD path = GetFileAttributesA(dirName.c_str());
	if (path == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	if (path & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
}