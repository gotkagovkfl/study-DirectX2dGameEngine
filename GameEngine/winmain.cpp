#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "spaceWar.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// ��������
HINSTANCE hinst;	

// Graphics ������
Graphics* graphics;

// ���� ������
Spacewar* game = NULL;
HWND hwnd = NULL;

//==========================================
// ������ ���ø����̼��� ���� ��ġ - main
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ����� ���� ���� �� �޸� ������ Ȯ�� 
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	
	MSG msg;	// ���� �� �߻��ϴ� �̺�Ʈ �޼���

	// ���� ����
	game = new Spacewar;

	// â�� �����.
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try
	{
		// ���� �ʱ�ȭ 
		game->initialize(hwnd);

		// main message loop 
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
			else // �޼��� ������ ȭ�� ��ü
				game->run(hwnd);
		}
		SAFE_DELETE(game);
		return msg.wParam;

	}
	// ������ ���ӿ��� �߻��� �޸� ����
	catch (const GameError& err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	// �������� �ʴ� ���ӿ��� �߻���
	catch (...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}
	SAFE_DELETE(game); // ��ġ�鼭 ���ҽ� ����
	return 0;
}

//==========================================
// ������ �̺�Ʈ �ݹ��Լ�
//==========================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hwnd, msg, wParam, lParam);
}


//==========================================
// ������ ���� - ���� �߻� �� false ��ȯ
//==========================================
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// Window Ŭ���� ����ü�� ���� �����쿡 ���� �Ű������� ä���.
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;						// �޼��� ó�� �Լ��� ������
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;
	
	// Window Ŭ������ ����Ѵ�.
	// RegisterClassEx �Լ��� ������ �߻��� ��� 0�� ��ȯ
	if (RegisterClassEx(&wcx) == 0)
		return false;

	// ��üȭ�� �Ǵ� â ȭ���� ����
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	// ������ ����
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

	// �����츦 �����ϴ� ���� ������ �߻��Ѵٸ�
	if (!hwnd)
		return false;
	
	// ������ Ŭ���̾�Ʈ ������ ���ϴ� ������ �缳��
	if (!FULLSCREEN)
	{
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		MoveWindow(hwnd, 0, 0, GAME_WIDTH + (GAME_WIDTH - clientRect.right), GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), TRUE);
	}

	// �����츦 ǥ��
	ShowWindow(hwnd, nCmdShow);

	return true;
}