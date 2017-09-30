#pragma once
class TeicEffectParent
{
public:
	virtual void Update() = 0;
	virtual void Start() = 0;
	virtual void Render() = 0;
	virtual void Stop() = 0;
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos) = 0;
	virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos) = 0;
	virtual bool IsRunning() = 0;
public:
	void Release() {}
public:
	TeicEffectParent();
	~TeicEffectParent();
};

