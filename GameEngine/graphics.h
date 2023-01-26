// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 3 graphics.h v1.1
// Last modification: March-6-2015

#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

class Graphics;

#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

// DirectX 포인터 타입
#define LP_TEXTURE	LPDIRECT3DTEXTURE9
#define LP_SPRITE	LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// 색깔 정의
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

namespace graphicsNS
{
	// Some common colors
	// ARGB numbers range from 0 through 255
	// A = Alpha channel (transparency where 255 is opaque)
	// R = Red, G = Green, B = Blue
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);

	// 
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

	//
	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}


// SpriteData : Graphics::drawSprtie에서 스프라이트를 그리기 위해 필요한 속성
struct SpriteData
{
	int width;			// 픽셀 단위 폭
	int height;			// 픽셀 단위 높이
	float x;			// 화면 위치 (왼쪽 상단 모서리 기준)
	float y;
	float scale;		// <1 이면 작게, >1이면 크게
	float angle;		// 라디안 단위 회전 각도
	RECT rect;			// 큰 텍스처에서 사용할 이미지 선택
	LP_TEXTURE texture;		// 텍스처 이미지를 가리키는 포인터
	bool flipHorizontal;		// true라면 수평으로 뒤집음
	bool flipVertical;			// true라면 수직으로 뒤집음
};


//
class Graphics
{
private:
	// DirectX 포인터 및 관련 변수
	LP_3D		direct3d;
	LP_3DDEVICE	device3d;
	LP_SPRITE	sprite;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// 다른 변수
	HRESULT	result;
	HWND	hwnd;
	bool	fullscreen;
	int		width;
	int		height;
	COLOR_ARGB backColor;
	
	// 
	void	initD3Dpp(); // D3D 프레젠테이션 매개변수 초기화 

public :
	// 생성자
	Graphics();

	// 소멸자
	virtual ~Graphics();

	// 동적 할당 된 direct3d와 device3d를 해제
	void releaseAll();	

	// DirectX 그래픽을 초기화
	void initialize(HWND hw, int width, int height, bool fullscreen);

	// 화면에 보이지 않는 백 버퍼를 화면에 표시 
	HRESULT showBackbuffer();

	// 어댑터가 d3dpp에 지정된 백 버퍼의 높이, 폭, 리프레시속도와 호환되는지 확인하여 pMode 구조체에 호환 모드의 형식을 채운다. 
	bool isAdapterCompatible();

	// 로스트 상태가 된 디바이스를 확인한다.
	HRESULT getDeviceState();

	// 그래픽 디바이스 리셋 
	HRESULT reset();

	//============================== 스프라이트 관련 ===========================================
	// 텍스처를 기본 D3D 메모리로 불러온다. 
	HRESULT loadTexture(const char* file, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture);

	// SpriteData 구조체의 정보를 토대로 스프라이트를 그린다. 
	void drawSprite(const SpriteData& spriteData, COLOR_ARGB color = graphicsNS::WHITE);

	// =========================  렌더링을 위한 씬 관련 함수 ====================================
	// ================================================================================================
	// 백버퍼를 지우고 DirectX의 BeginScene 호출
	// ================================================================================================
	HRESULT beginScene()
	{
		result = E_FAIL;
		if (device3d == NULL)
			return result;

		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0); // 배경깔고
		result = device3d->BeginScene();	// 씬 시작
		return result;
	}

	// ================================================================================================
	// DirectX의 EndScene 호출
	// ================================================================================================
	HRESULT endScene()
	{
		result = E_FAIL;
		if (device3d)
			result = device3d->EndScene();
		return result;
	}

	// ================================================================================================
	// 스프라이트 시작
	// ================================================================================================
	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	// ================================================================================================
	// 스프라이트 끝
	// ================================================================================================
	void spriteEnd()
	{
		sprite->End();
	}

};

#endif
