#pragma once

#include <fstream>
#include <string>

class iniFileLoader
{
	/* private begin */
private:
	std::ifstream file;

	std::string CurrentLine;

	//this is set to true if the file specified by the user is valid
	bool        m_bGoodFile;

	void        GetParameterValueAsString(std::string& line);

	std::string GetNextParameter();

	/* private end*/

	/* public begin */
public:
	//helper methods. They convert the next parameter value found into the 
	//relevant type
	double      GetNextParameterDouble() { if (m_bGoodFile) return atof(GetNextParameter().c_str()); throw std::runtime_error("bad file"); }
	float       GetNextParameterFloat() { if (m_bGoodFile) return (float)atof(GetNextParameter().c_str()); throw std::runtime_error("bad file"); }
	int         GetNextParameterInt() { if (m_bGoodFile) return atoi(GetNextParameter().c_str()); throw std::runtime_error("bad file"); }
	bool        GetNextParameterBool() { return (bool)(atoi(GetNextParameter().c_str())); throw std::runtime_error("bad file"); }

	bool        eof()const { if (m_bGoodFile) return file.eof(); throw std::runtime_error("bad file"); }
	bool        FileIsGood()const { return m_bGoodFile; }

	iniFileLoader(char* filename) :CurrentLine(""), m_bGoodFile(true)
	{
		file.open(filename);

		if (!file) { m_bGoodFile = false; }
	}
	/* public end */
};
