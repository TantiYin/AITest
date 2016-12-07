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

inline std::wstring GetNameOfEntity(int n)
{
	BaseGameEntity* Entity = EntityManager::Instance()->GetEntityFromID(n);
	int EntityType = Entity->GetEntityType();
	std::wstringstream ss;
	std::wstring str;
	switch (EntityType)
	{
	case ent_Miner_Bob:
		ss << L"Miner Bob " << n;
		break;
	case ent_Elsa:
		ss << L"Elsa " << n;
		break;

	case ent_Bus:
		ss << L"Bus" << n;
		break;

	default:
		ss << L"Unknown" << n;
		break;

	}
	ss >> str;
	return str;
}


