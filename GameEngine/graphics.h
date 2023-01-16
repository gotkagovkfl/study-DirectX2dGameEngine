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
#include "constants.h"
#include "gameError.h"

// DirectX ������ Ÿ��
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// ���� ����
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

//
class Graphics
{
private:
	// DirectX ������ �� ���� ����
	LP_3D		direct3d;
	LP_3DDEVICE	device3d;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// �ٸ� ����
	HRESULT	result;
	HWND	hwnd;
	bool	fullscreen;
	int		width;
	int		height;
	
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
};

#endif
