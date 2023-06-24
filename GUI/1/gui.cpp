#include <windows.h>
#include <iostream>

//HWND hwnd - Window header unique key given to each window
//uMsg - Detect Key presses and mouse clicks
//WPARAM wParam - Word Parameter represent additional information about the message
//LPARAM lParam - Long Parameter represemns additional information about data of the message

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_DESTROY: //If window is closed
			PostQuitMessage(0); //display close message
			return 0;
		
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

int main(){
	//window class name
	const char* CLASS_NAME = "MyWindowClass";

	//get a unique identifier representing the instance running
	HINSTANCE hInstance = GetModuleHandle(NULL); 

	//register the window class
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc; //window actions
	wc.hInstance = hInstance; //assign window instance
	wc.lpszClassName = CLASS_NAME; //window Class name
	RegisterClass(&wc); //register the window

	//Create the window 
	//CreateWindowExA is the narrow character version of CreateWindowEx
	HWND hwnd = CreateWindowExA(
		0, //optional window styles
		CLASS_NAME, //window class
		"My Window", //window title
		WS_OVERLAPPEDWINDOW, //Window style (window has a title and a border)
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, //size and position

		NULL,  //parent window
 		NULL, //menu
		hInstance, //Instance handle
		NULL //additional application data
	);

	if(hwnd == NULL){
		std::cerr << "Could not load window";
		return 1;
	}

	//Show window and upadate the window
	ShowWindow(hwnd, SW_SHOWDEFAULT); //window, settings
	UpdateWindow(hwnd);

	//message loop
	MSG msg = {};

	//GetMessage(retreive message, handle receicing the message, minimum value, maximum value)
	//GetMessage receives a message and stores it in the msg variable
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); //virtual key messages -> character messages
		DispatchMessage(&msg); //forward message for processing
	}

	return 0;

}