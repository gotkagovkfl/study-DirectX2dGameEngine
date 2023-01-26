// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine Part 1
// Chapter 4 spacewar.cpp v1.0
// Spacewar is the class we create.

#include "spaceWar.h"


//===================================================================================
// 생성자
//===================================================================================
Spacewar::Spacewar()
{

}

//===================================================================================
// 소멸자
//===================================================================================
Spacewar::~Spacewar()
{
	releaseAll();
}

//===================================================================================
// 게임 초기화
//===================================================================================
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	// 부모클래스 초기화 함수 호출

	// 텍스처 및 이미지 초기화
	// 성운 텍스처 
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// 행성 텍스처 
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));
	
	// 성운 이미지
	if (!nebula.initialize(graphics, 0,0,0,&nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	
	// 행성 이미지 
	if (!planet.initialize(graphics,0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// 행성 위치 조정 ( 성운은 0,0에 고정됨)
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	return;
}

// ================================== 순수 가상함수 오버라이드 ============================================

//===================================================================================
// 모든 게임 아이템 갱신
//===================================================================================
void Spacewar::update()
{

}

//===================================================================================
// 인공지능
//===================================================================================
void Spacewar::ai()
{

}

//===================================================================================
// 충돌처리
//===================================================================================
void Spacewar::collision()
{

}

//===================================================================================
// 게임 아이템 렌더링
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
// 그래픽 디바이스가 로스트 상태가 됐을 때, 예약된 모든 비디오 메모리를 해제하고 그래픽 디바이스를 리셋할 수 있게 한다.(호출 순서의 역순으로 )
//===================================================================================
void Spacewar::releaseAll()
{
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	
	Game::releaseAll();
	return;
}

//===================================================================================
// 그래픽 디바이스를 리셋, 모든 표면 재생성
//===================================================================================
void Spacewar::resetAll()
{
	planetTexture.onResetDevice();
	nebulaTexture.onResetDevice();
	
	Game::resetAll();
	return;
}





