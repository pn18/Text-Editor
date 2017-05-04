#define WIN32_LEAN_AND_MEAN
#define STRICT

#include <Windows.h>
#include <iostream>
#include <fstream>

#define IDM_FILE		1000
#define IDM_FILE_NEW	1001
#define IDM_FILE_OPEN	1002
#define IDM_FILE_SAVE	1003
#define IDM_FILE_SAVEAS	1004
#define IDM_FILE_PRINT	1005
#define IDM_FILE_EXIT	1006
#define IDM_EDIT		1010
#define IDM_EDIT_COPY	1011
#define IDM_EDIT_PASTE	1012
#define IDM_EDIT_DELETE	1013

#define IDC_EDIT		2000

LRESULT _stdcall WndProc(HWND mainWindow, UINT message, WPARAM wParam, LPARAM lParam);

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	int retval = 0;

	//Defines Window Class
	WNDCLASSEX wincl;
	ZeroMemory(&wincl, sizeof(wincl));
	wincl.cbSize = sizeof(wincl);
	wincl.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wincl.hIcon = wincl.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wincl.hInstance = hInstance;
	wincl.lpszClassName = L"Text_Editor";
	wincl.style = CS_HREDRAW | CS_VREDRAW;
	wincl.lpfnWndProc = WndProc;

	if (RegisterClassEx(&wincl))
	{
		HMENU hMenu = CreateMenu();
		HMENU hFileMenu = CreateMenu();
		HMENU hEditMenu = CreateMenu();
		HMENU hFormatMenu = CreateMenu();

		//Appending the menu bar
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"&Edit");
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFormatMenu, L"&Format");

		//Appending the File Dropdown menu
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVEAS, L"Save &AS");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_PRINT, L"&Print");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, L"E&xit");

		//Appending the Edit Menu
		AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_COPY, L"&Copy");
		AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_PASTE, L"&Paste");
		AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_DELETE, L"&Delete");

		HWND mainWindow = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
									L"Text_Editor",
									L"Untitled",
									WS_OVERLAPPEDWINDOW | WS_VISIBLE,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									NULL,
									hMenu,
									hInstance,
									NULL);

		if (mainWindow)
		{
			MSG wMessage;
			while (GetMessage(&wMessage, nullptr, 0, 0) > 0)
			{
				TranslateMessage(&wMessage);
				DispatchMessage(&wMessage);
			}
		}
		UnregisterClass(L"Text_Editor", hInstance);
	}


	return retval;
}

LRESULT _stdcall WndProc(HWND mainWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT retval = S_OK;
	static HWND childWindow;
	switch (message)
	{
	case WM_CREATE:
		childWindow = CreateWindowEx(0,
									L"Edit",
									nullptr,
									WS_CHILD | WS_VISIBLE | ES_LEFT |ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | ES_AUTOHSCROLL,
									0,
									0,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									mainWindow,
									(HMENU)IDC_EDIT,
									(HINSTANCE)GetWindowLong(mainWindow, GWLP_HINSTANCE),
									nullptr
									);
		if(childWindow == nullptr)
			retval = E_FAIL;
		break;

	case WM_SETFOCUS:
		SetFocus(childWindow);
		break;

	case WM_SIZE:
		MoveWindow(childWindow, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;

	case WM_CLOSE:
		DestroyWindow(mainWindow);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
			EnableMenuItem(GetMenu(mainWindow), IDM_FILE_SAVEAS, MF_BYCOMMAND | MF_DISABLED);
			SendMessage(childWindow, WM_SETTEXT, 0, (LPARAM)L"");
			SendMessage(childWindow, EM_SETMODIFY, FALSE, 0);
			break;

		case IDM_FILE_OPEN:
			break;

		case IDM_FILE_SAVE:

			break;

		case IDM_FILE_EXIT:
			DestroyWindow(mainWindow);
			break;

		case IDC_EDIT:
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
				EnableMenuItem(GetMenu(mainWindow), IDM_FILE_SAVEAS, MF_BYCOMMAND | MF_ENABLED);
				break;
			default:
				retval = DefWindowProc(mainWindow, message, wParam, lParam);
				break;
			}

		default:
			retval = DefWindowProc(mainWindow, message, wParam, lParam);
			break;
		}
		break;

	default:
		retval = DefWindowProc(mainWindow, message, wParam, lParam);
		break;
	}
	return retval;
}


