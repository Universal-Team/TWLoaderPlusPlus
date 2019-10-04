/*
*   This file is part of TWLoader++
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "utils/inifile.h"
#include "utils/config.hpp"

#include <3ds.h>
#include <string>
#include <unistd.h>

using std::string;
using std::wstring;

CIniFile ndsbootstrapini( "sdmc:/_nds/nds-bootstrap.ini" );
CIniFile twloaderppini( "sdmc:/TWLoaderPlusPlus/settings.ini" );

int Config::SpeedMode;
int Config::VRAMBoost;
int Config::Version;
std::string Config::PATH;
std::string Config::SavPath;

std::string Config::savepath;
std::string Config::gamepath;

void Config::loadTWLoaderPPIni() {
	Config::gamepath = twloaderppini.GetString("NDS-BOOTSTRAP", "NDS_PATH", Config::gamepath);
	Config::savepath = twloaderppini.GetString("NDS-BOOTSTRAP", "SAV_PATH", Config::savepath);
	
}

void Config::createTWLoaderPPIni() {
	twloaderppini.SetString("NDS-BOOTSTRAP", "NDS_PATH", Config::gamepath);
	twloaderppini.SetString("NDS-BOOTSTRAP", "SAV_PATH", Config::savepath);
	twloaderppini.SaveIniFile("sdmc:/TWLoaderPlusPlus/settings.ini");
}

void Config::loadNDSBootstrap() {
	Config::SpeedMode = ndsbootstrapini.GetInt("NDS-BOOTSTRAP", "BOOST_CPU", Config::SpeedMode);
	Config::VRAMBoost = ndsbootstrapini.GetInt("NDS-BOOTSTRAP", "BOOST_VRAM", Config::VRAMBoost);
	Config::PATH = ndsbootstrapini.GetString("NDS-BOOTSTRAP", "NDS_PATH", Config::PATH);
	Config::SavPath = ndsbootstrapini.GetString("NDS-BOOTSTRAP", "SAV_PATH", Config::SavPath);
}


void Config::saveNDSBootstrap() {
	ndsbootstrapini.SetString("NDS-BOOTSTRAP", "NDS_PATH", Config::PATH);
	ndsbootstrapini.SetString("NDS-BOOTSTRAP", "SAV_PATH", Config::SavPath);
	ndsbootstrapini.SaveIniFile("sdmc:/_nds/nds-bootstrap.ini");
}