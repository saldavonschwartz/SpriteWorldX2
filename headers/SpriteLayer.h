///--------------------------------------------------------------------------------------
//	SpriteLayer.h
//
//	Portions are copyright: � 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	constants, structures, and prototypes for sprite layers
///--------------------------------------------------------------------------------------


#ifndef __SPRITELAYER__
#define __SPRITELAYER__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#ifndef _SDL_H
#include <SDL.h>
#endif

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SPRITE__
#include <Sprite.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

///--------------------------------------------------------------------------------------
//	sprite layer data structure
///--------------------------------------------------------------------------------------


struct SpriteLayerRec
{
	SpriteWorldPtr	parentSpriteWorldP;	// The SpriteWorld this layer is in, otherwise NULL.

	SpriteLayerPtr	nextSpriteLayerP;	// next sprite layer
	SpriteLayerPtr	prevSpriteLayerP;	// previous sprite layer

	SpritePtr		headSpriteP;		// head of sprite linked list
	SpritePtr		tailSpriteP;		// tail of sprite linked list


	int				isPaused;			// does this layer get processed by SWProcessSpriteWorld?
	int				layerIsNonScrolling;// is this a non-scrolling SpriteLayer?

        
        short		tileLayer;				// the tile layer this sprite layer is under
        
	long			userData;			// reserved for user
};

///--------------------------------------------------------------------------------------
//	sprite layer function prototypes
///--------------------------------------------------------------------------------------

SWError SWCreateSpriteLayer(
	SpriteLayerPtr *spriteLayerP);

void SWDisposeSpriteLayer(
	SpriteLayerPtr *spriteLayerPP);

SWError SWAddSprite(
	SpriteLayerPtr spriteLayerP,
	SpritePtr newSpriteP);

SWError SWRemoveSprite(
	SpritePtr oldSpriteP);

void SWRemoveAllSpritesFromLayer(
	SpriteLayerPtr srcSpriteLayerP);

void SWDisposeAllSpritesInLayer(
	SpriteLayerPtr spriteLayerP);

unsigned long SWCountNumSpritesInLayer(
	SpriteLayerPtr spriteLayerP);

SWError SWSwapSprite(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP);

SWError SWInsertSpriteAfterSprite(
	SpritePtr newSpriteP,
	SpritePtr dstSpriteP);

SWError SWInsertSpriteBeforeSprite(
	SpritePtr newSpriteP,
	SpritePtr dstSpriteP);

SpritePtr SWGetNextSprite(
	SpriteLayerPtr spriteLayerP,
	SpritePtr curSpriteP);

SpritePtr SWGetPreviousSprite(
	SpriteLayerPtr spriteLayerP,
	SpritePtr curSpriteP);

void SWLockSpriteLayer(
	SpriteLayerPtr spriteLayerP);

void SWUnlockSpriteLayer(
	SpriteLayerPtr spriteLayerP);

void SWMoveSpriteLayer(
	SpriteLayerPtr spriteLayerP);

void SWCollideSpriteLayer(
	SpriteWorldPtr	spriteWorldP,
	SpriteLayerPtr srcSpriteLayerP,
	SpriteLayerPtr dstSpriteLayerP);

int SWSpriteCollidesWithLayer(
	SpritePtr srcSpriteP,
	SpriteLayerPtr spriteLayerP,
	SpriteCollisionProcPtr collideFunc);

void SWSortSpriteLayer(
	SpriteLayerPtr theLayer);

void SWCustomSortSpriteLayer(
	SpriteLayerPtr theLayer,
	SpriteCompareProcPtr compareFunc);

void SWPauseSpriteLayer(
	SpriteLayerPtr	spriteLayerP);

void SWUnpauseSpriteLayer(
	SpriteLayerPtr	spriteLayerP);

void SWSetLayerAsNonScrolling(
	SpriteLayerPtr	spriteLayerP,
	int layerIsNonScrolling);

SpritePtr SWFindSpriteByPoint(
	SpriteLayerPtr spriteLayerP,
	SpritePtr startSpriteP,
	int testH, int test);

SpritePtr SWTopSpriteFromPoint(
	SpriteLayerPtr spriteLayerP,
	SpritePtr startSpriteP,
	int testH, int testV,
	SpriteInMaskProcPtr testFunc);

void SWMoveSpriteLayerToTail (
	SpriteWorldPtr	spriteWorldP,
	SpriteLayerPtr moveSpriteLayerP);


#ifdef __cplusplus
}
#endif

#endif /* __SPRITELAYER__ */

