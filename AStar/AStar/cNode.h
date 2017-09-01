#pragma once
class cNode
{
public:
	enum eNodeType
	{
		E_NONE,
		E_EMPTY,
		E_WALL,
		E_START,
		E_DEST,
		E_CLOSE,
		E_OPEN,
		E_PATH
	};

	cNode(int nNodeIndex, eNodeType e);
	~cNode(void);

	float		F;
	float		G;
	float		H;
	cNode*		m_pParent;
	bool		m_isClose;
	bool		m_isOpen;
	int			m_nRow;
	int			m_nCol;
	RECT		m_rc;
	eNodeType	m_eNodeType;

	void Render(HDC hdc);
};

