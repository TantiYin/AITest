#include "iniFileLoader.h"

using std::string;

//removes any commenting from a line of text
void RemoveCommentingFromLine(string& line)
{
	//search for any comment and remove
	string::size_type idx = line.find('//');

	if (idx != string::npos)
	{
		//cut out the comment
		line = line.substr(0, idx);
	}
}

void iniFileLoader::GetParameterValueAsString(std::string& line)
{
	//find beginning of parameter description
	string::size_type begIdx;
	string::size_type endIdx;

	//define some delimiters
	const string delims(" \;=,");

	begIdx = line.find_first_not_of(delims);

	//find the end of the parameter description
	if (begIdx != string::npos)
	{
		endIdx = line.find_first_of(delims, begIdx);

		//end of word is the end of the line
		if (endIdx == string::npos)
		{
			endIdx = line.length();
		}
	}

	//find the beginning of the parameter value
	begIdx = line.find_first_not_of(delims, endIdx);
	//find the end of the parameter value
	if (begIdx != string::npos)
	{
		endIdx = line.find_first_of(delims, begIdx);

		//end of word is the end of the line
		if (endIdx == string::npos)
		{
			endIdx = line.length();
		}
	}

	line = line.substr(begIdx, endIdx);
}

std::string iniFileLoader::GetNextParameter()
{
	//this will be the string that holds the bext parameter
	std::string line;

	std::getline(file, line);

	RemoveCommentingFromLine(line);

	//if the line is of zero length, get the next line from
	//the file
	if (line.length() == 0)
	{
		return GetNextParameter();
	}

	GetParameterValueAsString(line);

	return line;
}
