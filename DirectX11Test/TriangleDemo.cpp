#include "TriangleDemo.h"
#include <xnamath.h>

struct VertexPos
{
	XMFLOAT3 pos;
};

TriangleDemo::TriangleDemo(void):
	m_solidColorPS(0),
	m_solidColorVS(0),
	m_inputLayout(0),
	m_vertexBuffer(0)
{
}


TriangleDemo::~TriangleDemo(void)
{
}

void TriangleDemo::UnloadContent()
{
	if (m_solidColorPS) m_solidColorPS->Release();
	if (m_solidColorVS) m_solidColorVS->Release();
	if (m_inputLayout) m_inputLayout->Release();
	if (m_vertexBuffer) m_vertexBuffer->Release();

	m_solidColorPS = NULL;
	m_solidColorVS = NULL;
	m_inputLayout = NULL;
	m_vertexBuffer = NULL;
}

BOOL TriangleDemo::LoadContent()
{
#define _TRIANGLEDEMO_CPP_TRIANGLEDEMO_LOADCONTENT_SHADER_LOADING_ "Shader Loading code part"
#ifdef _TRIANGLEDEMO_CPP_TRIANGLEDEMO_LOADCONTENT_SHADER_LOADING_
	ID3DBlob* vsBuffer = NULL;

	BOOL compileResult = CompileD3DShader(L"TestVertexShader.hlsl", "main", "vs_4_0", &vsBuffer);

	//BOOL compileResult = LoadPrecompiledShader(L"D:\\v-chucui\\My Documents\\Visual Studio 2012\\Projects\\C++Win32\\DirectX11Test\\ReleaseTestVertexShader.cso", &vsBuffer);

	if (compileResult == FALSE)
	{
		MessageBoxW(hwnd_, L"Error loading vertex shader!", L"Compile Error", MB_OK);
		return FALSE;
	}

	HRESULT result;
	result = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		NULL,
		&m_solidColorVS);

	if(FAILED(result))
	{
		if (vsBuffer)
			vsBuffer->Release();

		return FALSE;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	UINT totalLayoutElements = ARRAYSIZE(solidColorLayout);

	result = d3dDevice_->CreateInputLayout(
		solidColorLayout,
		totalLayoutElements, 
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		&m_inputLayout);

	vsBuffer->Release();

	if (FAILED(result))
	{
		return FALSE;
	}
	
	ID3DBlob* psBuffer = NULL;

	result = CompileD3DShader(L"TestPixelShader.hlsl",
		"main", "ps_4_0", &psBuffer);

	if (result == FALSE)
	{
		MessageBoxW(hwnd_, L"Error loading pixel shader!", L"Compile Error", MB_OK);
		return FALSE;
	}

	result = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		NULL,
		&m_solidColorPS);

	psBuffer->Release();

	if (FAILED(result))
		return FALSE;
#endif

#define _TRIANGLEDEMO_CPP_TRIANGLEDEMO_LOADCONTENT_GEOMETRY_LOADING_ "Geometry Loading code part"
#ifdef _TRIANGLEDEMO_CPP_TRIANGLEDEMO_LOADCONTENT_GEOMETRY_LOADING_

	VertexPos vertices[] =
	{
		XMFLOAT3(0.1f,	0.2f,	0.3f),
		XMFLOAT3(0.1f,	-0.2f,	0.3f),
		XMFLOAT3(-0.1f,	-0.2f,	0.3f),
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(D3D11_BUFFER_DESC));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 3;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = vertices;

	result = d3dDevice_->CreateBuffer(&vertexDesc,
		&resourceData,
		&m_vertexBuffer);

	if (FAILED(result))
		return FALSE;

	return TRUE;
#endif
}

void TriangleDemo::Update(float dt)
{
}


void TriangleDemo::Render()
{
	if (d3dDeviceContext_ == NULL)
	{
		return;
	}

	float clearColor[] = {0.0f, 0.0f, 0.25f, 1.0f};
	d3dDeviceContext_->ClearRenderTargetView(backBufferTarget_, clearColor);

	UINT stride = sizeof(VertexPos);
	UINT offset = 0;

	d3dDeviceContext_->IASetInputLayout(m_inputLayout);
	d3dDeviceContext_->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	d3dDeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext_->VSSetShader(m_solidColorVS, NULL, 0);
	d3dDeviceContext_->PSSetShader(m_solidColorPS, NULL, 0);
	d3dDeviceContext_->Draw(3, 0);

	swapChain_->Present(0, 0);
}