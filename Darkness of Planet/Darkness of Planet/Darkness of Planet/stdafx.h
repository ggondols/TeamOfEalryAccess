// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//전역 변수
extern HWND g_hWnd;
extern HINSTANCE hInst;
extern POINT g_ptMouse;
extern bool g_isLButtonDown;
extern bool g_isRButtonDown;
extern bool g_isLButtonUp;
extern bool g_isRButtonUp;
extern int g_MouseWheel;



//다이렉트 X 라이브러리
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3dx9.h>
#include <vector>
#include <map>
#include <set>
#include <assert.h>
#include <list>
#include <string>
#include <functional>  //////바인드 함수를 이용하기 위한 헤더


//필요한 Define은 여기서 선언 합니다.
#pragma warning(disable : 4996)

#define SAFE_ADD_REF(p) if(p) { (p)->AddRef(); }
#define SAFE_RELEASE(p) if(p){(p)->Release(); (p)=NULL;}
#define SAFE_DELETE(p) if(p){delete (p); p=NULL;}
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p) ; p =NULL;}
#define INF 999999.f

//## 메니저 디파인 여기 ##//
#define SCENEMANAGER cSceneManager::getSingleton()
#define DEVICEMANAGER cDeviceManager::getSingleton()
#define TIMEMANAGER	cTimeManager::getSingleton()
#define KEYMANAGER cKeyManager::getSingleton()
#define MATERIALMANAGER cMaterialManager::getSingleton()
#define TEXTUREMANAGER cTextureManager::getSingleton()
#define FONTMANAGER cFontManager::getSingleton()
#define UIOBJECTMANAGER UIObjectManager::getSingleton()

//## 일반적인 클래스 메크로##//
#define RND cRandomFunction::getSingleton()

#define OBJECTPOOL cObjectPool::getSingleton()

//## 디바이스 메크로##//
#define GETDEVICE cDeviceManager::getSingleton()->GetDevice()


//## SYNTHESIZE 
//싱글턴 다른형태
#define SINGLETON(class_name)	private:\
	class_name(void);\
	~class_name(void);\
public:\
	static class_name* GetInstance() { static class_name instance; return &instance; }

//겟터 세터 자동 생성
#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

//겟터 세터 레퍼런스 자동 생성
#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(const varType& var){ varName = var; }

//ADD 레퍼런스 자동 생성
#define SYNTHESIZE_ADD_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_RELEASE(varName);\
	SAFE_ADD_REF(var);\
	varName = var;\
	}\
}


// 유틸리티 헤더
#include "commonFunction.h"


// 매니저 헤더(싱글톤 버전1 : 템플릿)
#include "cDeviceManager.h"
#include "cSceneManager.h"
#include "cTimeManager.h"
#include "cKeyManager.h"
#include "cMaterialManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "UIObjectManager.h"

// 매니저 헤더(싱글톤 버전2 : 매크로)
#include "cSkinnedMeshManager.h"

// 전역 싱글톤 클래스(디파인)
#include "cObjectPool.h"

// 일반 클래스 헤더(싱글톤 버전1 : 템플릿)
#include "cRandomFunction.h"
#include "cAction.h"
#include "cGameObject.h"

// 1. Position 정점의 좌표 x,y,z(float)			: D3DFVF_XYZ
// 2. RHW (float)                               : D3DFVF_XYZRHW (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 3. Blending Weight Data 결합 가중치 (float)	: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 4. Vertex Normal 정점의 법선 벡터 x,y,z(float)	: D3DFVF_NORMAL
// 5. Vertex Point Size 정점의 점 크기 (float)	: D3DFVF_PSIZE
// 6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE
// 7. Specular Color (DWORD)                    : D3DFVF_SPECULAR
// 8. Texture Coordinate Set 1 (float)          : D3DFVF_TEX0 - D3DFVF_TEX8

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR c;

	ST_PC_VERTEX() : p(0, 0, 0), c(D3DCOLOR_XRGB(0, 0, 0)) {}
	ST_PC_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	ST_PT_VERTEX() :p(0, 0, 0), t(0, 0) {}
	ST_PT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR2 _t) :p(_p), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	ST_PNT_VERTEX() :p(0, 0, 0), n(0, 0, 0), t(0, 0) {}
	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) :p(_p), n(_n), t(_t) {}
	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR2 _t, D3DXVECTOR3 _n = D3DXVECTOR3(0, 1, 0)) :p(_p), n(_n), t(_t) {} //노말 법선 자동 생성

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;


	ST_PN_VERTEX() :p(0, 0, 0), n(0, 0, 0) {}
	ST_PN_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n = D3DXVECTOR3(0, 0, 0)) :p(_p), n(_n) {}


	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_SPHERE
{
	D3DXVECTOR3		p;
	float			r;
	bool			isPicked;
	ST_SPHERE() {}
	ST_SPHERE(D3DXVECTOR3 _p, float _r, bool _i = false) : p(_p), r(_r), isPicked(_i) {}
};

