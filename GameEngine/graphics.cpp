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
	safeRelease(device3d);
	safeRelease(direct3d);
}
