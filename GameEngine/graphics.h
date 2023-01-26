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

// DirectX ������ Ÿ��
#define LP_TEXTURE	LPDIRECT3DTEXTURE9
#define LP_SPRITE	LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// ���� ����
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


// SpriteData : Graphics::drawSprtie���� ��������Ʈ�� �׸��� ���� �ʿ��� �Ӽ�
struct SpriteData
{
	int width;			// �ȼ� ���� ��
	int height;			// �ȼ� ���� ����
	float x;			// ȭ�� ��ġ (���� ��� �𼭸� ����)
	float y;
	float scale;		// <1 �̸� �۰�, >1�̸� ũ��
	float angle;		// ���� ���� ȸ�� ����
	RECT rect;			// ū �ؽ�ó���� ����� �̹��� ����
	LP_TEXTURE texture;		// �ؽ�ó �̹����� ����Ű�� ������
	bool flipHorizontal;		// true��� �������� ������
	bool flipVertical;			// true��� �������� ������
};


//
class Graphics
{
private:
	// DirectX ������ �� ���� ����
	LP_3D		direct3d;
	LP_3DDEVICE	device3d;
	LP_SPRITE	sprite;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// �ٸ� ����
	HRESULT	result;
	HWND	hwnd;
	bool	fullscreen;
	int		width;
	int		height;
	COLOR_ARGB backColor;
	
	// 
	void	initD3Dpp(); // D3D ���������̼� �Ű����� �ʱ�ȭ 

public :
	// ������
	Graphics();

	// �Ҹ���
	virtual ~Graphics();

	// ���� �Ҵ� �� direct3d�� device3d�� ����
	void releaseAll();	

	// DirectX �׷����� �ʱ�ȭ
	void initialize(HWND hw, int width, int height, bool fullscreen);

	// ȭ�鿡 ������ �ʴ� �� ���۸� ȭ�鿡 ǥ�� 
	HRESULT showBackbuffer();

	// ����Ͱ� d3dpp�� ������ �� ������ ����, ��, �������üӵ��� ȣȯ�Ǵ��� Ȯ���Ͽ� pMode ����ü�� ȣȯ ����� ������ ä���. 
	bool isAdapterCompatible();

	// �ν�Ʈ ���°� �� ����̽��� Ȯ���Ѵ�.
	HRESULT getDeviceState();

	// �׷��� ����̽� ���� 
	HRESULT reset();

	//============================== ��������Ʈ ���� ===========================================
	// �ؽ�ó�� �⺻ D3D �޸𸮷� �ҷ��´�. 
	HRESULT loadTexture(const char* file, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture);

	// SpriteData ����ü�� ������ ���� ��������Ʈ�� �׸���. 
	void drawSprite(const SpriteData& spriteData, COLOR_ARGB color = graphicsNS::WHITE);

	// =========================  �������� ���� �� ���� �Լ� ====================================
	// ================================================================================================
	// ����۸� ����� DirectX�� BeginScene ȣ��
	// ================================================================================================
	HRESULT beginScene()
	{
		result = E_FAIL;
		if (device3d == NULL)
			return result;

		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0); // �����
		result = device3d->BeginScene();	// �� ����
		return result;
	}

	// ================================================================================================
	// DirectX�� EndScene ȣ��
	// ================================================================================================
	HRESULT endScene()
	{
		result = E_FAIL;
		if (device3d)
			result = device3d->EndScene();
		return result;
	}

	// ================================================================================================
	// ��������Ʈ ����
	// ================================================================================================
	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	// ================================================================================================
	// ��������Ʈ ��
	// ================================================================================================
	void spriteEnd()
	{
		sprite->End();
	}

};

#endif
