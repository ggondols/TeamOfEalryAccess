#pragma once

// 생성일 : 2017/09/03
// 작성자 : 여현구
// 최종 수정일 : 2017/09/03
// 최종 수정자 : 여현구

// 기능 : 여러 군데 에서 사용되는 클래스들의 레퍼런스 카운터를 저장하는 기능을 상속해줌

class cObject
{
private:
	int m_nRefCount;
public:
	cObject();
	virtual ~cObject();

	void AddRef(); //사용할 오브젝트의 레퍼런스 카운터 상승
	void Release(); //다사용한 오브젝트의 레퍼런스 카운터 감소
	cObject* AutoRelase();
};

