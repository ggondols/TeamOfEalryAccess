#pragma once
class TeicEffectParent
{
protected:
	virtual void Update() = 0;
	virtual void Start() = 0;
	virtual void Render() = 0;
public:
	TeicEffectParent();
	~TeicEffectParent();
};

