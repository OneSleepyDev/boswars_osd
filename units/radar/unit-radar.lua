--            ____            
--           / __ )____  _____
--          / __  / __ \/ ___/
--         / /_/ / /_/ (__  ) 
--        /_____/\____/____/  
--
--  Invasion - Battle of Survival                  
--   A GPL'd futuristic RTS game
--
--	radar.lua	-	Define the radar unit.
--
--	(c) Copyright 2004-2005 by Fran�ois Beerten.
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
--      Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
--
--	$Id$

DefineAnimations("animations-radar", {
    Still = {"frame 0", "wait 4", "frame 1", "wait 4", "frame 2", "wait 4",
        "frame 3", "wait 4", "frame 4", "wait 4", "frame 5", "wait 4", 
        "frame 6", "wait 4", "frame 7", "wait 4", },
    Death = {"unbreakable begin", "frame 0", "wait 1", "frame 1", "wait 10",
        "frame 1", "unbreakable end", "wait 10", },
    })

DefineIcon({
	Name = "icon-radar",
	Size = {46, 38},
	Frame = 0,
	File = "units/radar/radar_i.png"})

DefineIcon({
	Name = "icon-radar_b",
	Size = {46, 38},
	Frame = 0,
	File = "units/radar/radar_b.png"})

MakeSound("radar-selected", GetCurrentLuaPath().."/sfx_rdar.select.wav")

DefineConstruction("construction-radar", {
	Constructions = {
		{Percent = 0, File = "main", Frame = 11},
		{Percent = 50, File = "main", Frame = 10},
	}
})

DefineUnitType("unit-radar", {
	Name = "Radar",
	Image = {"file", "units/radar/radar.png", "size", {64, 64}},
	Offset = {12, -16},
	Shadow = {"file", "units/radar/radar_s.png", "size", {64, 64}},
	Animations = "animations-radar", Icon = "icon-radar",
	Costs = {"time", 20, "titanium", 7, "crystal", 35},
	RepairHp = 1, RepairCosts = {"crystal", 3}, Construction = "construction-radar",
	Speed = 0, HitPoints = 5, DrawLevel = 25, TileSize  = {1, 1}, BoxSize = {32, 28},
	SightRange = 3, Armor = 0 , BasicDamage = 0, PiercingDamage = 0,
	Missile = "missile-none", Priority = 20, AnnoyComputerFactor = 65,
	Points = 10, Supply = 0, ExplodeWhenKilled = "missile-64x64-explosion",
	Corpse = {"radar_destroyed", 0}, Type = "land",
	Building = true, BuilderOutside = true,
	VisibleUnderFog = true,
	NumDirections = 1,
	RadarRange = 40,
	Sounds = {"selected", "radar-selected"}
})

DefineUnitType("radar_destroyed", {
	Name = "RadarWreck",
	Image = {"file", "units/radar/radar.png", "size", {64, 64}},
	Animations = "animations-radar", Icon = "icon-cancel",
	Speed = 0, HitPoints = 999, DrawLevel = 10,
	TileSize = {1, 1}, BoxSize = {28, 28}, SightRange = 1,
	BasicDamage = 0, PiercingDamage = 0, Missile = "missile-none",
	Priority = 0, Type = "land", Building = true, Vanishes = true
})

DefineAllow("unit-radar", "AAAAAAAAAAAAAAAA")

DefineButton({
	Pos = 4, Level = 1, Icon = "icon-radar_b", Action = "build",
	Value = "unit-radar", Key = "r", Hint = "BUILD ~!RADAR",
	ForUnit = {"unit-engineer"}})



