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

#include "gui.hpp"
#include "screens/screenCommon.hpp"

#include <3ds.h>
#include <assert.h>
#include <stack>
#include <stdarg.h>
#include <unistd.h>

C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

C2D_TextBuf sizeBuf;
C2D_Font systemFont;
bool currentScreen = false;

static C2D_SpriteSheet SpriteSheet, DSiSheet, AcekardSheet, R4Sheet, SplashSheet, DotCircleSheet, BorderSheet;
std::stack<std::unique_ptr<SCREEN>> screens;

void Gui::clearTextBufs(void)
{
    C2D_TextBufClear(sizeBuf);
}

void Load_SpriteSheets(void) {
	// Load all the Spritesheets.
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLACK);
	C2D_TargetClear(bottom, BLACK);
	Gui::clearTextBufs();
	set_screen(top);
	Draw_Rect(0, 0, 400, 27, BLUE);
	Draw_Rect(0, 27, 400, 186, GREEN);
	Draw_Rect(0, 213, 400, 27, BLUE);
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Loading Spritesheets... please wait."))/2, 2, 0.8f, WHITE, "Loading Spritesheets... please wait.", 400);
	set_screen(bottom);
	Draw_Rect(0, 0, 320, 27, BLUE);
	Draw_Rect(0, 27, 320, 186, GREEN);
	Draw_Rect(0, 213, 320, 27, BLUE);
	C3D_FrameEnd(0);

    SpriteSheet    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    DSiSheet    = C2D_SpriteSheetLoad("romfs:/gfx/dsi.t3x");
    AcekardSheet    = C2D_SpriteSheetLoad("romfs:/gfx/acekard.t3x");
    R4Sheet    = C2D_SpriteSheetLoad("romfs:/gfx/r4.t3x");
    SplashSheet    = C2D_SpriteSheetLoad("romfs:/gfx/splash.t3x");
    DotCircleSheet = C2D_SpriteSheetLoad("romfs:/gfx/dotcircle.t3x");
    BorderSheet     = C2D_SpriteSheetLoad("romfs:/gfx/border.t3x");
}

Result Gui::init(void)
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    sizeBuf = C2D_TextBufNew(4096);
    Load_SpriteSheets();
    systemFont = C2D_FontLoadSystem(CFG_REGION_USA);
    return 0;
}

void sheetsFree(void) {
    C2D_SpriteSheetFree(SpriteSheet);
    C2D_SpriteSheetFree(DSiSheet);
    C2D_SpriteSheetFree(AcekardSheet);
    C2D_SpriteSheetFree(R4Sheet);
    C2D_SpriteSheetFree(SplashSheet);
    C2D_SpriteSheetFree(BorderSheet);
    C2D_SpriteSheetFree(DotCircleSheet);
}

void Gui::exit(void)
{
    sheetsFree();
    C2D_TextBufDelete(sizeBuf);
    C2D_Fini();
    C3D_Fini();
}

void set_screen(C3D_RenderTarget * screen)
{
    C2D_SceneBegin(screen);
    currentScreen = screen == top ? 1 : 0;
}

// This is mainly, to replace `\\n` from the Ini file with `\n`.
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
 
	// Repeat till end is reached
	while( pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos =data.find(toSearch, pos + replaceStr.size());
	}
}

void Gui::DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth) {
    Gui::DrawString((currentScreen ? 400 : 320)+x-(std::min(maxWidth, (int)Gui::GetStringWidth(size, Text)/2)), y, size, color, Text, maxWidth);
}

// Draw String or Text.
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth)
{
    float width = 1, height = 1;
    
    findAndReplaceAll(Text, "\\n", "\n");
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());

	if(maxWidth > 0) {
		C2D_TextGetDimensions(&c2d_text, size, size, &width, &height);
	}


	C2D_TextOptimize(&c2d_text);
	C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, std::min(size, size*(maxWidth/width)), size, color);
}


// Get String or Text Width.
float Gui::GetStringWidth(float size, std::string Text) {
    findAndReplaceAll(Text, "\\n", "\n");
	float width = 0;
	GetStringSize(size, &width, NULL, Text);
	return width;
}

// Get String or Text Size.
void Gui::GetStringSize(float size, float *width, float *height, std::string Text) {
    findAndReplaceAll(Text, "\\n", "\n");
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}


// Get String or Text Height.
float Gui::GetStringHeight(float size, std::string Text) {
    findAndReplaceAll(Text, "\\n", "\n");
	float height = 0;
	GetStringSize(size, NULL, &height, Text.c_str());
	return height;
}

bool Draw_Rect(float x, float y, float w, float h, u32 color) {
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}

// All the Draws of the Spritesheets.

// Draw a Sprite from the Sheet.
void Gui::sprite(int sheet, int key, int x, int y)
{
    if (sheet == 0) { // Sprites.
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(SpriteSheet, key), x, y, 0.5f);
    } else if (sheet == 1) { // credits.
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(AcekardSheet, key), x, y, 0.5f);
    } else if (sheet == 2) { // animation.
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(DSiSheet, key), x, y, 0.5f);
    } else if (sheet == 3) { // button.
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(R4Sheet, key), x, y, 0.5f);
    } else if (sheet == 4) {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(BorderSheet, key), x, y, 0.5f);
    } else if (sheet == 5) {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(DotCircleSheet, key), x, y, 0.5f);
    } else if (sheet == 6) {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(SplashSheet, key), x, y, 0.5f);
    }
}

void Gui::mainLoop(u32 hDown, u32 hHeld, touchPosition touch) {
	screens.top()->Draw();
	screens.top()->Logic(hDown, hHeld, touch);
}

void Gui::setScreen(std::unique_ptr<SCREEN> screen)
{
    screens.push(std::move(screen));
}

void Gui::screenBack()
{
    screens.pop();
}