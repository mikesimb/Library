#pragma  once
#include <stdafx.h>
#include "ZQCommon.h"


std::string ExtractFilePath(std::string FullPathFileName)
{
	std::string Path = "";
	int Pos = FullPathFileName.rfind("\\");
	if (Pos >= 0)
	{

		Path.append(FullPathFileName.c_str(), Pos+1);
	}

	return Path;
}

bool FileExists(const char * strFilename)
{
	FILE * fl;
	fopen_s(&fl,strFilename, "r");
	if (fl == NULL)
	{
		return false;
	}
	fclose(fl);
	return true;
}
