#include "graphics.h"

//================================================================================
// 생성자
//===============================================================================
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;

	backColor = SETCOLOR_ARGB(255, 0, 0, 128); // dark blue;
}

//================================================================================
// 소멸자
//===============================================================================
Graphics::~Graphics()
{
	releaseAll();
}


// ================================================================================
// DirectX graphics 초기화
// ================================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	// Direct3D 초기화
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error inializing Direct3D"));

	// D3D 프레젠테이션 매개변수 초기화
	initD3Dpp();

	//전체화면 모드일때 어댑터가 호환되는지 확인하여 리프레시 속도를 호환되는 것으로 설정
	if (fullscreen)
	{
		if (isAdapterCompatible())
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution and/or format"));
	}

	// 그래픽 카드가 텍스처, 라이팅, 정점 셰이더를 지원하는지 확인
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// 디바이스가 ~를 지원하지 않는다면 소프트웨어 정점처리로 전환함.
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing


	// Direct3D 디바이스 생성
	result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior, &d3dpp, &device3d);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprtie"));
}

//===================================================================================
// D3D 프레젠테이션 매개변수 초기화
//===================================================================================
void Graphics::initD3Dpp()
{
	try
	{
		// 구조체를 0으로 채우기
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		// 필요한 매개변수 채우기
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)                                  // if fullscreen
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24 bit color
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // use desktop setting
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 백버퍼를 보여주고 있을 때 이전 화면의 버퍼 내용 삭제 지시 
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

//=============================================================================
//  백 버퍼 표시
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // 기본값은 FAIL. 성공시 바뀜

	// 백버퍼를 화면에 표시 
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

//============================================================================
// 어댑터가 d3dpp에 지정된 백 버퍼의 높이, 폭, 리프레시 속도와 호환되는지 확인하여,  pMode 구조체에 호환 모드의 형식을 채운다. 
//============================================================================
bool Graphics::isAdapterCompatible()
{
	// 지정된 어탭터에 대해 사용가능한 디스플레이 모드의 수 반환
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);

	// 호환되는 모드를 찾았고, pMode 구조체가 채워져 있는지 확인. 
	for (UINT i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);

		if (pMode.Height == d3dpp.BackBufferHeight && pMode.Width == d3dpp.BackBufferWidth && pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

// ================================================================================================
// 로스트 상태가 된 디바이스를 확인
// ================================================================================================
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;
	if (device3d == NULL)
		return result;
	result = device3d->TestCooperativeLevel(); //디바이스 로스트가 발생했는지 테스트
	return result;
}

// ================================================================================================
// 그래픽 디바이스 리셋 - 
// ================================================================================================
HRESULT Graphics::reset()
{
	result = E_FAIL;
	initD3Dpp();
	result = device3d->Reset(&d3dpp);
	
	return result;
}

//=============================================================================
// Release all
//=============================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}



//===============================================================================
// 텍스처를 기본 D3D 메모리로 불러온다. 
//===============================================================================
HRESULT Graphics::loadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture)
{
	D3DXIMAGE_INFO info;	// 파일 정보를 읽기 위한 구조체
	result = E_FAIL;
	
	try
	{
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		// 파일로부터 폭과 높이를 얻는다. 
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// 파일을 불러와 새 텍스처를 생성한다.
		result = D3DXCreateTextureFromFileEx(
			device3d,			//3d 디바이스
			filename,			// 이미지 파일명
			info.Width,			
			info.Height,
			1,					// 밉맵 수준 (연결하지 않을 땐 1)
			0,					// 사용 ()
			D3DFMT_UNKNOWN,		// 표면 형식 (기본)
			D3DPOOL_DEFAULT,	// 텍스처를 ㅜ이한 메모리 클래스
			D3DX_DEFAULT,		// 이미지 필터
			D3DX_DEFAULT,		// 밉 필터
			transcolor,			// 투명도를 위한 색상 키
			&info,				// 비트맵 파일 정보 (불러온 파일로부터)
			NULL,				// 색상 팔레트
			&texture			// 텍스처 목적지
		);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}

//===============================================================================
// SpriteData 구조체의 정보를 토대로 스프라이트를 그린다. 
//===============================================================================
void Graphics::drawSprite(const SpriteData& spriteData, COLOR_ARGB color)
{
	// 텍스처가 없다면 반환
	if (spriteData.texture == NULL)
		return;
	
	// 스프라이트의 중심을 찾는다. 
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale), (float)(spriteData.height / 2 * spriteData.scale));

	// 스프라이트의 화면 위치
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	
	// x,y 확대 배율 조정
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	// 뒤집힘에 맞춰 좌표 재조정 (수평, 수직)
	if (spriteData.flipHorizontal)		// 수평
	{
		// 뒤집기 위해 x 방향에 -1을 곱한다. 
		scaling.x *= -1;

		// 뒤집은 이미지의 중심을 가져온다. 
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);

		// 이동시키기 
		translate.x += (float)(spriteData.width * spriteData.scale);
	}
	if (spriteData.flipVertical)	// 수직
	{
		scaling.y *= -1;

		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);

		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	// 스프라이트를 회전, 크기 조정, 배치하기 위한 행렬을 생성한다. 
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,		// 행렬					
		NULL,			// 크기를 조정할 때 기준을 외쪽 상단으로 유지
		0.0f,			// 크기 조정 회전 없음
		&scaling,		// 크기 조정값
		&spriteCenter,	// 회전 중심
		(float)(spriteData.angle),		// 회전 각도
		&translate			// x,y 위치
	);

	// 행렬에 맞춰 스프라이트 정보를 조정한다.
	sprite->SetTransform(&matrix);

	// 스프라이트를 그린다.
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}
