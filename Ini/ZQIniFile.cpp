#include "stdafx.h"
#include "ZQIniFile.h"
#include <iostream>
#include <Windows.h>
CZQIniFile::CZQIniFile(const char* szFileName)
{
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName));
}
int CZQIniFile::ReadInteger(char* szSection, char* szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt((LPCSTR)szSection, (LPCSTR)szKey, iDefaultValue, (LPCSTR)m_szFileName);
	return iResult;
}
float CZQIniFile::ReadFloat(char* szSection, char* szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	sprintf_s(szDefault, 255, "%f", fltDefaultValue);
	GetPrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szDefault, (LPSTR)szResult, 255, (LPCSTR)m_szFileName);
	fltResult = atof(szResult);
	return fltResult;
}
bool CZQIniFile::ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	bool bolResult;
	sprintf_s(szDefault, 255, "%s", bolDefaultValue ? "True" : "False");
	GetPrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szDefault, (LPSTR)szResult, 255, (LPCSTR)m_szFileName);
	bolResult = (strcmp(szResult, "True") == 0 || strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}
char* CZQIniFile::ReadString(char* szSection, char* szKey, const char* szDefaultValue)
{
	char* szResult = new char[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szDefaultValue, (LPSTR)szResult, 255, (LPCSTR)m_szFileName);
	return szResult;
}

void CZQIniFile::WriteInteger(char* szSection, char* szKey, int iValue)
{
	char szValue[255];
	sprintf_s(szValue, 255,"%d", iValue);
	WritePrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szValue, (LPCSTR)m_szFileName);
}
void CZQIniFile::WriteFloat(char* szSection, char* szKey, float fltValue)
{
	char szValue[255];
	sprintf_s(szValue,255, "%f", fltValue);
	WritePrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szValue, (LPCSTR)m_szFileName);
}
void CZQIniFile::WriteBoolean(char* szSection, char* szKey, bool bolValue)
{
	char szValue[255];
	sprintf_s(szValue,255, "%s", bolValue ? "True" : "False");
	WritePrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szValue, (LPCSTR)m_szFileName);
}
void CZQIniFile::WriteString(char* szSection, char* szKey, char* szValue)
{
	WritePrivateProfileString((LPCSTR)szSection, (LPCSTR)szKey, (LPCSTR)szValue, (LPCSTR)m_szFileName);
}

void CZQIniFile::ReadSection()
{
	//GetPrivateProfileSectionNames
	char SectionNames[MAX_PATH], *pSectionName;
	ZeroMemory(SectionNames, MAX_PATH);
	GetPrivateProfileSectionNames(SectionNames, MAX_PATH,m_szFileName);
	m_Sectionlist.clear();
	m_Sectionlist.push_back(SectionNames);   // 得到第一个Section
	for (int i = 0; i < MAX_PATH - 2; i++)     //遍历第二个开始的Section
	{
		if (SectionNames[0] == '\0')
			break;
		if (SectionNames[i] == '\0')
		{
			if (SectionNames[i + 1] == '\0')    // 连续出现两个 '\0' 表示后面没有了
				break;

			pSectionName = (char*)&SectionNames + i + 1;             // C语言指针一定要学好，呵呵
		std::string temp = pSectionName;                                               //先放CString，因为'\0'是结束符，安全得到Section
		m_Sectionlist.push_back(temp);
		}
	}
}

int CZQIniFile::getSectionCount()
{
	return  m_Sectionlist.size();
}
