////////////////////////////////////////////////////////////////////
//游戏对象管理类
//存储着id到对象的映射
////////////////////////////////////////////////////////////////////

#pragma once


#include "npc.h"
#include <map>

class EntityManager
{
private:
	typedef std::map<int, BaseGameEntity*> EntityMap;
private:
	EntityMap mEntityMap;

	EntityManager(){}
	EntityManager(const EntityManager&);
	EntityManager& operator= (const EntityManager&);
public:
	static EntityManager* Instance();
	void RegisterEntity(BaseGameEntity* newEntity);
	BaseGameEntity* GetEntityFromID(int id) const;
	void RemoveEntity(BaseGameEntity* pEntity);
};

