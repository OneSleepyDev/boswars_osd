##     ____                _       __               
##    / __ )____  _____   | |     / /___ ___________
##   / __  / __ \/ ___/   | | /| / / __ `/ ___/ ___/
##  / /_/ / /_/ (__  )    | |/ |/ / /_/ / /  (__  ) 
## /_____/\____/____/     |__/|__/\__,_/_/  /____/  
##                                              
##       A futuristic real-time strategy game.
##          This file is part of Bos Wars.
##
##      SConstruct build file. See http://www.scons.org for info about scons.
##      (c) Copyright 2005-2007 by Francois Beerten
##
##      Stratagus is free software; you can redistribute it and/or modify
##      it under the terms of the GNU General Public License as published
##      by the Free Software Foundation; only version 2 of the License.
##
##      Stratagus is distributed in the hope that it will be useful,
##      but WITHOUT ANY WARRANTY; without even the implied warranty of
##      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##      GNU General Public License for more details.
##
##      $Id$
##

import os
import sys
import glob
from stat import *
import filecmp

ccflags = "-fsigned-char"
SConsignFile()

def DefineOptions(filename, args):
   opts = Options(filename, args)
   opts.Add('CPPPATH', 'Additional preprocessor paths')
   opts.Add('CPPFLAGS', 'Additional preprocessor flags')
   opts.Add('CPPDEFINES', 'defined constants', Split(''))
   opts.Add('LIBPATH', 'Additional library paths')
   opts.Add('LIBS', 'Additional libraries')
   opts.Add('CCFLAGS', 'C Compiler flags', Split(ccflags))
   opts.Add('LINKFLAGS', 'Linker Compiler flags')
   opts.Add('CC', 'C Compiler')
   opts.Add('CXX', 'C++ Compiler')
   opts.Add('opengl', 'Build with opengl support', 0)
   opts.Add('extrapath', 'Path to extra root directory for includes and libs', '')
   return opts


opts = DefineOptions("build_options.py", ARGUMENTS)
env = Environment(ENV = {'PATH':os.environ['PATH']}) # for an unknown reason Environment(options=opts) doesnt work well
opts.Update(env) # Needed as Environment(options=opts) doesnt seem to work
Help(opts.GenerateHelpText(env))
mingw = env.Copy()
optionsChanged = True
if os.path.exists('build_options.py'):
   os.rename('build_options.py', 'build_options_OLD_FOR_CHECK.py')
   optionsChanged = False
opts.Save("build_options.py", env)
if not optionsChanged:
   optionsChanged  = not filecmp.cmp('build_options.py', 'build_options_OLD_FOR_CHECK.py', False)
   os.remove('build_options_OLD_FOR_CHECK.py')

engineSourceDir = 'engine'

def globSources(sourceDirs, builddir):
  sources = []
  sourceDirs = Split(sourceDirs)
  for d in sourceDirs:
    sources.append(glob.glob(engineSourceDir + '/' + d + '/*.cpp'))
  sources = Flatten(sources)
  targetsources = []
  for s in sources:
    targetsources.append(builddir + s[len(engineSourceDir):])
  return targetsources

def buildSourcesList(builddir):
   sources = globSources("action ai editor game map network pathfinder sound stratagus ui unit video tolua", builddir)
   sources.append(globSources("guichan guichan/sdl guichan/widgets", builddir))
   return sources
sourcesEngine = buildSourcesList('build')

def CheckOpenGL(env, conf):
  opengl = {}
  opengl['linux'] = { 
      'LIBS': ['GL'], 
      'LIBPATH': ['/usr/lib', '/usr/X11R6/lib'],
      'CPPPATH': ['/usr/include']}
  opengl['cygwin'] = {
      'LIBS': ['opengl3']}
  platform = sys.platform
  if sys.platform[:5] == 'linux':
     platform = 'linux'
  for key in opengl[platform].keys():
      if key != 'LIBS':
         for i in opengl[platform][key]:
            env[key].append(i)
  for lib in opengl[platform]['LIBS']:
     if not conf.CheckLib('GL'):
         print("Can't find OpenGL libs. Exiting")
         sys.exit(1)
  env.Append(CPPDEFINES = 'USE_OPENGL')
  sourcesEngine.append(globSources("guichan/opengl"))

def CheckLuaLib(env, conf):
  if not 'USE_WIN32' in env['CPPDEFINES'] and env.WhereIs('lua-config'):
    env.ParseConfig('lua-config --include --libs')
  found = 0
  if conf.CheckLibWithHeader('lua', 'lua.h', 'c'):
    found = 1
  if not found and conf.CheckLibWithHeader('lua50', 'lua.h', 'c'):
    found =1
  if not found and conf.CheckLibWithHeader('lua5.0', 'lua.h', 'c'):
    found =1
  if not found and conf.CheckLibWithHeader('lua51', 'lua.h', 'c'):
    found =1
  if not found and conf.CheckLibWithHeader('lua5.1', 'lua.h', 'c'):
    found =1
  if not found:
    return 0

  if conf.CheckLibWithHeader('lualib', 'lualib.h', 'c'):
     return 1
  if conf.CheckLibWithHeader('lualib50', 'lualib.h', 'c'):
     return 1
  if conf.CheckLibWithHeader('lualib5.0', 'lualib.h', 'c'):
     return 1
  if conf.CheckLibWithHeader('lualib51', 'lualib.h', 'c'):
     return 1
  if conf.CheckLibWithHeader('lualib5.1', 'lualib.h', 'c'):
     return 1
  return 0

