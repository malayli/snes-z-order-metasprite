#include <snes.h>
#include "common/vblank.h"
#include "common/utils.h"
#include "common/input.h"
#include "common/oam.h"
#include "common/palette.h"
#include "common/spriteEngine.h"

// ROM

const u16 spritePalette[] = {
    RGB8(255, 0, 255),
    RGB8(0, 0, 255),
    RGB8(255, 0, 0),
    RGB8(0, 255, 0),
    RGB8(206, 140, 115),
    RGB8(214, 189, 173),
    RGB8(231, 231, 222),
    RGB8(140, 140, 0),
    RGB8(181, 181, 0),
    RGB8(255, 255, 0),
    RGB8(0, 0, 173),
    RGB8(82, 82, 222),
    RGB8(132, 132, 255),
    RGB8(148, 148, 148),
    RGB8(222, 222, 222),
    RGB8(0, 74, 0)
};

const u8 emptyPicture[] = {
    // First part
    0b00000000, 0b00000000, // Bit plane 1 + Bit plane 0
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000,

    // Second part
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000
};

const u8 squareFillPicture[] = {
    // First part
    0b11111111, 0b00000000,
    0b11111111, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b00000000,

    // Second part
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000
};

const u8 squareFillPicture2[] = {
    // First part
    0b00000000, 0b11111111,
    0b00000000, 0b11111111, 
    0b00000000, 0b11111111, 
    0b00000000, 0b11111111, 
    0b00000000, 0b11111111, 
    0b00000000, 0b11111111, 
    0b00000000, 0b11111111, 
    0b00000000, 0b11111111,

    // Second part
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000
};

const u8 squareFillPicture3[] = {
    // First part
    0b11111111, 0b11111111,
    0b11111111, 0b11111111, 
    0b11111111, 0b11111111, 
    0b11111111, 0b11111111, 
    0b11111111, 0b11111111, 
    0b11111111, 0b11111111, 
    0b11111111, 0b11111111, 
    0b11111111, 0b11111111,

    // Second part
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000
};

// Metasprite
#define MS_TYPE_INDEX 0
#define MS_X_INDEX 1
#define MS_Y_INDEX 2
#define MS_SPRITES_INDEX 3
#define MS_MOVE_INDEX 4
#define MS_ITEM_SIZE 5

// Sprite
#define SPR_X_INDEX 0
#define SPR_Y_INDEX 1
#define SPR_GFX_INDEX 2
#define SPR_ATTR_INDEX 3
#define SPR_SIZE_INDEX 4
#define SPR_ITEM_SIZE 5

const u16 objSprites[] = {
    -16, 16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    16, 16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    -8, 16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    8, 16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    -8, 8, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    8, 8, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    0, 0, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    0, -8, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    0, -16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    -8, -16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    -16, -16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL,
    16, -16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL,
    8, -16, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    0, -24, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_SMALL, 
    -4, -40, 3, OAM_ATTR(2, 0, 0, 3, PAL0), OBJ_LARGE, 
    0xFFFF,
    -16, 16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    16, 16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    -8, 16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    8, 16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    -8, 8, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    8, 8, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    0, 0, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    0, -8, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL,
    0, -16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    -8, -16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    -16, -16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL,
    16, -16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL,
    8, -16, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    0, -24, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_SMALL, 
    -4, -40, 1, OAM_ATTR(2, 0, 0, 1, PAL0), OBJ_LARGE, 
    0xFFFF
};

#define SPR_PLAYER_IDLE_INDEX 0
#define SPR_ENEMY_IDLE_INDEX 76

// RAM

u16 oamIndex;
u16 playerIndex;
u16 bg3TileMap[1024];
u16 bgTileIndex;
u16 metaSpriteTemp[MS_ITEM_SIZE];
u8 updated;
u16 xTmp, yTmp;
u8 enemyMoveState;
u16 typeTmp;
s16 i, j;
u16 *ptr, *ptr2;
u16 metaSprites[] = {
    5, 16, 140, SPR_PLAYER_IDLE_INDEX, 0,
    1, 92, 70, SPR_ENEMY_IDLE_INDEX, 0,
    1, 140, 100, SPR_ENEMY_IDLE_INDEX, 0,
    1, 200, 130, SPR_ENEMY_IDLE_INDEX, 0,
    0xFFFF
};

