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

// DirectX 포인터 타입
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// 색깔 정의
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

//
class Graphics
{
private:
	// DirectX 포인터 및 관련 변수
	LP_3D		direct3d;
	LP_3DDEVICE	device3d;
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

};

#endif
