////////////////////////////////////////////////////////////////////
//��Ϸ���������
//�洢��id�������ӳ��
////////////////////////////////////////////////////////////////////

#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

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
#endif