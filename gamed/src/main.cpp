/*
IntWars playground server for League of Legends protocol testing
Copyright (C) 2012  Intline9 <Intline9@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "Game.h"
#include "RAFManager.h"
#include "Inibin.h"

#define SERVER_HOST ENET_HOST_ANY 
#define SERVER_PORT 5119
#define SERVER_KEY "17BLOhi6KZsTtldTsizvHg=="

#define SERVER_VERSION "0.1.0"

int main(int argc, char ** argv) 
{
   Logging->writeLine("IntWars %s\n",SERVER_VERSION);
   //printf("Yorick %s\n", SERVER_VERSION);
   Logging->writeLine("Loading RAF files in filearchives/ ..\n");
   
   std::string basePath = RAFManager::getInstance()->findGameBasePath();
   if(!RAFManager::getInstance()->init(basePath + "filearchives")) {
      Logging->errorLine("Couldn't load RAF files. Make sure you have League of Legends installed correctly or 'filearchives' directory in the server's root directory\n");
      Logging->errorLine("This directory is to be taken from RADS/projects/lol_game_client/\n");
      return EXIT_FAILURE;
   }
   


	Game g;
	ENetAddress address;
	address.host = SERVER_HOST;
	address.port = SERVER_PORT;
   
   

	if(!g.initialize(&address, SERVER_KEY))
	{
		Logging->writeLine("Failed to initialize game!");
		return -1;
	}

	g.netLoop();
   

	
   return EXIT_SUCCESS;
}