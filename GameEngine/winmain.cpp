#define WIN32_LEAN_AND_MEAN

#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// ��������
HINSTANCE hinst;	
HDC hdc;			//	����̽� ���ؽ�Ʈ�� �ڵ�
TCHAR ch = ' ';		// �Է��� ����
RECT rect;			// �簢�� ����ü
PAINTSTRUCT ps;		// WM_PAINT���� �����
bool vkKeys[256];	// ���� Ű

// ���
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World ang gimochi";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

//==========================================
// ������ ���ø����̼��� ���� ��ġ - main
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// â�� �����.
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;
	
	// ����Ű �迭 �ʱ�ȭ (false��)
	for (int i = 0; i < 256; i++)  
		vkKeys[i] = false;

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
	}

}

//==========================================
// ������ �̺�Ʈ �ݹ��Լ�
//==========================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	short nVirtKey;                 // virtual-key code 
	const short SHIFTED = (short)0x8000;
	TEXTMETRIC tm;                  // structure for text metrics 
	DWORD chWidth = 20;             // width of characters
	DWORD chHeight = 20;            // height of characters
	
	
	switch (msg)
	{
		case WM_CREATE:
			// get the text metrics
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			ReleaseDC(hwnd, hdc);
			chWidth = tm.tmAveCharWidth;        // average character width
			chHeight = tm.tmHeight;             // character height
			return 0;
		
		// ���α׷� ���� ��û ��
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		// Ű �ٿ�
		case WM_KEYDOWN:                                // key down
			vkKeys[wParam] = true;
			switch (wParam)
			{
			case VK_SHIFT:                          // shift key
				nVirtKey = GetKeyState(VK_LSHIFT);  // get state of left shift
				if (nVirtKey & SHIFTED)             // if left shift
					vkKeys[VK_LSHIFT] = true;
				nVirtKey = GetKeyState(VK_RSHIFT);  // get state of right shift
				if (nVirtKey & SHIFTED)             // if right shift
					vkKeys[VK_RSHIFT] = true;
				break;
			case VK_CONTROL:                        // control key
				nVirtKey = GetKeyState(VK_LCONTROL);
				if (nVirtKey & SHIFTED)             // if left control
					vkKeys[VK_LCONTROL] = true;
				nVirtKey = GetKeyState(VK_RCONTROL);
				if (nVirtKey & SHIFTED)             // if right control
					vkKeys[VK_RCONTROL] = true;
				break;
			}
			InvalidateRect(hwnd, NULL, TRUE);           // force WM_PAINT
			return 0;
			break;

		// Ű ��
		case WM_KEYUP:                                  // key up
			vkKeys[wParam] = false;
			switch (wParam)
			{
			case VK_SHIFT:                          // shift key
				nVirtKey = GetKeyState(VK_LSHIFT);
				if ((nVirtKey & SHIFTED) == 0)      // if left shift
					vkKeys[VK_LSHIFT] = false;
				nVirtKey = GetKeyState(VK_RSHIFT);
				if ((nVirtKey & SHIFTED) == 0)      // if right shift
					vkKeys[VK_RSHIFT] = false;
				break;
			case VK_CONTROL:                        // control key
				nVirtKey = GetKeyState(VK_LCONTROL);
				if ((nVirtKey & SHIFTED) == 0)      // if left control
					vkKeys[VK_LCONTROL] = false;
				nVirtKey = GetKeyState(VK_RCONTROL);
				if ((nVirtKey & SHIFTED) == 0)      // if right control
					vkKeys[VK_RCONTROL] = false;
				break;
			}
			InvalidateRect(hwnd, NULL, TRUE);    // force WM_PAINT
			return 0;
			break;

		// Ű���忡�� ���� �Է½�
		case WM_CHAR:
			switch (wParam)	// ���ڴ� wParam�� ����
			{
				// ǥ�õ��� ���ϴ� ������ ���
				case 0x08:		// �齺���̽�
				case 0x09:		// ��
				case 0x0A:		// �����ǵ�
				case 0x0D:		// ĳ���� ����
				case 0x1B:		// �̽�������
					MessageBeep((UINT)-1);	// ������ �߻�
					return 0;
				// ǥ�� ������ ������ ��� 
				default:
					ch = (TCHAR)wParam; 
					InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT�� ����
					return 0;
			}
		// �����츦 �ٽ� �׷����ϴ� ��� 
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);	// ����̽� ���ؽ�Ʈ�� �ڵ��� �����´�. 

			TextOut(hdc, 0, 0, &ch, 1);     // display the character

			// Display the state of vkKeys array
			// Display 'T' if key is down and 'F' is key is up
			for (int r = 0; r < 16; r++)
			{
				for (int c = 0; c < 16; c++)
				{
					if (vkKeys[r * 16 + c])
					{
						SetBkMode(hdc, OPAQUE);         // opaque text background
						TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "T", 1);
					}
					else 
					{
						SetBkMode(hdc, TRANSPARENT);    // transparent text background
						TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "F", 1);
					}
				}
			}
			EndPaint(hwnd, &ps);
			return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//==========================================
// ������ ���� - ���� �߻� �� false ��ȯ
//==========================================
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
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;
	
	// Window Ŭ������ ����Ѵ�.
	// RegisterClassEx �Լ��� ������ �߻��� ��� 0�� ��ȯ
	if (RegisterClassEx(&wcx) == 0)
		return false;

	// ������ ����
	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
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