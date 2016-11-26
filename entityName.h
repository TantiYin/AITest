#pragma once

#include <string>

enum
{
	ent_Miner_Bob,

	ent_Elsa,
	ent_Bus,
};

inline std::string GetNameOfEntity(int n)
{
	switch (n)
	{
	case ent_Miner_Bob:

		return "Miner Bob";

	case ent_Elsa:

		return "Elsa";
	case ent_Bus:
		return "bus";
	default:

		return "UNKNOWN!";
	}
}


