#pragma once
#include "cSingletonBase.h"

class DataBase : public cSingletonBase<DataBase>
{
private:
	string name;

public:
	DataBase();
	~DataBase();

	void LoadData(void);
};

