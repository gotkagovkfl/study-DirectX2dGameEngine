#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "spaceWar.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역변수
HINSTANCE hinst;	

// Graphics 포인터
Graphics* graphics;

// 게임 포인터
Spacewar* game = NULL;
HWND hwnd = NULL;

//==========================================
// 윈도우 애플리케이션의 시작 위치 - main
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 디버그 모드로 빌드 시 메모리 누수를 확인 
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	
	MSG msg;	// 게임 중 발생하는 이벤트 메세지

	// 게임 생성
	game = new Spacewar;

	// 창을 만든다.
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try
	{
		// 게임 초기화 
		game->initialize(hwnd);

		// main message loop 
		int done = 0;
		while (!done)
		{
			// PeekMessage는 윈도우 메세지를 확인하는 논블로킹 메소드이다.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// 종료메세지 찾기
				if (msg.message == WM_QUIT)
					done = 1;

				//해석한 뒤 메세지를 WinProc에 전달
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else // 메세지 없으면 화면 교체
				game->run(hwnd);
		}
		SAFE_DELETE(game);
		return msg.wParam;

	}
	// 정의한 게임에러 발생시 메모리 해제
	catch (const GameError& err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	// 정의하지 않는 게임에러 발생시
	catch (...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}
	SAFE_DELETE(game); // 마치면서 리소스 해제
	return 0;
}

//==========================================
// 윈도우 이벤트 콜백함수
//==========================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hwnd, msg, wParam, lParam);
}


//==========================================
// 윈도우 생성 - 에러 발생 시 false 반환
//==========================================
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// Window 클래스 구조체를 메인 윈도우에 대한 매개변수로 채운다.
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;						// 메세지 처리 함수의 포인터
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;
	
	// Window 클래스를 등록한다.
	// RegisterClassEx 함수는 에러가 발생할 경우 0을 반환
	if (RegisterClassEx(&wcx) == 0)
		return false;

	// 전체화면 또는 창 화면을 설정
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	// 윈도우 생성
	hwnd = CreateWindow(
		CLASS_NAME,
		GAME_TITLE,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GAME_WIDTH,
		GAME_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	// 윈도우를 생성하는 동안 에러가 발생한다면
	if (!hwnd)
		return false;
	
	// 윈도우 클라이언트 영역을 원하는 값으로 재설정
	if (!FULLSCREEN)
	{
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		MoveWindow(hwnd, 0, 0, GAME_WIDTH + (GAME_WIDTH - clientRect.right), GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), TRUE);
	}

	// 윈도우를 표시
	ShowWindow(hwnd, nCmdShow);

	return true;
}