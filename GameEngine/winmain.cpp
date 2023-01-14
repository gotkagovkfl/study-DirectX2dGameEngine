#define WIN32_LEAN_AND_MEAN

#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역변수
HINSTANCE hinst;	
HDC hdc;			//	디바이스 컨텍스트의 핸들
TCHAR ch = ' ';		// 입력한 문자
RECT rect;			// 사각형 구조체
PAINTSTRUCT ps;		// WM_PAINT에서 사용함
bool vkKeys[256];	// 가상 키

// 상수
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World ang gimochi";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

//==========================================
// 윈도우 애플리케이션의 시작 위치 - main
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// 창을 만든다.
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;
	
	// 가상키 배열 초기화 (false로)
	for (int i = 0; i < 256; i++)  
		vkKeys[i] = false;

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
	}

}

//==========================================
// 윈도우 이벤트 콜백함수
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
		
		// 프로그램 종료 요청 시
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		// 키 다운
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

		// 키 업
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

		// 키보드에서 문자 입력시
		case WM_CHAR:
			switch (wParam)	// 문자는 wParam에 있음
			{
				// 표시되지 못하는 문자의 경우
				case 0x08:		// 백스페이스
				case 0x09:		// 탭
				case 0x0A:		// 라인피드
				case 0x0D:		// 캐리지 리턴
				case 0x1B:		// 이스케이프
					MessageBeep((UINT)-1);	// 비프음 발생
					return 0;
				// 표시 가능한 문자의 경우 
				default:
					ch = (TCHAR)wParam; 
					InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT를 강제
					return 0;
			}
		// 윈도우를 다시 그려야하는 경우 
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);	// 디바이스 컨텍스트의 핸들을 가져온다. 

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
// 윈도우 생성 - 에러 발생 시 false 반환
//==========================================
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
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;
	
	// Window 클래스를 등록한다.
	// RegisterClassEx 함수는 에러가 발생할 경우 0을 반환
	if (RegisterClassEx(&wcx) == 0)
		return false;

	// 윈도우 생성
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

	// 윈도우를 생성하는 동안 에러가 발생한다면
	if (!hwnd)
		return false;

	// 윈도우를 표시
	ShowWindow(hwnd, nCmdShow);

	// 윈도우 프로시저에게 WM_PAINT 메세지를 보낸다.
	UpdateWindow(hwnd);
	return true;
}