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

	// â�� �����.
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	// main message loop - ���������� �ݺ�
	int done = 0;
	while (!done)
	{
		// PeekMessage�� ������ �޼����� Ȯ���ϴ� ����ŷ �޼ҵ��̴�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ����޼��� ã��
			if (msg.message == WM_QUIT)
				done = 1;

			//�ؼ��� �� �޼����� WinProc�� ����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}


// ������ �̺�Ʈ �ݹ��Լ�
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

	// Window Ŭ���� ����ü�� ���� �����쿡 ���� �Ű������� ä���.
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;						// �޼��� ó�� �Լ��� ������
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = (LPCWSTR)CLASS_NAME;
	wcx.hIconSm = NULL;
	
	// Window Ŭ������ ����Ѵ�.
	// RegisterClassEx �Լ��� ������ �߻��� ��� 0�� ��ȯ
	if (RegisterClassEx(&wcx) == 0)
		return false;

	// ������ ����
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

	// �����츦 �����ϴ� ���� ������ �߻��Ѵٸ�
	if (!hwnd)
		return false;

	// �����츦 ǥ��
	ShowWindow(hwnd, nCmdShow);

	// ������ ���ν������� WM_PAINT �޼����� ������.
	UpdateWindow(hwnd);
	return true;
}