#pragma once
class cGameNode
{
protected:
private:
	bool _managerInit;


public:
	cGameNode();
	virtual ~cGameNode();

	virtual HRESULT Setup();
	virtual HRESULT Setup(bool _managerInit);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	bool About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	//virtual INT_PTR CALLBACK  About(HWND, UINT, WPARAM, LPARAM);


};
