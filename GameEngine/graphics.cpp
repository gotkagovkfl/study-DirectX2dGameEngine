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

	backColor = SETCOLOR_ARGB(255, 0, 0, 128); // dark blue;
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

	//��üȭ�� ����϶� ����Ͱ� ȣȯ�Ǵ��� Ȯ���Ͽ� �������� �ӵ��� ȣȯ�Ǵ� ������ ����
	if (fullscreen)
	{
		if (isAdapterCompatible())
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution and/or format"));
	}

	// �׷��� ī�尡 �ؽ�ó, ������, ���� ���̴��� �����ϴ��� Ȯ��
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// ����̽��� ~�� �������� �ʴ´ٸ� ����Ʈ���� ����ó���� ��ȯ��.
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

	// ����۸� ȭ�鿡 ǥ�� 
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

//============================================================================
// ����Ͱ� d3dpp�� ������ �� ������ ����, ��, �������� �ӵ��� ȣȯ�Ǵ��� Ȯ���Ͽ�,  pMode ����ü�� ȣȯ ����� ������ ä���. 
//============================================================================
bool Graphics::isAdapterCompatible()
{
	// ������ �����Ϳ� ���� ��밡���� ���÷��� ����� �� ��ȯ
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);

	// ȣȯ�Ǵ� ��带 ã�Ұ�, pMode ����ü�� ä���� �ִ��� Ȯ��. 
	for (UINT i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);

		if (pMode.Height == d3dpp.BackBufferHeight && pMode.Width == d3dpp.BackBufferWidth && pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

// ================================================================================================
// �ν�Ʈ ���°� �� ����̽��� Ȯ��
// ================================================================================================
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;
	if (device3d == NULL)
		return result;
	result = device3d->TestCooperativeLevel(); //����̽� �ν�Ʈ�� �߻��ߴ��� �׽�Ʈ
	return result;
}

// ================================================================================================
// �׷��� ����̽� ���� - 
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
