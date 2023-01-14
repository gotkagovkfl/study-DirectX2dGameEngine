#define WIN32_LEAN_AND_MEAN

#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hinst;

const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World ang gimochi";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// 창을 만든다.
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	// main message loop - 끝날때까지 반복
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
	}

}


// 윈도우 이벤트 콜백함수
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// Window 클래스 구조체를 메인 윈도우에 대한 매개변수로 채운다.
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;						// 메세지 처리 함수의 포인터
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = (LPCWSTR)CLASS_NAME;
	wcx.hIconSm = NULL;
	
	// Window 클래스를 등록한다.
	// RegisterClassEx 함수는 에러가 발생할 경우 0을 반환
	if (RegisterClassEx(&wcx) == 0)
		return false;

	// 윈도우 생성
	hwnd = CreateWindow(
		(LPCTSTR) CLASS_NAME,
		(LPCTSTR) APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	// 윈도우를 생성하는 동안 에러가 발생한다면
	if (!hwnd)
		return false;

	// 윈도우를 표시
	ShowWindow(hwnd, nCmdShow);

	// 윈도우 프로시저에게 WM_PAINT 메세지를 보낸다.
	UpdateWindow(hwnd);
	return true;
}