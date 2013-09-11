#include "Dx11DemoBase.h"
#include <xnamath.h>

Dx11DemoBase::Dx11DemoBase() : 
	driverType_(D3D_DRIVER_TYPE_NULL),
	featureLevel_(D3D_FEATURE_LEVEL_11_0),
	d3dDevice_(NULL),
	d3dDeviceContext_(NULL),
	swapChain_(NULL),
	backBufferTarget_(NULL)
{
}

Dx11DemoBase::~Dx11DemoBase()
{
	this->Shutdown();
}

BOOL Dx11DemoBase::LoadContent()
{
	return TRUE;
}

void Dx11DemoBase::UnloadContent()
{
}

void Dx11DemoBase::Shutdown()
{
	this->UnloadContent();

	if (backBufferTarget_) backBufferTarget_->Release();
	if (swapChain_) swapChain_->Release();
	if (d3dDeviceContext_) d3dDeviceContext_->Release();
	if (d3dDevice_) d3dDevice_->Release();

	d3dDevice_ = NULL;
	d3dDeviceContext_ = NULL;
	swapChain_ = NULL;
	backBufferTarget_ = NULL;
}

BOOL Dx11DemoBase::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	this->hInstance_ = hInstance;
	this->hwnd_ = hwnd;

	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	UINT width = dimensions.right - dimensions.left;
	UINT height = dimensions.bottom - dimensions.top;

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE,
	};

	UINT totalDriverTypes = static_cast<UINT>(ARRAYSIZE(driverTypes));

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT totalFeatureLevels = static_cast<UINT>(ARRAYSIZE(featureLevels));

	// SwapChain Desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	UINT creationFlags = 0;

#if defined (_DEBUG) || defined (DEBUG) 
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	UINT driver = 0;

	for (driver = 0; driver < totalDriverTypes; driver++)
	{
		result = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverTypes[driver],
			NULL,
			creationFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain_,
			&d3dDevice_,
			&featureLevel_,
			&d3dDeviceContext_);

		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}

	if (FAILED(result))
	{
		DXTRACE_ERR("Failed to create the Direct3D device!", result);
		return FALSE;
	}

	ID3D11Texture2D* backBufferTexture;
	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

	if (FAILED(result))
	{
		DXTRACE_ERR("Failed to get the swap chain back buffer!", result);
		return FALSE;
	}

	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, NULL, &backBufferTarget_);

	if (backBufferTexture)
		backBufferTexture->Release();

	if (FAILED(result))
	{
		DXTRACE_ERR("Failed to create the render target view!", result);
		return FALSE;
	}

	d3dDeviceContext_->OMSetRenderTargets(1, &backBufferTarget_, NULL);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	d3dDeviceContext_->RSSetViewports(1, &viewport);

	return this->LoadContent();
}

BOOL Dx11DemoBase::CompileD3DShader(LPCWSTR filePath, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined (DEBUG) || defined (_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBuffer = NULL;
	HRESULT result;

	result = D3DX11CompileFromFileW(filePath, 
		NULL, 
		NULL, 
		entry, 
		shaderModel,
		shaderFlags,
		0,
		NULL,
		buffer,
		&errorBuffer,
		NULL);

	if (FAILED(result))
	{
		if (errorBuffer != NULL)
		{
			OutputDebugString((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}

		return FALSE;
	}

	if (errorBuffer != NULL)
	{
		errorBuffer->Release();
	}

	return TRUE;
}

BOOL Dx11DemoBase::LoadPrecompiledShader(LPCWSTR filePath, ID3DBlob** buffer)
{
	ID3DBlob* errorBuffer = NULL;
	HRESULT result = D3DX11PreprocessShaderFromFileW(filePath,
		NULL,
		NULL,
		NULL,
		buffer,
		&errorBuffer,
		NULL);

	if (FAILED(result))
	{
		if (errorBuffer != NULL)
		{
			OutputDebugString((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}

		return FALSE;
	}

	if (errorBuffer != NULL)
	{
		errorBuffer->Release();
	}

	return TRUE;
}