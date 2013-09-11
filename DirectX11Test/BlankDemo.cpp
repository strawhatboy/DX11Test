#include "BlankDemo.h"


BlankDemo::BlankDemo(void)
{
}


BlankDemo::~BlankDemo(void)
{
}

BOOL BlankDemo::LoadContent()
{
	return TRUE;
}

void BlankDemo::UnloadContent()
{
}

void BlankDemo::Update(float dt)
{
}

void BlankDemo::Render()
{
	if (!d3dDeviceContext_)
		return;

	if (count > 1000000 || count < 0)
		count = 0;
	else
		count++;

	int red, green, blue, tmp;
	tmp = count;
	blue = tmp % 10000;
	tmp = tmp / 100;
	green = tmp % 100;
	tmp = tmp / 100;
	red = tmp;
	float fRed = red / 100.0f;
	float fGreen = green / 100.f;
	float fBlue = blue / 100.f;

	FLOAT clearColor[] = {fRed, fGreen, fBlue, 1.0f};
	d3dDeviceContext_->ClearRenderTargetView(backBufferTarget_, clearColor);

	swapChain_->Present(0, 0);
}