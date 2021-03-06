#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
using namespace std;

DWORD pid;
HWND hwnd;
POINT mouse;
INPUT ip;
std::string disc;
bool finished = false;

void maximizeWindow() {
	//Activate window
	ShowWindow(hwnd, SW_MAXIMIZE);
	ShowWindow(hwnd, SW_HIDE);
	SetActiveWindow(hwnd);
}

void LeftClick(int x, int y) {
	mouse.x = x;
	mouse.y = y;
	SendMessage(hwnd, WM_LBUTTONDOWN, 1, MAKELPARAM(mouse.x, mouse.y));
	Sleep(100);
	SendMessage(hwnd, WM_LBUTTONUP, 1, MAKELPARAM(mouse.x, mouse.y));
	Sleep(50);
	SendMessage(hwnd, WM_CAPTURECHANGED, 00000000, 0);
	Sleep(50);
}

void RightClick(int x, int y) {
	mouse.x = x;
	mouse.y = y;
	SendMessage(hwnd, WM_PARENTNOTIFY, WM_RBUTTONDOWN, MAKELPARAM(mouse.x, mouse.y));
	Sleep(400);
	SendMessage(hwnd, WM_RBUTTONUP, 1, MAKELPARAM(mouse.x, mouse.y));
	Sleep(400);
	SendMessage(hwnd, WM_CAPTURECHANGED, 00000000, 0);
	Sleep(50);
}

void TypeRagnar() {
	SendMessage(hwnd, WM_CHAR, 0x52, 0);
	SendMessage(hwnd, WM_CHAR, 0x41, 0);
	SendMessage(hwnd, WM_CHAR, 0x47, 0);
	SendMessage(hwnd, WM_CHAR, 0x4E, 0);
	SendMessage(hwnd, WM_CHAR, 0x41, 0);
	SendMessage(hwnd, WM_CHAR, 0x52, 0);
}

static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	int length = GetWindowTextLength(hWnd);
	char* buffer = new char[length + 1];
	GetWindowTextA(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);
	std::string discord = "Discord";

	if (windowTitle.find(discord) != std::string::npos) {
		std::cout << "\"" << windowTitle << "\" <<<< MATCH FOUND" << '\n';
		disc = windowTitle;
		return FALSE; //exit loop
	}

	// Ignore windows if invisible or missing a title
	if (IsWindowVisible(hWnd) && length != 0) {
		std::cout << hWnd << ":  " << windowTitle << std::endl;
	}
	return TRUE;
}


void window() {
	// Locale Swedish
	SetConsoleOutputCP(1252);
	std::cout << "Just testing Swedish letters.." << endl;
	std::cout << "."; Sleep(500); std::cout << "."; Sleep(500); std::cout << "."; Sleep(500);
	std::cout << "ÅÄÖ" << endl << endl;
	Sleep(3000);
	std::cout << "Mouse position: " << endl;
	while (!finished) {
		GetCursorPos(&mouse);
		std::cout << "x: " << mouse.x << " y: " << mouse.y << " \r";
		Sleep(50);
	}
}

int main()
{
	std::cout << "Enumerating windows.." << std::endl;
	EnumWindows(enumWindowCallback, NULL);

	wstring wstr(disc.begin(), disc.end()); // String -> wide string conversion

	hwnd = FindWindowW(NULL, wstr.c_str()); // hook window handle by string match

	if (hwnd) {
		std::cout << "Window handle hooked" << endl << endl;
	}
	else std::cout << "NOT FOUND" << endl;

	// separation of endless while loop
	std::thread mouse_tracker(window);


	maximizeWindow();
	Sleep(1000);
	LeftClick(159, 40); // Server
	Sleep(1000);
	LeftClick(161, 135); // Settings
	Sleep(800);
	LeftClick(510, 434); // Members
	Sleep(1000);
	LeftClick(1228, 131); // Text field
	Sleep(1000);

	TypeRagnar();

	for (int i = 0; i < 2; i++) {
		Sleep(500);
		RightClick(1357, 199); //Player setting
		Sleep(500);
		LeftClick(1425, 337); // Transfer ownership
		Sleep(500);
		LeftClick(1135, 564); //U sure?
		Sleep(500);
		LeftClick(1083, 652); //Transfer
	}

	finished = true;

	ShowWindow(hwnd, SW_SHOW);
	
	std::cin.get();
}