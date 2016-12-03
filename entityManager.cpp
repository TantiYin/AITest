#include "entityManager.h"
#include "baseEntity.h"

EntityManager* EntityManager::Instance()
{
	static EntityManager* ins = new EntityManager();
	return ins;
}

void EntityManager::RegisterEntity(BaseGameEntity* newEntity)
{
	mEntityMap.insert(std::make_pair(newEntity->GetID(), newEntity));
}

BaseGameEntity* EntityManager::GetEntityFromID(int id) const
{
	EntityMap::const_iterator iter = mEntityMap.find(id);
	if (iter != mEntityMap.end())
	{
		return iter->second;
	}
	return NULL;
}

void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{
	mEntityMap.erase(mEntityMap.find(pEntity->GetID()));
}