/*!\brief Sort an array of metasprites according to the Y position.
*/
void sortMetaSprites() {
    i = MS_ITEM_SIZE;

    while (metaSprites[i] != 0xFFFF) {
        yTmp = metaSprites[i + MS_Y_INDEX];

        metaSpriteTemp[MS_TYPE_INDEX] = metaSprites[i + MS_TYPE_INDEX];
        metaSpriteTemp[MS_X_INDEX] = metaSprites[i + MS_X_INDEX];
        metaSpriteTemp[MS_Y_INDEX] = yTmp;
        metaSpriteTemp[MS_SPRITES_INDEX] = metaSprites[i + MS_SPRITES_INDEX];
        metaSpriteTemp[MS_MOVE_INDEX] = metaSprites[i + MS_MOVE_INDEX];
        j = i - MS_ITEM_SIZE;

        // Move elements of arr[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && metaSprites[j + MS_Y_INDEX] < yTmp) {
            metaSprites[j + MS_ITEM_SIZE + MS_TYPE_INDEX] = metaSprites[j + MS_TYPE_INDEX];
            metaSprites[j + MS_ITEM_SIZE + MS_X_INDEX] = metaSprites[j + MS_X_INDEX];
            metaSprites[j + MS_ITEM_SIZE + MS_Y_INDEX] = metaSprites[j + MS_Y_INDEX];
            metaSprites[j + MS_ITEM_SIZE + MS_SPRITES_INDEX] = metaSprites[j + MS_SPRITES_INDEX];
            metaSprites[j + MS_ITEM_SIZE + MS_MOVE_INDEX] = metaSprites[j + MS_MOVE_INDEX];
            j = j - MS_ITEM_SIZE;
        }

        metaSprites[j + MS_ITEM_SIZE + MS_TYPE_INDEX] = metaSpriteTemp[MS_TYPE_INDEX];
        metaSprites[j + MS_ITEM_SIZE + MS_X_INDEX] = metaSpriteTemp[MS_X_INDEX];
        metaSprites[j + MS_ITEM_SIZE + MS_Y_INDEX] = yTmp;
        metaSprites[j + MS_ITEM_SIZE + MS_SPRITES_INDEX] = metaSpriteTemp[MS_SPRITES_INDEX];
        metaSprites[j + MS_ITEM_SIZE + MS_MOVE_INDEX] = metaSpriteTemp[MS_MOVE_INDEX];

        i += MS_ITEM_SIZE;
    }
}

/*!\brief Set all the tiles to 0, set a palette number and a tile priority.
*/
void clearBgTextEx(u16 *tileMap, u8 paletteNumber, u8 priority) {
    for (bgTileIndex=0; bgTileIndex < 1024;) {
        tileMap[bgTileIndex] = 0 | (paletteNumber<<10) | (priority<<13);
        bgTileIndex += 1;
    }
}

/*!\brief Load a black background on BG3.
*/
void initBg3Black() {
    bgSetMapPtr(BG2, 0x0000 + 2048, SC_32x32);
    bgSetGfxPtr(BG2, 0x5000);
    clearBgTextEx((u16 *)bg3TileMap, PAL0, 0);
    WaitForVBlank();
    setPaletteColor(PAL0, blackColor);
    dmaCopyVram((u8 *)bg3TileMap, 0x1000, 32*32*2);
    dmaCopyVram((u8 *)emptyPicture, 0x5000, 32);
}

/*!\brief Set an array of metasprites in the OAM.
*/
void oamSetMetaSprites() {
    oamIndex = 0;
    ptr = metaSprites;

    while((*ptr) != 0xFFFF) {
        xTmp = *(ptr + MS_X_INDEX);
        yTmp = *(ptr + MS_Y_INDEX);
        ptr2 = objSprites + *(ptr + MS_SPRITES_INDEX);

        while((*ptr2) != 0xFFFF) {
            oamSetAttr(oamIndex,
                xTmp + *(ptr2 + SPR_X_INDEX),
                yTmp + *(ptr2 + SPR_Y_INDEX),
                *(ptr2 + SPR_GFX_INDEX),
                *(ptr2 + SPR_ATTR_INDEX));

            oamSetEx(oamIndex, *(ptr2 + SPR_SIZE_INDEX), OBJ_SHOW);
            
            ptr2 += SPR_ITEM_SIZE;
            oamIndex += 4;
        }

        ptr += MS_ITEM_SIZE;
    }
}

/*!\brief Set metasprites {x, y} position in the OAM.
*/
void oamSetMetaSpritesXY() {
    oamIndex = 0;
    ptr = metaSprites;

    while((*ptr) != 0xFFFF) {
        ptr2 = objSprites + *(ptr + MS_SPRITES_INDEX);

        if (*(ptr + MS_MOVE_INDEX) == 0) {
            while(*(ptr2) != 0xFFFF) {
                ptr2 += SPR_ITEM_SIZE;
                oamIndex += 4;
            }
            ptr += MS_ITEM_SIZE;
            continue;
        }

        xTmp = *(ptr + MS_X_INDEX);
        yTmp = *(ptr + MS_Y_INDEX);

        while((*ptr2) != 0xFFFF) {      
            oamSetXY(oamIndex,
                xTmp + *(ptr2 + SPR_X_INDEX),
                yTmp + *(ptr2 + SPR_Y_INDEX));

            ptr2 += SPR_ITEM_SIZE;
            oamIndex += 4;
        }

        *(ptr + MS_MOVE_INDEX) = 0;

        ptr += MS_ITEM_SIZE;
    }
}

