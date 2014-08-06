#include "stdafx.h"
#include "RAFManager.h"
#include "tinydir.h"
#include "Log.h"

#ifdef _WIN32
#include<Winreg.h>

#pragma comment(lib, "Advapi32.lib")
#endif

using namespace std;

RAFManager* RAFManager::instance = 0;

bool RAFManager::init(const string& rootDirectory) {
   tinydir_dir dir;

   if(tinydir_open_sorted(&dir, rootDirectory.c_str()) == -1) {
      return false;
   }

   for (int i = 0; i < dir.n_files; i++)
   {
      tinydir_file file;
      tinydir_readfile_n(&dir, &file, i);

      if (!file.is_dir || strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0) {
         continue;
      }
      
      printf("%s\n", file.name);
      
      string rafPath = rootDirectory + '/' + file.name + "/Archive_2.raf";
      RAFFile* raf = new RAFFile(rafPath);
      
      files.push_back(raf);
   }
   
   printf("Loaded %d RAF files\n", files.size());

   tinydir_close(&dir);
   
   return true;
}

bool RAFManager::readFile(const std::string& path, vector<unsigned char>& toFill) {
   for(vector<RAFFile*>::iterator it = files.begin(); it != files.end(); ++it) {
      RAFFile* f = *it;
      if(f->readFile(path, toFill)) {
         return true;
      }
   }
   
   return false;
}

std::string RAFManager::findGameBasePath()
{
	Logging->debugLine("Searching for LoL base path...\n");
#ifdef _WIN32
	HKEY hKey;
	std::vector<string> strKeyPathCU, strKeyPathLM;
	strKeyPathCU.push_back("SOFTWARE\\RIOT GAMES\\RADS");
	strKeyPathCU.push_back("SOFTWARE\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\Wow6432Node\\RIOT GAMES\\RADS");
	strKeyPathCU.push_back("SOFTWARE\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\RIOT GAMES\\RADS");
	strKeyPathCU.push_back("SOFTWARE\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\RIOT GAMES\\RADS");
	strKeyPathCU.push_back("SOFTWARE\\RIOT GAMES\\RADS");

	strKeyPathLM.push_back("Software\\Wow6432Node\\Riot Games\\RADS");
	strKeyPathLM.push_back("SOFTWARE\\RIOT GAMES\\RADS");

	string strKeyName = "LOCALROOTFOLDER";
	DWORD dwValueType;
	TCHAR byteValue[100];
	DWORD dwValueSize;


	//Check CURRENT_USER keys
	for(int i=0; i< strKeyPathCU.size();i++)
	{
		if( RegOpenKeyExA(HKEY_CURRENT_USER, strKeyPathCU[i].c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS )
		{
			continue;
		}

		if( RegQueryValueExA(hKey, strKeyName.c_str(), NULL, &dwValueType, (LPBYTE)byteValue, &dwValueSize) != ERROR_SUCCESS )
		{
			continue;
		}

		string sValue(byteValue);
		sValue += "/projects/lol_game_client/";
		Logging->debugLine("Found base path in %s\n",sValue.c_str());

		return sValue;
	}

	//Check LOCAL_MACHINE keys

	for(int i=0; i< strKeyPathLM.size();i++)
	{
		if( RegOpenKeyExA(HKEY_CURRENT_USER, strKeyPathLM[i].c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS )
		{
			continue;
		}

		if( RegQueryValueExA(hKey, strKeyName.c_str(), NULL, &dwValueType, (LPBYTE)byteValue, &dwValueSize) != ERROR_SUCCESS )
		{
			continue;
		}

		std::string sValue(byteValue);
		sValue += "/projects/lol_game_client/";

		Logging->debugLine("Found base path in %s\n",sValue.c_str());
		return sValue;
	}

	Logging->errorLine("Couldnt find League of Legends game path or unable to read Registry keys\n");
	return "";

#else
	return "";
#endif
}