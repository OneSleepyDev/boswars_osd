//     ____                _       __               
//    / __ )____  _____   | |     / /___ ___________
//   / __  / __ \/ ___/   | | /| / / __ `/ ___/ ___/
//  / /_/ / /_/ (__  )    | |/ |/ / /_/ / /  (__  ) 
// /_____/\____/____/     |__/|__/\__,_/_/  /____/  
//                                              
//       A futuristic real-time strategy game.
//          This file is part of Bos Wars.
//
/**@name title.h - The title screen headerfile. */
//
//      (c) Copyright 2007 by Jimmy Salmon
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

#ifndef __TITLE_H__
#define __TITLE_H__

//@{

#include <string>


class CFont;

enum {
	TitleFlagCenter = 1 << 0,  /// Center Text
};

class TitleScreenLabel {
public:
	TitleScreenLabel() : Font(0), Xofs(0), Yofs(0), Flags(0) {}

	std::string Text;
	CFont *Font;
	int Xofs;
	int Yofs;
	int Flags;
};

class TitleScreen {
public:
	TitleScreen() : StretchImage(true), Timeout(0), Labels(NULL) {}
	~TitleScreen() {
		if (this->Labels) {
			for (int i = 0; this->Labels[i]; ++i) {
				delete this->Labels[i];
			}
			delete[] this->Labels;
		}
	}

	std::string File;
	std::string Music;
	bool StretchImage;
	int Timeout;
	TitleScreenLabel **Labels;
};

extern TitleScreen **TitleScreens;          /// File for title screen

extern void ShowTitleScreens();

//@}

#endif // !__TITLE_H__