struct ST_SIZE
{
	float			fWidth;
	float			fHeight;

	ST_SIZE() : fWidth(0.0f), fHeight(0.0f) {}
	ST_SIZE(float _w, float _h) : fWidth(_w), fHeight(_h) {}
};

struct ST_PN_CUBE
{
	vector<ST_PN_VERTEX>	m_vecVertex;

	ST_PN_CUBE(float fCubeSize = 1.0f)
	{
		vector<D3DXVECTOR3>	vecVertex;
		vector<DWORD>		vecIndex;
		vecVertex.push_back(D3DXVECTOR3(-fCubeSize, -fCubeSize, -fCubeSize));
		vecVertex.push_back(D3DXVECTOR3(-fCubeSize,  fCubeSize, -fCubeSize));
		vecVertex.push_back(D3DXVECTOR3( fCubeSize,  fCubeSize, -fCubeSize));
		vecVertex.push_back(D3DXVECTOR3( fCubeSize, -fCubeSize, -fCubeSize));
		vecVertex.push_back(D3DXVECTOR3(-fCubeSize, -fCubeSize,  fCubeSize));
		vecVertex.push_back(D3DXVECTOR3(-fCubeSize,  fCubeSize,  fCubeSize));
		vecVertex.push_back(D3DXVECTOR3( fCubeSize,  fCubeSize,  fCubeSize));
		vecVertex.push_back(D3DXVECTOR3( fCubeSize, -fCubeSize,  fCubeSize));

		vector<D3DXVECTOR3> vecNormal;
		vecNormal.push_back(D3DXVECTOR3(0, 0, -1));
		vecNormal.push_back(D3DXVECTOR3(0, 0, 1));
		vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
		vecNormal.push_back(D3DXVECTOR3(1, 0, 0));
		vecNormal.push_back(D3DXVECTOR3(0, 1, 0));
		vecNormal.push_back(D3DXVECTOR3(0, -1, 0));

		//앞
		vecIndex.push_back(0);
		vecIndex.push_back(1);
		vecIndex.push_back(2);
		vecIndex.push_back(0);
		vecIndex.push_back(2);
		vecIndex.push_back(3);
		//뒤
		vecIndex.push_back(4);
		vecIndex.push_back(6);
		vecIndex.push_back(5);
		vecIndex.push_back(4);
		vecIndex.push_back(7);
		vecIndex.push_back(6);
		//좌
		vecIndex.push_back(4);
		vecIndex.push_back(5);
		vecIndex.push_back(1);
		vecIndex.push_back(4);
		vecIndex.push_back(1);
		vecIndex.push_back(0);
		//우
		vecIndex.push_back(3);
		vecIndex.push_back(2);
		vecIndex.push_back(6);
		vecIndex.push_back(3);
		vecIndex.push_back(6);
		vecIndex.push_back(7);
		//상
		vecIndex.push_back(1);
		vecIndex.push_back(5);
		vecIndex.push_back(6);
		vecIndex.push_back(1);
		vecIndex.push_back(6);
		vecIndex.push_back(2);
		//하
		vecIndex.push_back(4);
		vecIndex.push_back(0);
		vecIndex.push_back(3);
		vecIndex.push_back(4);
		vecIndex.push_back(3);
		vecIndex.push_back(7);

		for (size_t i = 0; i < vecIndex.size(); ++i)
		{
			D3DXVECTOR3 p = vecVertex[vecIndex[i]];
			D3DXVECTOR3 n = vecNormal[i / 6];
			m_vecVertex.push_back(ST_PN_VERTEX(p, n));
		}
	}
};

/////////////////////////////// 
//////김태영 전용 구조체///////
//enum Nodetype { WALL, PASS };
//struct Node
//{
//	D3DXVECTOR3		st_position;
//	Nodetype		st_type;
//	Node*			st_parent;
//	vector<Node*>	st_neighbor;
//	float			m_vecH;
//	float			m_vecG;
//	float			m_vecF;
//	bool			m_vecClose;
//	int				m_iIndex;
//};

///////////////////////////////

#include "cUIObject.h"

//콜백 타입 정의

typedef void(*CALLBACK_FUNCTION)(void);
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);
typedef std::function<void()> CallbackBindFunction; // 이것은 함수포인터 방식에서의 typedef void(*pFunc)(void); 에 해당함.