def AutoConfigure(env):
  conf = Configure(env)

  ## check for required libs ##
  if not conf.CheckLibWithHeader('png', 'png.h', 'c'):
     print 'Did not find png library or headers, exiting!'
     Exit(1)
  if not conf.CheckLibWithHeader('z', 'zlib.h', 'c'):
     print 'Did not find the zlib library or headers, exiting!'
     Exit(1)
  if not 'USE_WIN32' in env['CPPDEFINES'] and not conf.CheckLib('dl'):
     print 'Did not find dl library or header which is needed on some systems for lua. Exiting!'
     Exit(1)
  if not CheckLuaLib(env, conf):
     print 'Did not find required lua library. Exiting!'
     Exit(1)

  # Check for optional libraries #
  if conf.CheckLib('vorbis'):
     env.Append(CPPDEFINES = 'USE_VORBIS')
  if conf.CheckLib('theora'):
     env.Append(CPPDEFINES = 'USE_THEORA')
  if conf.CheckLib('ogg'):
     env.Append(CPPDEFINES = 'USE_OGG')
  if env['opengl'] == 1:
     CheckOpenGL(env, conf)
  
  # check for optional functions
  if conf.CheckFunc('strcasestr'):
     env.Append(CPPDEFINES = 'HAVE_STRCASESTR')

  # check for optional headers
  if (conf.CheckHeader('X11/Xlib.h') and conf.CheckHeader('X11/Xatom.h') and
     conf.CheckLib('X11')):
     env.Append(CPPDEFINES = 'HAVE_X')

  # Determine compiler and linker flags for SDL. Must be done at the end 
  # as on some platforms, SDL redefines main which conflicts with the checks.
  if not 'USE_WIN32' in env['CPPDEFINES']:
    env.ParseConfig('sdl-config --cflags')
    env.ParseConfig('sdl-config --libs')
    if not '-Dmain=SDL_main' in env['CCFLAGS'] and not conf.CheckLibWithHeader('SDL', 'SDL.h', 'c'):
       print 'Did not find SDL library or headers, exiting!'
       Exit(1)

  env = conf.Finish()

def AutoConfigureIfNeeded(env, name):
   cachename = "build_conf_%scache.py" % name
   if os.path.exists(cachename):  
      if optionsChanged or \
         os.stat(cachename)[ST_MTIME] < os.stat("SConstruct")[ST_MTIME]:
            # Remove outdated cache file
            os.remove(cachename)
   if optionsChanged or not os.path.exists(cachename):
      print cachename + " doesn't exist or out of date."
      print "Generating new build config cache ..."
      cache = DefineOptions(cachename, {})
      AutoConfigure(env)
      cache.Save(cachename, env)
   else:
      cache = DefineOptions(cachename, {})
      print "Using " + cachename
      cache.Update(env)

AutoConfigureIfNeeded(env, '')

def addBosWarsPaths(env):
   # Stratagus build specifics
   env.Append(CPPPATH=engineSourceDir+'/include')
   env.Append(CPPPATH=engineSourceDir+'/guichan/include')
addBosWarsPaths(env)

# define the different build environments (variants)
release = env.Copy()
release.Append(CCFLAGS = Split('-O2 -pipe -fomit-frame-pointer -fexpensive-optimizations -ffast-math'))

if mingw['extrapath']:
  mingw.Tool('crossmingw', toolpath = ['tools/scons/'])
  mingw['CPPDEFINES'] = ['USE_WIN32']
  if mingw['extrapath'] != '':
     x = mingw['extrapath']
     mingw['CPPPATH'] = [x + '/include']
     mingw['LIBPATH'] = [x + '/lib']
  AutoConfigureIfNeeded(mingw, 'mingw')
  addBosWarsPaths(mingw)
  mingw.Append(LIBS = ['mingw32', 'SDLmain', 'SDL', 'wsock32', 'ws2_32'])
  mingw.Append(LINKFLAGS = ['-mwindows'])
else:
  mingw = None

debug = env.Copy()
debug.Append(CPPDEFINES = 'DEBUG')
debug.Append(CCFLAGS = Split('-g -Wsign-compare -Wall -Werror'))

profile = debug.Copy()
profile.Append(CCFLAGS = Split('-pg'))
profile.Append(LINKFLAGS = Split('-pg'))

staticenv = release.Copy()
staticlibs = 'lua lua50 lua5.0 lua5.1 lua51 lualib lualib50 lualib51 lualib5.0 lualib5.1 vorbis theora ogg'
staticlibs = staticlibs.split(' ')
if os.access('libstdc++.a', os.F_OK) == 0:
     l = os.popen(static['CXX'] + ' -print-file-name=libstdc++.a').readlines()
     os.symlink(l[0][:-1], os.path.join(os.curdir, 'libstdc++.a'))
LINKFLAGS = '-L. -Wl,-Bstatic -lstdc++ '
for i in staticlibs:
    if i in staticenv['LIBS']:
          LINKFLAGS += '-l%s ' % i
          staticenv['LIBS'].remove(i)
LINKFLAGS += '-logg -Wl,-Bdynamic'
staticenv['LINKFLAGS'].append(LINKFLAGS.split())

# Targets
def DefineVariant(venv, v, vv = None):
   if vv == None:
      vv = '-' + v
   BuildDir('build/' + v, engineSourceDir, duplicate = 0)
   r = venv.Program('boswars' + vv, buildSourcesList('build/' + v))
   Alias(v, 'boswars' + vv)
   return r 

r = DefineVariant(release, 'release', '')
Default(r)
DefineVariant(debug, 'debug')
DefineVariant(profile, 'profile')
if staticenv:
   DefineVariant(staticenv, 'static')
if mingw:
   DefineVariant(mingw, 'mingw', '.exe')

