#! /usr/bin/env python
#     ____                _       __               
#    / __ )____  _____   | |     / /___ ___________
#   / __  / __ \/ ___/   | | /| / / __ `/ ___/ ___/
#  / /_/ / /_/ (__  )    | |/ |/ / /_/ / /  (__  ) 
# /_____/\____/____/     |__/|__/\__,_/_/  /____/  
#                                              
#       A futuristic real-time strategy game.
#          This file is part of Bos Wars.
#
#      Script that generates all the distribution packages.
#      (c) Copyright 2007 by Francois Beerten
#
#      Bos Wars is free software; you can redistribute it and/or modify
#      it under the terms of the GNU General Public License as published
#      by the Free Software Foundation; only version 2 of the License.
#
#      Bos Wars is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#      GNU General Public License for more details.
import os
import sys
import csv

importantkeys = ['Name', 'ProductionCosts', 'MaxUtilizationRate', 
    'ProductionRate', 'HitPoints', 'SightRange', 'Armor', 'BasicDamage', 
    'PiercingDamage', 'MaxAttackRange']

def findunits():
    u = os.listdir('units')
    u = [x for x in u if not x.startswith('.')]
    return u

def findunitscripts(directory):
    base = 'units/' + directory + '/'
    s = os.listdir(base)
    s = [base + x for x in s if x.endswith('.lua')]
    return s

def findallscripts():
    scripts = []
    units = findunits()
    for u in units:
        scripts.extend(findunitscripts(u))
    return scripts

#print findallscripts()

def parseKvList(kvlist):
    d = {}
    key = kvlist[0]
    for i in kvlist[1]:
        if key:
            d[key] = i
            key = None
        else:
            key = i
    return key

class ParsedScript:
    def __init__(self, path):
        self.path = path
        self.units = []
    def regenerate(self, f):
        f.write(self.head)
        for unit in self.units:
            unit.regenerate(f)
    
class ParsedUnit:
    def __init__(self):
        self.orderedkeys = []
        self.stats = {}
    def regenerateStats(self, f):
        keys = self.stats.keys()
        keys.sort()
        for k in self.orderedkeys[:-1]:
            f.write('    ' + k + ' = ' + self.stats[k] + ',\n')
        k = self.orderedkeys[-1]
        f.write('    ' + k + ' = ' + self.stats[k])
    def regenerate(self, f):
        f.write('DefineUnitType("' + self.internalname + '", {\n')
        self.regenerateStats(f)
        f.write('})')
        f.write(self.rest)
    def writeCsv(self, statsfile):
        statsfile.writerow(self.stats) 
        

def stripComments(s):
    s = s.split('\n')
    new = []
    for i in s:
        i = i.split('--', 1)
        new.append(i[0])
    return '\n'.join(new)

def parseDefineUnitType(text):
    unit = ParsedUnit()
    body, unit.rest = text.split('})', 1)
    body = stripComments(body)
    internalname, stats = body.split(', {', 1)
    unit.internalname = internalname.strip().strip('"')
    stats = stats.split('=')
    prevKey = stats[0].strip()
    for x in stats[1:-1]:
        x = x.split(',')
        unit.stats[prevKey] = ','.join(x[:-1]).strip()
        unit.orderedkeys.append(prevKey)
        prevKey = x[-1].strip()
    if len(stats) > 2:
        unit.orderedkeys.append(prevKey)
        unit.stats[prevKey] = stats[-1]
    return unit
    
def parseScript(path):
    parsedscript = ParsedScript(path)
    s = file(path, 'rt').read()
    elements = s.split('DefineUnitType(')
    parsedscript.head = elements[0]
    for e in elements[1:]:
        unit = parseDefineUnitType(e)
        parsedscript.units.append(unit)
    return parsedscript


def parseAllScripts():
    units = []
    scriptpaths = findallscripts()
    scripts = []
    for i in scriptpaths:
        if 'crystal' not in i:
            parsedscript = parseScript(i)
            scripts.append(parsedscript)
            units.extend(parsedscript.units)
    return units, scripts

def generateStatsFile(units):
    rawcsvfile = file('stats.csv', 'w')
    statsfile = csv.DictWriter(rawcsvfile, importantkeys, extrasaction='ignore')
    title = {}
    for i in importantkeys:
        title[i]=i
    statsfile.writerow(title)
    for unit in units:
        unit.writeCsv(statsfile)


def regenerateScripts(scripts):
    for i in scripts:
        f = file(i.path, 'wt')
        i.regenerate(f)

def updateUnitStats(units):
    pass

Usage = """
    Unit stats generation tool.
    Usage: %s <command>

    Command:
        csv
        regenerate
        update
"""
def printUsage(args):
    print Usage % args[0]

def main(args):
    if len(args) == 1:
        printUsage(args)
        return
    units, scripts = parseAllScripts()
    if args[1] == 'csv':
        generateStatsFile(units)
    elif args[1] == 'regenerate':
        regenerateScripts(scripts)
    elif args[1] == 'update':
        updateUnitStats(units)
        regenerateScripts(scripts)
    else:
        printUsage(args)
        return


if __name__ == '__main__':
    main(sys.argv)
    
