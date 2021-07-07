#pragma once

#include<vector>
#include"Value.h"


class User :public Value
{
public:
	Value* getArg(int no);

	void setArg(Value* val, int no);

	void addArg(Value* val);

	void addArg(vector<Value*> args);

	void removeArg(int i);

	//range from i to j
	void removeArg(int i, int j);

	void removeAllArg();

private:
	vector<Value*> args;
	int argNum;
};