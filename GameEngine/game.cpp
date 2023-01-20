// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.cpp v1.1
// Last modified Mar-9-2015

#include "game.h"

// ================================================================================================
// ������
// ================================================================================================
Game::Game()
{
	input = new Input();

	// �߰� �ʱ�ȭ �۾��� input->initialize() �Ŀ� ó��
	paused = false;
	graphics = NULL;
	initialized = false;
}

// ================================================================================================
// �Ҹ���
// ================================================================================================
Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

// ================================================================================================
// ������ �޼��� �ڵ鷯 - ���� WinProc �Լ����� ������ ������ game ��ü���� ���� 
// ================================================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �ݵ�� �ʱ�ȭ�� �� ���¿��� �޼����� �޾ƾ���
	if (initialized)
	{
		switch (msg)
		{
		case WM_DESTROY:				// ����
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:				// Ű�� ����
		case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return 0;

		case WM_KEYUP:					// Ű�� ��
		case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;

		case WM_CHAR:					// ���� �Է�
			input->keyIn(wParam);
			return 0;

		case WM_MOUSEMOVE:				// ���콺�� ������
			input->mouseIn(lParam);
			return 0;
		case WM_INPUT:					// ���콺 �����Ͱ� ����
			input->mouseRawIn(lParam);
			return 0;

		case WM_LBUTTONDOWN:			// ���콺 ���� ��ư ����
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:			// ���콺 �߾� ��ư ����
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:			// ���콺 ������ ��ư ����
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_LBUTTONUP:				// ���콺 ���� ��ư ��
			input->setMouseLButton(false);
			input->mouseIn(lParam);
		case WM_MBUTTONUP:				// ���콺 �߾� ��ư ��
			input->setMouseMButton(false);
			input->mouseIn(lParam);
		case WM_RBUTTONUP:				// ���콺 ������ ��ư ��
			input->setMouseRButton(false);
			input->mouseIn(lParam);


		// ��Ʈ�ѷ� ���� �Լ��� ��������
		}
		return DefWindowProc(hwnd, msg, wParam, lParam); // ������ ���� ó�� ���
	}
}

// ================================================================================================
// ���� �ʱ�ȭ
// ================================================================================================
void Game::initialize(HWND hw)
{
	hwnd = hw;

	// graphics ��ü �ʱ�ȭ
	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// Input ��ü �ʱ�ȭ
	input->initialize(hwnd, false);

	// ���� Ÿ�̸� ��� �õ� - ������ ���� 
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	
	// ���� �ð� �������� - �Ŀ� fps �����ϱ� ���� . - window xp �̻� ���������� ������ ���� ����.
	QueryPerformanceCounter(&timeStart);

	initialized = true;
}

// ================================================================================================
// �ν�Ʈ ���°� �� �׷��� ����̽��� ó��
// ================================================================================================
void Game::handleLostGraphicsDevice()
{
	hr = graphics->getDeviceState();
	// �켱 ����̽��� ��ȿ�� ���°� �ƴ϶��,
	if (FAILED(hr))
	{
		// ����̽��� �ν�Ʈ �����̸� ������ �Ұ����ϴٸ�
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);			// CPU���� �ð��� �纸 - ������ �� ���� ������ ��ٸ���
			return;
		}
		// ����̽��� �ν�Ʈ �������� ������ �����ϴٸ�
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();			// ���½õ�
			if (FAILED(hr))					// ������ �����ϸ�
				return;
			releaseAll();
		}
		// �ٸ� ����̽� ����
		else
			return;				
	}
}


// ================================================================================================
// ���� �������� ������
// ================================================================================================
void Game::renderGame()
{
	// ���� ���� ������ �� �� ���� 
	if (SUCCEEDED(graphics->beginScene()))
	{
		render();

		graphics->endScene();
	}
	handleLostGraphicsDevice();

	// �� ���۸� ǥ�� 
	graphics->showBackbuffer();
}

// ================================================================================================
// WinMain�� ���� �޽��� �������� �ݺ������� ȣ��
// ================================================================================================
void Game::run(HWND hwnd)
{
	// �׷����� �ʱ�ȭ���� �ʾҴٸ� ��ȯ
	if (graphics == NULL)
		return;
	//=================================== fps ���� =======================================================
	// ������ ���������κ��� ����� �ð��� ����� frameTime�� ����
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	
	// ������ ���� �ڵ�. winmm.lib �ʿ�
	// ����� �ð��� ���ϴ� ������ �ӵ��� �������� �ʴ´ٸ�,
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		// ���� Ÿ�̸Ӵ� �ּ� 15.625ms�� ������ �� ������,  timeBeginPeriod�� ����ϸ� 1ms�� ���� �����ϴ�. 
		timeBeginPeriod(1);			
		Sleep(sleepTime);	//sleepTime��ŭ  CPU�� ������.
		timeEndPeriod(1);
		return;
	}

	// fps ���
	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime); 

	// fps ���� 
	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;
	
	// ���� fps����� ���� timeStart �� ����
	timeStart = timeEnd;

	//====================================================================================================

	// �Ͻ� ���� ���°� �ƴ� ���,
	if (!paused)
	{
		update();
		ai();
		collision();
	}

	// ��� ���� ������ �׸���(����)
	renderGame();

	// �Է��� ����.
	input->clear(inputNS::KEYS_PRESSED);
}

// ================================================================================================
// ����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� ����
// ================================================================================================
void Game::releaseAll()
{

}

// ================================================================================================
// ��� ǥ���� ������ϰ� ��� ��ü�� ����
// ================================================================================================
void Game::resetAll()
{

}

// ================================================================================================
// ����� ��� �޸� ����
// ================================================================================================
void Game::deleteAll()
{
	releaseAll();
	safeDelete(graphics);
	safeDelete(input);
	initialized = false;
}





