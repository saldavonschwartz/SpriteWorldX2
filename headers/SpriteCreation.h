///--------------------------------------------------------------------------------------
//	SpriteCreation.h
//
//	Portions are copyright: c 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	constants, structures, and prototypes for creation of sprites
///--------------------------------------------------------------------------------------


#ifndef __SPRITECREATION__
#define __SPRITECREATION__

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

#ifndef __SPRITEFRAME__
#include <SpriteFrame.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

///--------------------------------------------------------------------------------------
//	spritecreation function prototypes
///--------------------------------------------------------------------------------------
SWError SWCreateSpriteFromSingleFileXY(SpritePtr* newSpriteP,
	void* spriteStorageP,const char* fileName,int frameWidth,int frameHeight,int borderWidth,
	int borderHeight,int hasOutsideBorder,long maxFrames );

SWError SWCreateSpriteFromFileSequence(SpritePtr* newSpriteP,void* spriteStorageP, 
    const char *filename, int start,long maxFrames);

SWError SWCreateSpriteFromFile(SpritePtr* newSpriteP,void* spriteStorageP,
	const char * file,long maxFrames);

SWError SWCreateSprite(SpritePtr* newSpriteP,void* spriteStorage,long maxFrames);
	
SWError SWCloneSprite(
	SpritePtr	cloneSpriteP,
	SpritePtr*	newSpriteP,
	void*		spriteStorageP);
	
#ifdef __cplusplus
}
#endif

#endif	/* __SPRITECREATION__ */

