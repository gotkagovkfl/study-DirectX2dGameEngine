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
	
	// ���� �̹���
	if (!nebula.initialize(graphics, 0,0,0,&nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// �༺ �̹��� 
	if (!planet.initialize(graphics,0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// �༺ ��ġ ���� ( ������ 0,0�� ������)
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	return;
}

// ================================== ���� �����Լ� �������̵� ============================================

//===================================================================================
// ��� ���� ������ ����
//===================================================================================
void Spacewar::update()
{

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

	graphics->spriteEnd();
}

//==================================================================================================

//===================================================================================
// �׷��� ����̽��� �ν�Ʈ ���°� ���� ��, ����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� ������ �� �ְ� �Ѵ�.(ȣ�� ������ �������� )
//===================================================================================
void Spacewar::releaseAll()
{
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
	
	Game::resetAll();
	return;
}