/*!\brief Find and set the player index in the array of metasprites.
*/
void findPlayerIndex() {
    i = 0;
    typeTmp = metaSprites[i];

    while (typeTmp != 0xFFFF) {
        if (typeTmp == 5) {
            updated = (playerIndex != i);
            playerIndex = i;
            break;
        }
            
        i += MS_ITEM_SIZE;
        typeTmp = metaSprites[i];
    }
}

/*!\brief Move enemies metasprites.
*/
void moveEnemies() {
    metaSpriteIndex = 0;

    while(metaSprites[metaSpriteIndex] != 0xFFFF) {
        if (metaSpriteIndex != playerIndex) {
            metaSprites[metaSpriteIndex + MS_MOVE_INDEX] = 1;

            yTmp = metaSprites[metaSpriteIndex + MS_Y_INDEX];

            switch(enemyMoveState) {
                case 0:
                    if (yTmp < 200) {
                        metaSprites[metaSpriteIndex + MS_Y_INDEX] = yTmp + 1;
                    } else {
                        enemyMoveState = 1;
                    }
                    break;

                case 1:
                    if (yTmp > 40) {
                        metaSprites[metaSpriteIndex + MS_Y_INDEX] = yTmp - 1;
                    } else {
                        enemyMoveState = 0;
                    }
                    break;
            }
        }

        metaSpriteIndex += MS_ITEM_SIZE;
    }
}

int main(void) {
    consoleInit();

    nmiSet(superNintendoVblank);

    initBg3Black();

    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);
    bgSetDisable(BG0);
    bgSetDisable(BG1);
    bgSetEnable(BG2);
    bgSetDisable(BG3);

    // Workaround for SNES
    bgSetScroll(BG0, 0, -1);
    bgSetScroll(BG1, 0, -1);
    bgSetScroll(BG2, 0, -1);

    WaitForVBlank();

    playerIndex = 0;
    prio = 2;
    updated = 0;
    enemyMoveState = 0;

    initSpriteEngine(OBJ_SIZE8_L16);

    dmaCopyCGram((u8 *)spritePalette, 128 + (PAL0<<4), 32);
    dmaCopyVram((u8 *)squareFillPicture, oamAddressOrigin, 32);

    dmaCopyVram((u8 *)squareFillPicture2, oamAddressOrigin + 0x10, 32);
    dmaCopyVram((u8 *)squareFillPicture2, oamAddressOrigin + 0x20, 32);
    dmaCopyVram((u8 *)squareFillPicture2, oamAddressOrigin + 0x110, 32);
    dmaCopyVram((u8 *)squareFillPicture2, oamAddressOrigin + 0x120, 32);

    dmaCopyVram((u8 *)squareFillPicture3, oamAddressOrigin + 0x30, 32);
    dmaCopyVram((u8 *)squareFillPicture3, oamAddressOrigin + 0x40, 32);
    dmaCopyVram((u8 *)squareFillPicture3, oamAddressOrigin + 0x130, 32);
    dmaCopyVram((u8 *)squareFillPicture3, oamAddressOrigin + 0x140, 32);

    WaitForVBlank();

    sortMetaSprites();
    findPlayerIndex();
    oamSetMetaSprites();

    setFadeEffect(FADE_IN);
    WaitForVBlank();

    while(1) {
        scanPads();
        pad0 = padsCurrent(0);
        padDown0 = padsDown(0);

        if (pad0 & KEY_RIGHT) {
            metaSprites[playerIndex + MS_X_INDEX] += 1;
            metaSprites[playerIndex + MS_MOVE_INDEX] = 1;
        }

        if (pad0 & KEY_LEFT) {
            metaSprites[playerIndex + MS_X_INDEX] -= 1;
            metaSprites[playerIndex + MS_MOVE_INDEX] = 1;
        }

        if (pad0 & KEY_UP) {
            metaSprites[playerIndex + MS_Y_INDEX] -= 1;
            metaSprites[playerIndex + MS_MOVE_INDEX] = 1;
        }

        if (pad0 & KEY_DOWN) {
            metaSprites[playerIndex + MS_Y_INDEX] += 1;
            metaSprites[playerIndex + MS_MOVE_INDEX] = 1;
        }

        moveEnemies();

        sortMetaSprites();

        findPlayerIndex();

        if (updated == 1) {
            oamSetMetaSprites();
            updated = 0;

        } else {
            oamSetMetaSpritesXY();
        }

        WaitForVBlank();
    }

    return 0;
}
