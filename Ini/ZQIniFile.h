 #pragma once

#include <list>

class CZQIniFile
{
public:
	CZQIniFile(const char* szFileName);
	int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
	bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
	char* ReadString(char* szSection, char* szKey, const char* szDefaultValue);
	void WriteInteger(char* szSection, char* szKey, int iValue);
	void WriteFloat(char* szSection, char* szKey, float fltValue);
	void WriteBoolean(char* szSection, char* szKey, bool bolValue);
	void WriteString(char* szSection, char* szKey, char* szValue);

	void ReadSection();

	int getSectionCount();
	std::list<std::string> m_Sectionlist;
private:
private:
	char m_szFileName[255];
	
};
