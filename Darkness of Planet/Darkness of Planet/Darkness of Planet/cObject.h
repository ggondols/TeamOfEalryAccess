#pragma once

// ������ : 2017/09/03
// �ۼ��� : ������
// ���� ������ : 2017/09/03
// ���� ������ : ������

// ��� : ���� ���� ���� ���Ǵ� Ŭ�������� ���۷��� ī���͸� �����ϴ� ����� �������

class cObject
{
private:
	int m_nRefCount;
public:
	cObject();
	virtual ~cObject();

	void AddRef(); //����� ������Ʈ�� ���۷��� ī���� ���
	void Release(); //�ٻ���� ������Ʈ�� ���۷��� ī���� ����
	cObject* AutoRelase();
};

