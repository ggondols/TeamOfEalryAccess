#pragma once

namespace Hank {
	class cGrid
	{
	private:
		std::vector<ST_PC_VERTEX>	m_vecVertex;
		//cPyramidLight *pyramidR;
		//cPyramidLight *pyramidG;
	//	cPyramidLight *pyramidB;


		//그래픽 카드 랜더링
		IDirect3DVertexBuffer9* vb;
		ST_PC_VERTEX *vertices;

	public:
		cGrid();
		~cGrid();

		void Release(void);

		void Setup(int nNumLine = 20, float fInterval = 1.0f);
		void Render();
	};
}