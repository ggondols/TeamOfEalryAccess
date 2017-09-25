#pragma once

class LDYCharacter;

class LDYMotionBlur
{

private:


public:
	LDYMotionBlur();
	~LDYMotionBlur();

	void Setup();
	void Update();
	void Render();

	LPD3DXEFFECT LoadEffect(const char* szFileName);
};

