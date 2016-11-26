#include "headers.h"

#include <windows.h>
#include <iostream>
//#include "lua.hpp"

using namespace std;

//int Add(int a, int b)
//{
//	return a + b;
//}
//
//int cpp_Add(lua_State* L)
//{
//	int n = lua_gettop(L);
//	if (n != 2)
//	{
//		cout << "args not match" << endl;
//		return 0;
//	}
//	//ÅÐ¶ÏÀàÐÍ
//
//	int a = lua_tonumber(L, 1);
//	int b = lua_tonumber(L, 2);
//
//	int result = Add(a, b);
//	lua_pushnumber(L, result);
//	return 1;
//}

int main()
{
	//Miner *bob = new Miner(ent_Miner_Bob);
	//EntityManager::Instance()->RegisterEntity(bob);
	//Elsa *elsa = new Elsa(ent_Elsa);
	//EntityManager::Instance()->RegisterEntity(elsa);
	Vehicle *bus = new Vehicle(ent_Bus, Vector2(0, 0), 2, Vector2(0, 10), Vector2(0, 1), 10, 20, 30);
	
	cout << "target: 10,100" << endl;
	while (true)
	{
		bus->Update(1);
		cout << GetNameOfEntity(bus->GetID()) << "pos: " << bus->Pos().x << "," << bus->Pos().y << endl;
		//bob->Update();
		//cout << " Energy: " << bob->GetEnergy() << " Water: " << bob->GetWater() << " Gold: " << bob->GetGold();

		//elsa->Update();

		//dispatch any delayed messages
		//MessageDispatcher::Instance()->DispatchDelayedMessage();
		Sleep(1000);
	}
	/*lua_State * pL = luaL_newstate();

	lua_register(pL, "cpp_Add", cpp_Add);

	int ret = luaL_dofile(pL, "test.lua");
	if (ret )
	{
		cout << "load file failed" << endl;
		return 0;
	}


	int age = 0;
	string name = "";

	lua_settop(pL, 0);
	lua_getglobal(pL, "age");
	lua_getglobal(pL, "name");

	name = lua_tostring(pL, 2);
	age = (int)lua_tonumber(pL, 1);

	cout << "name: " << name << " age:" << age << endl;

	lua_pop(pL, 2);

	lua_getglobal(pL, "sample_table");
	if (!lua_istable(pL,1))
	{
		cout << "not table" << endl;
		
	}
	else
	{

		lua_getfield(pL, 1, "age");
		lua_getfield(pL, 1, "name");
		int age = 0;
		string name = "";
		if (lua_isnumber(pL, -2))
		{
			age = (int)lua_tonumber(pL, -2);
		}
		if (lua_isstring(pL, -1))
		{
			name = lua_tostring(pL, -1);
		}

		cout << "name: " << name << " age:" << age << endl; 

		lua_pop(pL, 2);

	}
	lua_pop(pL, 1);

	lua_getglobal(pL, "add");

	lua_pushnumber(pL, 5);
	lua_pushnumber(pL, 8);

	lua_call(pL, 2, 1);

	if (lua_isnumber(pL,1))
	{
		int result = lua_tonumber(pL, 1);
		cout << "result: " << result << endl;
	}
	lua_pop(pL, 1);

	lua_close(pL);*/

	system("pause");
	return 0;
}