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

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprtie"));
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
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}



//===============================================================================
// �ؽ�ó�� �⺻ D3D �޸𸮷� �ҷ��´�. 
//===============================================================================
HRESULT Graphics::loadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture)
{
	D3DXIMAGE_INFO info;	// ���� ������ �б� ���� ����ü
	result = E_FAIL;
	
	try
	{
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		// ���Ϸκ��� ���� ���̸� ��´�. 
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// ������ �ҷ��� �� �ؽ�ó�� �����Ѵ�.
		result = D3DXCreateTextureFromFileEx(
			device3d,			//3d ����̽�
			filename,			// �̹��� ���ϸ�
			info.Width,			
			info.Height,
			1,					// �Ӹ� ���� (�������� ���� �� 1)
			0,					// ��� ()
			D3DFMT_UNKNOWN,		// ǥ�� ���� (�⺻)
			D3DPOOL_DEFAULT,	// �ؽ�ó�� ������ �޸� Ŭ����
			D3DX_DEFAULT,		// �̹��� ����
			D3DX_DEFAULT,		// �� ����
			transcolor,			// ������ ���� ���� Ű
			&info,				// ��Ʈ�� ���� ���� (�ҷ��� ���Ϸκ���)
			NULL,				// ���� �ȷ�Ʈ
			&texture			// �ؽ�ó ������
		);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}

//===============================================================================
// SpriteData ����ü�� ������ ���� ��������Ʈ�� �׸���. 
//===============================================================================
void Graphics::drawSprite(const SpriteData& spriteData, COLOR_ARGB color)
{
	// �ؽ�ó�� ���ٸ� ��ȯ
	if (spriteData.texture == NULL)
		return;
	
	// ��������Ʈ�� �߽��� ã�´�. 
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale), (float)(spriteData.height / 2 * spriteData.scale));

	// ��������Ʈ�� ȭ�� ��ġ
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	
	// x,y Ȯ�� ���� ����
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	// �������� ���� ��ǥ ������ (����, ����)
	if (spriteData.flipHorizontal)		// ����
	{
		// ������ ���� x ���⿡ -1�� ���Ѵ�. 
		scaling.x *= -1;

		// ������ �̹����� �߽��� �����´�. 
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);

		// �̵���Ű�� 
		translate.x += (float)(spriteData.width * spriteData.scale);
	}
	if (spriteData.flipVertical)	// ����
	{
		scaling.y *= -1;

		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);

		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	// ��������Ʈ�� ȸ��, ũ�� ����, ��ġ�ϱ� ���� ����� �����Ѵ�. 
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,		// ���					
		NULL,			// ũ�⸦ ������ �� ������ ���� ������� ����
		0.0f,			// ũ�� ���� ȸ�� ����
		&scaling,		// ũ�� ������
		&spriteCenter,	// ȸ�� �߽�
		(float)(spriteData.angle),		// ȸ�� ����
		&translate			// x,y ��ġ
	);

	// ��Ŀ� ���� ��������Ʈ ������ �����Ѵ�.
	sprite->SetTransform(&matrix);

	// ��������Ʈ�� �׸���.
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}
