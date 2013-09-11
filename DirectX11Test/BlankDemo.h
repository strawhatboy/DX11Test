#ifndef _BLANKDEMO_H_
#define _BLANKDEMO_H_

#pragma once

#include "dx11demobase.h"


class BlankDemo :
	public Dx11DemoBase
{
public:
	BlankDemo(void);
	virtual ~BlankDemo(void);

	BOOL LoadContent();
	void UnloadContent();

	void Update(float dt);
	void Render();

	int count;
};

#endif