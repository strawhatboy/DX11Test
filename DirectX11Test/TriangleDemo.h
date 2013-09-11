#ifndef _TRIANGLEDEMO_H_
#define _TRIANGLEDEMO_H_

#pragma once
#include "dx11demobase.h"

class TriangleDemo :
	public Dx11DemoBase
{
public:
	TriangleDemo(void);
	virtual ~TriangleDemo(void);

	BOOL LoadContent();
	void UnloadContent();

	void Update(float dt);
	void Render();

private:
	ID3D11VertexShader* m_solidColorVS;
	ID3D11PixelShader* m_solidColorPS;

	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_vertexBuffer;
};

#endif