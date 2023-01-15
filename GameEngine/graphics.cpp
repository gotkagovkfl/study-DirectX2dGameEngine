#include "graphics.h"

//================================================================================
// ������
//===============================================================================
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
}

//================================================================================
// �Ҹ���
//===============================================================================
Graphics::~Graphics()
{
	releaseAll();
}


// ================================================================================
// DirectX graphics �ʱ�ȭ
// ================================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	// Direct3D �ʱ�ȭ
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error inializing Direct3D"));

	// D3D ���������̼� �Ű����� �ʱ�ȭ
	initD3Dpp();


	// determine if graphics card supports harware texturing and lighting and vertex shaders
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// If device doesn't support HW T&L or doesn't support 1.1 vertex 
	// shaders in hardware, then switch to software vertex processing.
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing



	// Direct3D ����̽� ����
	result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior, &d3dpp, &device3d);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));
}

//===================================================================================
// D3D ���������̼� �Ű����� �ʱ�ȭ
//===================================================================================
void Graphics::initD3Dpp()
{
	try
	{
		// ����ü�� 0���� ä���
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		// �ʿ��� �Ű����� ä���
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)                                  // if fullscreen
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24 bit color
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // use desktop setting
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ����۸� �����ְ� ���� �� ���� ȭ���� ���� ���� ���� ���� 
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
//  �� ���� ǥ��
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // �⺻���� FAIL. ������ �ٲ�

	// (this function will be moved in later versions)
	// 
	// ����۸� ä���  (rgb 0 255 0 )
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 222, 255), 0.0f, 0);

	// ����۸� ȭ�鿡 ǥ�� 
	result = device3d->Present(NULL, NULL, NULL, NULL);

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
