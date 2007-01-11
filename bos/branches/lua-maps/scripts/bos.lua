--       _________ __                 __                               
--      /   _____//  |_____________ _/  |______     ____  __ __  ______
--      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
--      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ \ 
--     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
--             \/                  \/          \//_____/            \/ 
--  ______________________                           ______________________
--			  T H E   W A R   B E G I N S
--	   Stratagus - A free fantasy real time strategy game engine
--
--	bos.lua		-	game specific stuff, and wc2 format compatibility
--
--	(c) Copyright 2001-2003 by Crestez Leonard
--
--      This program is free software; you can redistribute it and/or modify
--      it under the terms of the GNU General Public License as published by
--      the Free Software Foundation; either version 2 of the License, or
--      (at your option) any later version.
--  
--      This program is distributed in the hope that it will be useful,
--      but WITHOUT ANY WARRANTY; without even the implied warranty of
--      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--      GNU General Public License for more details.
--  
--      You should have received a copy of the GNU General Public License
--      along with this program; if not, write to the Free Software
--      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
--
--	$Id$

DefineRaceNames(
	"race", {
		"race", 0,
		"name", "elites",
		"display", "Elites",
		"visible"},
	"race", {
		"race", 1,
		"name", "neutral",
		"display", "Neutral"}
)

SetColorWaterCycleStart(38)
SetColorWaterCycleEnd(47)
SetColorIconCycleStart(240)
SetColorIconCycleEnd(244)
SetColorBuildingCycleStart(205)
SetColorBuildingCycleEnd(207)
