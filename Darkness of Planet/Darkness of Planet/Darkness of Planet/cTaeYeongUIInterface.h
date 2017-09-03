#pragma once
enum UItype { SHOW, TEXT, BUTTON, DRAG, NOTHING };
class cTaeYeongUIInterface
{
private:
	LPD3DXSPRITE	m_pSprite;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXIMAGE_INFO		m_stImageInfo;
	UItype				m_Type;
	D3DXMATRIX          m_World;
	D3DXMATRIX          m_Local;
	float               m_startX;
	float               m_startY;
	float               m_FinishX;
	float               m_FinishY;
	D3DXVECTOR2         m_Point;
	D3DXCOLOR			m_Color;
	vector<cTaeYeongUIInterface*>       m_vecChild;
	cTaeYeongUIInterface*       m_pTarget;
	UItype				m_typeTarget;
	D3DXMATRIX			m_BasicTranslation;
	RECT				m_Rc;
	D3DXVECTOR3         m_CollisionVec[4];
	bool				m_Drag;
	LPD3DXFONT	        m_pFont;
	char				str[2048];
	bool				m_bClose;
	CallbackBindFunction	CallBackF;
public:
	void Setup(char* szFolder, char* szFilename, float Startx, float Starty, UItype type, D3DXMATRIX& Local);
	void Setup(char* szFolder, char* szFilename, float Startx, float Starty, UItype type);

	void Update();
	void Render(cTaeYeongUIInterface * parent);
	void AddChild(cTaeYeongUIInterface* child);
	void SetTarget(cTaeYeongUIInterface* target, UItype type = NOTHING) { m_pTarget = target; m_typeTarget = type; }
	bool Collision();
	bool LineDotDown(D3DXVECTOR3 startDot, D3DXVECTOR3 finishDot, D3DXVECTOR3 targetDot);
	void SetText(char* text);
	void SetClose(bool OnOff) { m_bClose = OnOff; }
	void regi_func(CallbackBindFunction p_func)
	{
		CallBackF = std::move(p_func);
	}
public:
	cTaeYeongUIInterface();
	~cTaeYeongUIInterface();
};

