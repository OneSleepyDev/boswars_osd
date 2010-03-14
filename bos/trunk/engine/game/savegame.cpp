//     ____                _       __               
//    / __ )____  _____   | |     / /___ ___________
//   / __  / __ \/ ___/   | | /| / / __ `/ ___/ ___/
//  / /_/ / /_/ (__  )    | |/ |/ / /_/ / /  (__  ) 
// /_____/\____/____/     |__/|__/\__,_/_/  /____/  
//                                              
//       A futuristic real-time strategy game.
//          This file is part of Bos Wars.
//
/**@name savegame.cpp - Save game. */
//
//      (c) Copyright 2001-2007 by Lutz Sammer, Andreas Arens
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; only version 2 of the License.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//      02111-1307, USA.
//

//@{

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string>

#include "stratagus.h"
#include "ui.h"
#include "unit.h"
#include "upgrade.h"
#include "missile.h"
#include "map.h"
#include "player.h"
#include "ai.h"
#include "trigger.h"
#include "iolib.h"
#include "replay.h"
#include "script.h"
#include "actions.h"
#include "version.h"

/*----------------------------------------------------------------------------
--  Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
** Get the save directory
*/
static std::string GetSaveDir()
{
	std::string dir;

	dir = UserDirectory + "save/";

	return dir;
}

/**
**  Save a game to file.
**
**  @param filename  File name to be stored.
**
**  @note  Later we want to store in a more compact binary format.
*/
void SaveGame(const std::string &filename)
{
	time_t now;
	CFile file;
	char *s;
	char *s1;
	std::string fullpath;

	fullpath = GetSaveDir() + filename;
	if (file.open(fullpath.c_str(), CL_WRITE_GZ | CL_OPEN_WRITE) == -1) {
		fprintf(stderr, "Can't save to `%s'\n", filename.c_str());
		return;
	}

	time(&now);
	s = ctime(&now);
	if ((s1 = strchr(s, '\n'))) {
		*s1 = '\0';
	}

	//
	// Parseable header
	//
	file.printf("SavedGameInfo({\n");
	file.printf("---  \"comment\", \"Generated by Bos Wars Version " VERSION "\",\n");
	file.printf("---  \"comment\", \"Visit http://www.boswars.org for more informations\",\n");
	file.printf("---  \"type\",    \"%s\",\n", "single-player");
	file.printf("---  \"date\",    \"%s\",\n", s);
	file.printf("---  \"map\",     \"%s\",\n", Map.Info.Description.c_str());
	file.printf("---  \"media-version\", \"%s\",\n", "Undefined");
	file.printf("---  \"engine\",  {%d, %d, %d},\n",
		StratagusMajorVersion, StratagusMinorVersion, StratagusPatchLevel);
	file.printf("  SyncHash = %d, \n", SyncHash);
	file.printf("  SyncRandSeed = %d, \n", SyncRandSeed);
	file.printf("  SaveFile = \"%s\"\n", CurrentMapPath);
	file.printf("} )\n\n");

	// FIXME: probably not the right place for this
	file.printf("GameCycle = %lu\n", GameCycle);

	SaveCcl(&file);
	SaveUpgrades(&file);
	SavePlayers(&file);
	Map.Save(&file);
	SaveUnits(&file);
	SaveUserInterface(&file);
	SaveAi(&file);
	SaveSelections(&file);
	SaveGroups(&file);
	SaveMissiles(&file);
	SaveTriggers(&file);
	SaveReplayList(&file);
	// FIXME: find all state information which must be saved.
	s = SaveGlobal(Lua, true);
	if (s != NULL) {
		file.printf("-- Lua state\n\n %s\n", s);
		delete[] s;
	}
	file.close();
}

//@}
