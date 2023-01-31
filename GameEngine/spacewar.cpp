// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine Part 1
// Chapter 4 spacewar.cpp v1.0
// Spacewar is the class we create.

#include "spaceWar.h"


//===================================================================================
// ������
//===================================================================================
Spacewar::Spacewar()
{

}

//===================================================================================
// �Ҹ���
//===================================================================================
Spacewar::~Spacewar()
{
	releaseAll();
}

//===================================================================================
// ���� �ʱ�ȭ
//===================================================================================
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	// �θ�Ŭ���� �ʱ�ȭ �Լ� ȣ��

	// �ؽ�ó �� �̹��� �ʱ�ȭ
	// ���� �ؽ�ó 
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// �༺ �ؽ�ó 
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));

	// ���ּ� �ؽ�ó
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));
	
	// ���� �̹���
	if (!nebula.initialize(graphics, 0,0,0,&nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// �༺ �̹��� 
	if (!planet.initialize(graphics,0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// ���ּ� �̹���
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));

	// �༺ �ʱ� ���� ( ������ 0,0�� ������)
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	// ���ּ� �ʱ� ����
	ship.setX(GAME_WIDTH / 4);
	ship.setY(GAME_HEIGHT / 4);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);
	ship.setCurrentFrame(SHIP_START_FRAME);
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	//ship.setDegrees(45.0f);


	return;
}

// ================================== ���� �����Լ� �������̵� ============================================

//===================================================================================
// ��� ���� ������ ����
//===================================================================================
void Spacewar::update()
{
	if (input->isKeyDown(VK_RIGHT))
	{
		ship.setX(ship.getX() + frameTime * SHIP_SPEED);
		if (ship.getX() > GAME_WIDTH)
			ship.setX((float)-ship.getWidth());
	}
	
	
	ship.update(frameTime);
}

//===================================================================================
// �ΰ�����
//===================================================================================
void Spacewar::ai()
{

}

//===================================================================================
// �浹ó��
//===================================================================================
void Spacewar::collision()
{

}

//===================================================================================
// ���� ������ ������
//===================================================================================
void Spacewar::render()
{
	graphics->spriteBegin();

	nebula.draw();
	planet.draw();
	ship.draw();

	graphics->spriteEnd();
}

//==================================================================================================

//===================================================================================
// �׷��� ����̽��� �ν�Ʈ ���°� ���� ��, ����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� ������ �� �ְ� �Ѵ�.(ȣ�� ������ �������� )
//===================================================================================
void Spacewar::releaseAll()
{
	shipTexture.onLostDevice();
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	
	Game::releaseAll();
	return;
}

//===================================================================================
// �׷��� ����̽��� ����, ��� ǥ�� �����
//===================================================================================
void Spacewar::resetAll()
{
	planetTexture.onResetDevice();
	nebulaTexture.onResetDevice();
	shipTexture.onResetDevice();
	
	Game::resetAll();
	return;
}





