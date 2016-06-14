#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "resource.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ID_FILE_EXIT 9001
#define ID_FILE_DIALOG 9003
#define ID_FILE_DIALOG1 9004

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
	for (int j = 1; j <= 8; j++)
	{
		v[j] = 0;
	}
	while (val)
	{
		v[i] = val % 2;
		val = val / 2;
		i++;
	}
	v[i] = val;
}

void vectbin_to_stringbin(caracter *v, int poz,char cuv[])
{
	int i;
	for (i = 1; i <= 8; i++)
	{
		if (v[poz].vectorbinar[i] == 0) cuv[i-1] = '0';
		else cuv[i-1] = '1';
	}
	cuv[8] = 0;
}

void vecthamm_to_stringbin(caracter *v, int poz, char cuv[])
{
	int i;
	for (i = 1; i <= 12; i++)
	{
		if (v[poz].vectorhamming[i] == 0) cuv[i - 1] = '0';
		else cuv[i - 1] = '1';
	}
	cuv[12] = 0;
}

void H12_8(caracter *v)
{
	int P1, P2, P3, P4;
	for (int i = 1; i <= 12; i++)
	{
		(*v).vectorhamming[i] = 0;
	}
	//setare biti din vectorul hamming cu echivalentul lor din vectorul binar;
	(*v).vectorhamming[3] = (*v).vectorbinar[1];
	(*v).vectorhamming[5] = (*v).vectorbinar[2];
	(*v).vectorhamming[6] = (*v).vectorbinar[3];
	(*v).vectorhamming[7] = (*v).vectorbinar[4];
	(*v).vectorhamming[9] = (*v).vectorbinar[5];
	(*v).vectorhamming[10] = (*v).vectorbinar[6];
	(*v).vectorhamming[11] = (*v).vectorbinar[7];
	(*v).vectorhamming[12] = (*v).vectorbinar[8];
	///calcularea bitilor de paritate
	//1, 3, 5, 7, 9, 11
	P1 = (*v).vectorhamming[3] + (*v).vectorhamming[5] + (*v).vectorhamming[7] + (*v).vectorhamming[9] + (*v).vectorhamming[11];
	(*v).vectorhamming[1] = P1%2;
	//2,3,6,7,10,11
	P2 = (*v).vectorhamming[3] + (*v).vectorhamming[6] + (*v).vectorhamming[7] + (*v).vectorhamming[10] + (*v).vectorhamming[11];
	(*v).vectorhamming[2] = P2%2;
	//4, 5, 6, 7, 12
	P3 = (*v).vectorhamming[5] + (*v).vectorhamming[6] + (*v).vectorhamming[7] + (*v).vectorhamming[12];
	(*v).vectorhamming[4] = P3%2;
	//8,9,10,11,12
	P4 = (*v).vectorhamming[9] + (*v).vectorhamming[10] + (*v).vectorhamming[11] + (*v).vectorhamming[12];
	(*v).vectorhamming[8] = P4%2;
}

void hamm_gen_err(caracter *v)
{
	int generr, pozerr;
	generr = rand() % 2;
	if (generr == 1)
	{
		pozerr = rand() % 12 + 1;
		if ((*v).vectorhamming[pozerr] == 0) (*v).vectorhamming[pozerr] = 1;
		else (*v).vectorhamming[pozerr] = 0;
	}
}

int pozerr(caracter *v)
{
	int pozeronata = 0,P1,P2,P3,P4;
	P1 = (*v).vectorhamming[1] + (*v).vectorhamming[3] + (*v).vectorhamming[5] + (*v).vectorhamming[7] + (*v).vectorhamming[9] + (*v).vectorhamming[11];
	P2 = (*v).vectorhamming[2] + (*v).vectorhamming[3] + (*v).vectorhamming[6] + (*v).vectorhamming[7] + (*v).vectorhamming[10] + (*v).vectorhamming[11];
	P3 = (*v).vectorhamming[4] + (*v).vectorhamming[5] + (*v).vectorhamming[6] + (*v).vectorhamming[7] + (*v).vectorhamming[12];
	P4 = (*v).vectorhamming[8] + (*v).vectorhamming[9] + (*v).vectorhamming[10] + (*v).vectorhamming[11] + (*v).vectorhamming[12];
	pozeronata = (P1 % 2) + (P2 % 2) * 2 + (P3 % 2) * 4 + (P4 % 2) * 8;
	return pozeronata;
}

