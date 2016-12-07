#pragma once

#include <string>
#include <sstream>
#include "entityManager.h"

enum
{
	ent_Miner_Bob,

	ent_Elsa,
	ent_Bus,
};

inline std::string GetNameOfEntity(int n)
{
	BaseGameEntity* Entity = EntityManager::Instance()->GetEntityFromID(n);
	int EntityType = Entity->GetEntityType();
	std::stringstream ss;
	std::string str;
	switch (EntityType)
	{
	case ent_Miner_Bob:
		ss << "Miner Bob " << n;
		break;
	case ent_Elsa:
		ss << "Elsa " << n;
		break;

	case ent_Bus:
		ss << "Bus" << n;
		break;

	default:
		ss << "Unknown" << n;
		break;

	}
	ss >> str;
	return str;
}


