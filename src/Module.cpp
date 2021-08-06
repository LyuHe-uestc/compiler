#include"../include/Module.h"
#include"../include/node.h"
#include <vector>

using std::vector;
using namespace std;

void Module::ASTTranslate(NCompUnit* cu)
{
	vector<NDecl*> decl=cu->declarations;
	for (int i = 0; i < decl.size(); i++)
	{
		NVarDecl* p = dynamic_cast<NVarDecl*>(decl[i]);
		if (p == NULL)//Func decl
		{
			NFuncDecl* p = dynamic_cast<NFuncDecl*>(decl[i]);
			string name = p->function_name.name;
			string type = p->type;
			Type* t = new Type();
			if (type == "int")
			{
				t->tName = intType;
			}
			else
			{
				t->tName = voidType;
			}

			vector<Type*>arg;
			vector<std::string> paraName;
			NVarDeclList para = p->parameters;
			for (auto decl : para)
			{
				paraName.push_back(decl->identifier.name);
				if (decl->size == 0)
				{
					Type* paraType = new IntType();
					arg.push_back(paraType);
				}
				else
				{
					Type* paraType = new ArrayType(decl->size);
					arg.push_back(paraType);
				}
			}
			Function* func = Function::makeFunction(t, arg,paraName);
			func->setName(name);
			func->setParent(this);
			NStmtList stmt = p->statements;
			func->getFromStatment(stmt);

			addFunction(func);
		}

		else //var decl
		{
			int len = p->lengths.size();
			string name = p->identifier.name;
			if (len == 0) //int
			{
				Type* type = new Type();
				type->tName == intType;
				Value* val = new Value(type);
				
				addAddress(val, address);
				address += 4;
				addGlobalVar(name,val);
				if (p->init == true) //if init,add value to map
				{
					int intVal = p->initvalue->GetValue();
					ConstantInt* constInt = new ConstantInt(intVal);
					addConstantValue(val, constInt);
				}
			}

			else //int array
			{
				Type* type = new ArrayType(p->lengths.size());
				Value* val = new Value(type);

				addAddress(val, address);
				address += 4 * ((p->lengths[0])->GetValue());
				addGlobalVar(name,val);

				if (p->init == true)
				{
					vector<int> arrayValue;
					for (int i = 0; i < p->finalInitValue->size(); i++)
					{
						int intVal = (*(p->finalInitValue))[i];
						arrayValue.push_back(intVal);
					}
					ConstantArray* array = new ConstantArray(arrayValue);
					addConstantValue(val, array);
				}
			}
		}
	}
}

void Module::debugPrint()
{
	map<string, Value*>::iterator iter;
	for ( iter = globalVar.begin(); iter != globalVar.end(); iter++)
	{
		cout << iter->first << endl;
		Value* val = getGlobalValue(iter->first);
		/*
		cout << val->isArray() << " ";
		cout << val->isInt() << " ";
		cout << val->isConstant << endl;
		*/
		/*
		cout << iter->second->isArray() << " ";
		cout << iter->second->isInt() << " ";
		cout << iter->second->isConstant << endl;
		*/
		if (val->isArray())
		{
			cout << "array: " << iter->first;
			if (val->isConstant)
			{
				vector<int> value;
				value = ((ConstantArray*)getConstantValue(val))->value;
				for (int i = 0; i < value.size(); i++)
				{
					cout << value[i] << " ";
				}
			}
			cout << endl;
		}
		else if (val->isInt())
		{
			cout << "int: " << iter->first;
			if (val->isConstant)
			{
				int value;
				value = ((ConstantInt*)getConstantValue(val))->value;
				cout << value;
			}
			cout << endl;
		}
	}
	for (auto i : funcList)
	{
		i->debugPrint();
	}
};


Function* Module::getFunction(string funcName)
{
	for (auto i : funcList)
	{
		if (i->name == funcName)
			return i;
	}
	return nullptr;
}