char calcchar(caracter v)
{
	int ASC;
	ASC =(v.vectorhamming[3]*1) + (v.vectorhamming[5] * 2 )+ (v.vectorhamming[6] * 4 )+ (v.vectorhamming[7] * 8) + (v.vectorhamming[9] * 16) + (v.vectorhamming[10] * 32) + (v.vectorhamming[11] * 64) + (v.vectorhamming[12]*128);
	return (char)ASC;
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
			///mesajul initial este salvat intr-un buffer pentru prelucrarile viitoare
			caracter *vect;
			char *sir;
			char *sirbinar;
			sir = (char*)malloc(len + 1);
			GetDlgItemText(hwnd, IDC_EDIT1, sir, len + 1);
			sirbinar = (char*)malloc(9 * len + 1);
			vect = (caracter*)malloc(len*sizeof(caracter));
			for (int i = 0; i < len; i++) transfbin((int)sir[i], vect[i].vectorbinar);
		
			/////este afisat mesajul initial in format binar(fara erori generate)
			char *sirbin,cc[9];
			sirbin = (char*)malloc(9 * len);
			vectbin_to_stringbin(vect, 0, cc);
			strcpy(sirbin, cc);
			for (int i = 1; i < len; i++)
			{
				vectbin_to_stringbin(vect, i, cc);
				strcat(sirbin, "_");
				strcat(sirbin, cc);
			}
			sirbin[9 * len] = 0;
			SetDlgItemText(hwnd, IDC_EDIT2, sirbin);

			/////Codarea hamming 12_8
			for (int i = 0; i < len; i++) H12_8(&vect[i]);

			///afisarea mesajului binar dupa codare
			char *sirbinhamm, ch[13];
			sirbinhamm = (char*)malloc(13 * len);
			vecthamm_to_stringbin(vect, 0, ch);
			strcpy(sirbinhamm, ch);
			for (int i = 1; i < len; i++)
			{
				vecthamm_to_stringbin(vect, i, ch);
				strcat(sirbinhamm, "_");
				strcat(sirbinhamm, ch);
			}
			sirbinhamm[13 * len] = 0;
			SetDlgItemText(hwnd, IDC_EDIT3, sirbinhamm);

			///Generare erori pe mesajul binar codificat
			for (int i = 0; i < len; i++) hamm_gen_err(&vect[i]);

			//Afisare mesaj text cu erori
			char *mesajtexteronat;
			mesajtexteronat = (char*)malloc(len + 1);
			for (int i = 0; i < len; i++)
			{
				if (pozerr(&vect[i]) != 0) mesajtexteronat[i] = '#';
				else mesajtexteronat[i] = calcchar(vect[i]);
			}
			mesajtexteronat[len] = 0;
			SetDlgItemText(hwnd, IDC_EDIT5, mesajtexteronat);

			//Afisare mesaj binar cu erori
			char *mesajbherr, cbh[13];
			mesajbherr = (char*)malloc(13 * len);
			vecthamm_to_stringbin(vect, 0, cbh);
			strcpy(mesajbherr, cbh);
			for (int i = 1; i < len; i++)
			{
				vecthamm_to_stringbin(vect, i, cbh);
				strcat(mesajbherr, "_");
				strcat(mesajbherr, cbh);
			}
			mesajbherr[13 * len] = 0;
			SetDlgItemText(hwnd, IDC_EDIT6, mesajbherr);

			//Corectie erori
			for (int i = 0; i < len; i++)
			{
				if (pozerr(&vect[i]) != 0)
				{
					if (vect[i].vectorhamming[pozerr(&vect[i])] == 0) vect[i].vectorhamming[pozerr(&vect[i])] = 1;
					else vect[i].vectorhamming[pozerr(&vect[i])] = 0;
				}
			}

			//Afisare mesaj text corectat
			char *mesajtextcorectat;
			mesajtextcorectat = (char*)malloc(len + 1);
			for (int i = 0; i < len; i++)
			{
				mesajtextcorectat[i] = calcchar(vect[i]);
			}
			mesajtextcorectat[len] = 0;
			SetDlgItemText(hwnd, IDC_EDIT7, mesajtextcorectat);

			//Afisare mesaj binar corectat
			char *mesajbhcor, cbc[13];
			mesajbhcor = (char*)malloc(13 * len);
			vecthamm_to_stringbin(vect, 0, cbc);
			strcpy(mesajbhcor, cbc);
			for (int i = 1; i < len; i++)
			{
				vecthamm_to_stringbin(vect, i, cbc);
				strcat(mesajbhcor, "_");
				strcat(mesajbhcor, cbc);
			}
			mesajbhcor[13 * len] = 0;
			SetDlgItemText(hwnd, IDC_EDIT8, mesajbhcor);
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
	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, g_szClassName, "Hamming", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 200, 150, NULL, NULL, hInstance, NULL);
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