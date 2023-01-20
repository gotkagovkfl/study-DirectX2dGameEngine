// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.cpp v1.1
// Last modified Mar-9-2015

#include "game.h"

// ================================================================================================
// 생성자
// ================================================================================================
Game::Game()
{
	input = new Input();

	// 추가 초기화 작업은 input->initialize() 후에 처리
	paused = false;
	graphics = NULL;
	initialized = false;
}

// ================================================================================================
// 소멸자
// ================================================================================================
Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

// ================================================================================================
// 윈도우 메세지 핸들러 - 기존 WinProc 함수에서 실행할 내용을 game 객체에서 실행 
// ================================================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 반드시 초기화가 된 상태에서 메세지를 받아야함
	if (initialized)
	{
		switch (msg)
		{
		case WM_DESTROY:				// 종료
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:				// 키를 누름
		case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return 0;

		case WM_KEYUP:					// 키를 뗌
		case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;

		case WM_CHAR:					// 문자 입력
			input->keyIn(wParam);
			return 0;

		case WM_MOUSEMOVE:				// 마우스를 움직임
			input->mouseIn(lParam);
			return 0;
		case WM_INPUT:					// 마우스 데이터가 들어옴
			input->mouseRawIn(lParam);
			return 0;

		case WM_LBUTTONDOWN:			// 마우스 왼쪽 버튼 누름
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:			// 마우스 중앙 버튼 누름
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:			// 마우스 오른쪽 버튼 누름
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_LBUTTONUP:				// 마우스 왼쪽 버튼 뗌
			input->setMouseLButton(false);
			input->mouseIn(lParam);
		case WM_MBUTTONUP:				// 마우스 중앙 버튼 뗌
			input->setMouseMButton(false);
			input->mouseIn(lParam);
		case WM_RBUTTONUP:				// 마우스 오른쪽 버튼 뗌
			input->setMouseRButton(false);
			input->mouseIn(lParam);


		// 컨트롤러 관련 함수는 제외했음
		}
		return DefWindowProc(hwnd, msg, wParam, lParam); // 윈도우 한테 처리 명령
	}
}

// ================================================================================================
// 게임 초기화
// ================================================================================================
void Game::initialize(HWND hw)
{
	hwnd = hw;

	// graphics 객체 초기화
	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// Input 객체 초기화
	input->initialize(hwnd, false);

	// 고성능 타이머 사용 시도 - 진동수 측정 
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	
	// 시작 시간 가져오기 - 후에 fps 결정하기 위함 . - window xp 이상 버전에서는 실패할 일이 없다.
	QueryPerformanceCounter(&timeStart);

	initialized = true;
}

// ================================================================================================
// 로스트 상태가 된 그래픽 디바이스를 처리
// ================================================================================================
void Game::handleLostGraphicsDevice()
{
	hr = graphics->getDeviceState();
	// 우선 디바이스가 유효한 상태가 아니라면,
	if (FAILED(hr))
	{
		// 디바이스가 로스트 상태이며 리셋이 불가능하다면
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);			// CPU에게 시간을 양보 - 복구할 수 있을 때까지 기다리기
			return;
		}
		// 디바이스가 로스트 상태지만 리셋이 가능하다면
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();			// 리셋시도
			if (FAILED(hr))					// 리셋이 실패하면
				return;
			releaseAll();
		}
		// 다른 디바이스 에러
		else
			return;				
	}
}


// ================================================================================================
// 게임 아이템을 렌더링
// ================================================================================================
void Game::renderGame()
{
	// 씬을 열고 렌더한 후 씬 종료 
	if (SUCCEEDED(graphics->beginScene()))
	{
		render();

		graphics->endScene();
	}
	handleLostGraphicsDevice();

	// 백 버퍼를 표시 
	graphics->showBackbuffer();
}

// ================================================================================================
// WinMain의 메인 메시지 루프에서 반복적으로 호출
// ================================================================================================
void Game::run(HWND hwnd)
{
	// 그래픽이 초기화되지 않았다면 반환
	if (graphics == NULL)
		return;
	//=================================== fps 관련 =======================================================
	// 마지막 프레임으로부터 경과된 시간을 계산해 frameTime에 저장
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	
	// 절전을 위한 코드. winmm.lib 필요
	// 경과된 시간이 원하는 프레임 속도에 충족하지 않는다면,
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		// 원래 타이머는 최소 15.625ms를 측정할 수 있지만,  timeBeginPeriod를 사용하면 1ms로 측정 가능하다. 
		timeBeginPeriod(1);			
		Sleep(sleepTime);	//sleepTime만큼  CPU를 쉬게함.
		timeEndPeriod(1);
		return;
	}

	// fps 계산
	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime); 

	// fps 상한 
	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;
	
	// 다음 fps계산을 위해 timeStart 값 갱신
	timeStart = timeEnd;

	//====================================================================================================

	// 일시 정지 상태가 아닌 경우,
	if (!paused)
	{
		update();
		ai();
		collision();
	}

	// 모든 게임 아이템 그리기(렌더)
	renderGame();

	// 입력을 지움.
	input->clear(inputNS::KEYS_PRESSED);
}

// ================================================================================================
// 예약된 모든 비디오 메모리를 해제하고 그래픽 디바이스를 리셋
// ================================================================================================
void Game::releaseAll()
{

}

// ================================================================================================
// 모든 표면을 재생성하고 모든 개체를 리셋
// ================================================================================================
void Game::resetAll()
{

}

// ================================================================================================
// 예약된 모든 메모리 삭제
// ================================================================================================
void Game::deleteAll()
{
	releaseAll();
	safeDelete(graphics);
	safeDelete(input);
	initialized = false;
}





