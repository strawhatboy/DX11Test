#ifndef _DX11DEMOBASE_H_
#define _DX11DEMOBASE_H_

#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>

class Dx11DemoBase
{
public:
	Dx11DemoBase();
	virtual ~Dx11DemoBase();

	BOOL Initialize(HINSTANCE hInstance, HWND hwnd);
	void Shutdown();

	virtual BOOL LoadContent();
	virtual void UnloadContent();
	BOOL CompileD3DShader(LPCWSTR filePath, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer);
	BOOL LoadPrecompiledShader(LPCWSTR filePath, ID3DBlob** buffer);

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	HINSTANCE hInstance_;
	HWND hwnd_;
	
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;

	ID3D11Device* d3dDevice_;
	ID3D11DeviceContext* d3dDeviceContext_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* backBufferTarget_;
};

#endif