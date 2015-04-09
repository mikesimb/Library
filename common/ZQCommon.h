#include <string>
#include <stdio.h>


#define CONFIG_FILE_NAME "LoginGate.ini"

std::string ExtractFilePath(std::string FullPathFileName);
bool FileExists(const char * strFilename);