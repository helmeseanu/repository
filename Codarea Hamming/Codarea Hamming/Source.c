#include <windows.h>
#include "resource.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ID_FILE_EXIT 9001
#define ID_FILE_DIALOG 9003
#define ID_FILE_DIALOG1 9004
#define ID_STUFF_GO 9002

const char g_szClassName[] = "myWindowClass";
//////////////////////CODARE SIMPLA////////////////////////
//////////////////////////////////////////////////////////

typedef struct date
{
	int vectorbinar[32];
	int *vectorhamming;
	int nrbiti;
}date;

int trsfbin(int val,int *v)
{
	int i=1;
	while (val)
	{
		v[i] = val % 2;
		val = val / 2;
		i++;
	}
	return i-1;
}

void CodareSimplaHamming(date caracter)//codare in care se tripleaza fiecare bit(detectie si corectie de erori unare)
{
	int i,count,j=1;
	for (i = 1; i <= caracter.nrbiti; i++)
	{
		count = 3;
		while (count)
		{
			caracter.vectorhamming[j] = caracter.vectorbinar[i];
			j++;
			count--;
		}
	}
}

void generareerori(date caracter)
{
	int ok,pozitieerr,i,okerr;
	ok = rand() % 2;
	if (ok == 1)
	{
		for (i = 1; i <= caracter.nrbiti * 3; i += 3)
		{
			okerr = rand() % 2;//se stabileste daca pe bitul respectiv se genereaza sau nu o eroare
			if (okerr == 1)
			{
				pozitieerr = rand() % 3;//se stabileste pozitia bitului eronat
				if (caracter.vectorhamming[i + pozitieerr] == 0) caracter.vectorhamming[i + pozitieerr] = 1;//bit 0 devine bit 1(eroare)
				else caracter.vectorhamming[i + pozitieerr] = 0; //bit 1 devine bit 0(eroare)
			}
		}
	}
}

char detecterr(date caracter)
{
	int i,s,put=1,ASCII=0,j;
	for (i = 1; i <= caracter.nrbiti * 3; i += 3)
	{
		s = 0;
		for (j = 0; j <= 2; j++)
		{
			s += caracter.vectorhamming[i + j];
		}
		if (s != 0 && s != 3) return 'E';
		else
		{
			if (s==3) ASCII +=put;
		}
		put = put * 2;
	}
	return (char)ASCII;
}

char corectieerori(date caracter)
{
	int i, s, put = 1, ASCII = 0, j;
	for (i = 1; i <= caracter.nrbiti * 3; i += 3)
	{
		s = 0;
		for (j = 0; j <= 2; j++)
		{
			s += caracter.vectorhamming[i + j];
		}
		if (s == 2 || s == 3)
		{
			ASCII += put;
		}
		put = put * 2;
	}
	return (char)ASCII;
}

BOOL CALLBACK dialogcodaresimpla(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}
	case WM_COMMAND: switch (LOWORD(wParam))
	{
		case IDCANCEL: {EndDialog(hwnd, IDCANCEL); break; }
		case IDC_BUTTON1:
		{
		    int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT));
			char *sir,*sirerori,*sircorectat;
			if(len==0)MessageBox(hwnd, "Nu ai introdus nimic!", "Atentionare", MB_OK);
			else
			{
				sir = (char *)malloc(len + 1);
				sirerori = (char *)malloc(len + 1);
				sircorectat = (char *)malloc(len + 1);
				GetDlgItemText(hwnd, IDC_TEXT, sir, len + 1);
				int i;
				date *mesaj;
				mesaj = (date *)malloc(len*sizeof(date));
				for (i = 0; i < len; i++)
				{
					mesaj[i].nrbiti = trsfbin((int)sir[i],mesaj[i].vectorbinar);//codul ASCII al fiecarui caracter este transformat binar
					mesaj[i].vectorhamming = (int*)malloc(3*mesaj[i].nrbiti*sizeof(int));//este alocata memorie pentru vectorul hamming
					CodareSimplaHamming(mesaj[i]);//se genereaza vectorul hamming(vector in care fiecare bit este triplat)
					generareerori(mesaj[i]);//sunt generate erori in vectorul hamming(simularea canalului cu zgomot)
					sirerori[i] = detecterr(mesaj[i]);
					sircorectat[i] = corectieerori(mesaj[i]);
				}
				sirerori[len] = 0;
				sircorectat[len] = 0;
				SetDlgItemText(hwnd, IDC_TEXT2, sirerori);
				SetDlgItemText(hwnd, IDC_TEXT3, sircorectat);
			}
			break;
		}
	}
	break;
	default: return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////

//////////////////////HAMMING (12,8)/////////////
////////////////////////////////////////////////
typedef struct caracter
{
	int vectorbinar[9];
	int vectorhamming[13];
}caracter;

void transfbin(int val, int *v)
{
	int i = 1;
	while (val)
	{
		v[i] = val % 2;
		val = val / 2;
		i++;
	}
}

BOOL CALLBACK hamming12_8(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}
	case WM_COMMAND: switch (LOWORD(wParam))
	{
	case IDCANCEL: {EndDialog(hwnd, IDCANCEL); break; }
	case IDC_BUTTON1: {EndDialog(hwnd, IDCANCEL); break; }
	case IDC_BUTTON2:
	{
		int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1));
		if (len == 0)MessageBox(hwnd, "Nu ai introdus nimic!", "Atentionare", MB_OK);
		else
		{
			caracter *vect;
			char *sir, *sirerori, *sircorectat;
			char *sirbinar;
			sir = (char*)malloc(len + 1);
			sirbinar = (char*)malloc(9 * len + 1);
			vect = (caracter*)malloc(len*sizeof(caracter));
			for (int i = 0; i < len; i++)	transfbin(sir[i], vect[i].vectorbinar);

		}
		break;
	}
	}
	break;
	default: return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////
////////////////////////////////////////////////


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE: DestroyWindow(hwnd); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT: DestroyWindow(hwnd); break;
		case ID_STUFF_GO: MessageBox(hwnd, "Bravo!!", "mesaj", MB_OK); break;
		case ID_FILE_DIALOG:
		{
			int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, dialogcodaresimpla);
			if (ret == -1)
			{
				MessageBox(hwnd, "Dialog failed!", "Error", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
		case ID_FILE_DIALOG1:
		{
							   int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, hamming12_8);
							   if (ret == -1)
							   {
								   MessageBox(hwnd, "Dialog failed!", "Error", MB_OK | MB_ICONINFORMATION);
							   }
		}
		break;
		}
	break;
	case WM_CREATE:
	{
		HMENU hMenu, hSubMenu;
		HICON hIcon, hIconSm;
		hMenu = CreateMenu();
		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_DIALOG, "&Erori singulare");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_DIALOG1, "&Hamming (12,8)");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Codari");
		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");
		SetMenu(hwnd, hMenu);
		hIcon = (HICON)LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		if (hIcon) SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		else MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);
		hIconSm = (HICON)LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		if (hIconSm) SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
		else MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(NULL));
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, g_szClassName, "Fereastra", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 250, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}