///--------------------------------------------------------------------------------------
//	Sprite.h
//
//	Portions are copyright: c 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	constants, structures, and prototypes for sprites
///--------------------------------------------------------------------------------------



#ifndef __SPRITE__
#define __SPRITE__

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
//	sprite type definitions
///--------------------------------------------------------------------------------------

typedef void (*FrameProcPtr)(
	SpritePtr srcSpriteP,
	FramePtr curFrameP,
	long *frameIndex);

typedef void (*DrawProcPtr)(
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect *srcRect,
	SWRect *dstRect);

typedef void (*MoveProcPtr)(
	SpritePtr srcSpriteP);

typedef void (*CollideProcPtr)(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP,
	SWRect* sectRect);

	// examples functions of this type are:
	// SWInRegionMask, SWInPixelMask, SWInRLEMask, SWInRadiusMask
typedef int (*SpriteInMaskProcPtr)(
	SpritePtr spriteP,
	int testH, int testV); //ben changed from point type

	// examples functions of this type are:
	// SWRegionCollision, SWPixelCollision, SWRLECollision, SWRadiusCollision
typedef int (*SpriteCollisionProcPtr)(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP);

	// comparision function returns:
	// <0 if srcSpriteP < dstSpriteP
	//  0 if srcSpriteP == dstSpriteP
	// >0 if srcSpriteP > dstSpriteP
typedef short (*SpriteCompareProcPtr)(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP);



///--------------------------------------------------------------------------------------
//	enumeration type definitions
///--------------------------------------------------------------------------------------

typedef enum
{
	kSWDontRemoveSprite = 0,
	kSWRemoveSprite,
	kSWRemoveAndDisposeSprite
} RemovalType;

typedef enum
{
	kSWWrapAroundMode = 0,
	kSWPatrollingMode
} AdvanceType;


///--------------------------------------------------------------------------------------
//	sprite data structure
///--------------------------------------------------------------------------------------


struct SpriteRec
{
	SpriteLayerPtr	parentSpriteLayerP;	// the sprite layer this sprite is currently in
	
	SpritePtr 		nextSpriteP;		// next sprite in that layer
	SpritePtr		prevSpriteP;		// previous sprite in that layer
	
	SpritePtr		nextActiveSpriteP;	// next active sprite in the SpriteWorld
	SpritePtr		nextIdleSpriteP;	// next idle sprite in the SpriteWorld

		// drawing fields
	int				isVisible;			// draw the sprite?
	int			    needsToBeDrawn;		// sprite has changed, needs to be drawn
	int			    needsToBeErased;	// sprite needs to be erased onscreen
	int				doFastMovingCheck;	// used by SWAnimateSpriteWorld
	SWRect			destFrameRect;		// frame destination rectangle
	SWRect			oldFrameRect;		// last frame destination rectangle
	SWRect			deltaFrameRect;		// union of the sprite's lastRect and curRect
	short			tileDepth;			// the tile layers this sprite is under 
	
	DrawProcPtr		frameDrawProc;			// callback to draw sprite normally
	
		// drawing fields for scrolling routines
	int				destRectIsVisible;	// is destOffscreenRect visible on screen?
	int				oldRectIsVisible;	// was any part of the Sprite on the screen last frame?
	SWRect			clippedSourceRect;	// source sprite frame rect after clipping
	SWRect			destOffscreenRect;	// sprite's dest rect after clipping and offset
	
	
	
    	// frame fields
	FramePtr		*frameArray;		// array of frames
	FramePtr		curFrameP;			// current frame
	long			numFrames;			// number of frames
	long			maxFrames;			// maximum number of frames
	long			frameTimeInterval;	// time interval to advance frame
	unsigned long 	timeOfLastFrameChange; // time (from runningTimeCount) frame was last changed
	long			frameAdvance;		// amount to adjust frame index
	long			curFrameIndex;		// current frame index
	long			firstFrameIndex;	// first frame to advance
	long			lastFrameIndex;		// last frame to advance
	FrameProcPtr 	frameChangeProc;	// callback to change frames

		// movement fields
	long			moveTimeInterval;	// time interval to move sprite
	unsigned long 	timeOfLastMove;		// time (from runningTimeCount) sprite was last moved
	double			pseudoMoveTimeInterval;	// used for time-based animation
	double			moveTimeAccum;		// used to set the moveProcHasFired variable.
	int				moveProcHasFired;	// used in time-based animations.
	SWRect			moveBoundsRect;		// bounds of the sprite's movement
	MoveProcPtr		spriteMoveProc;		// callback to handle movement
	MoveProcPtr 	realSpriteMoveProc;	// see SWTempSpriteMoveProc for more info.

		// collision detection
	CollideProcPtr	spriteCollideProc;	// callback to handle collisions

		// miscellaneous
	AdvanceType		frameAdvanceMode;	// mode of automatic frame advancement
	RemovalType		spriteRemoval;		// whether to remove sprite, and if so, how.
	
	SDL_Texture		*sharedSurface;		// if common surface is used for frames, here it is
	DrawDataPtr 	drawData;			// has various drawing style attributes
	
    long            userData;
};

///--------------------------------------------------------------------------------------
//	sprite function prototypes
///--------------------------------------------------------------------------------------


void SWRemoveSpriteFromAnimation(
	SpriteWorldPtr	spriteWorldP,
	SpritePtr		spriteP,
	int			disposeOfSprite);

void SWDisposeSprite(
	SpritePtr *deadSpritePP);

SWError SWAddFrame(
	SpritePtr srcSpriteP,
	FramePtr newFrameP);

SWError SWInsertFrame(
	SpritePtr	srcSpriteP,
	FramePtr	newFrameP,
	long		frameIndex);

void SWRemoveFrame(
	SpritePtr srcSpriteP,
	FramePtr oldFrameP);

FramePtr SWGetNextFrame(
	SpritePtr spriteP,
	FramePtr curFrameP);

void SWStdSpriteDrawProc(
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect* srcSWRect,
	SWRect* dstSWRect);

void SWLockSprite(SpritePtr srcSpriteP);
void SWUnlockSprite(SpritePtr srcSpriteP);

///--------------------------------------------------------------------------------------
//	Sprite Set-up functions
///--------------------------------------------------------------------------------------



SWError SWSetCurrentFrame(
	SpritePtr srcSpriteP,
	FramePtr curFrameP);

SWError SWSetCurrentFrameIndex(
	SpritePtr srcSpriteP,
	long frameIndex);

void SWSetSpriteCollideProc(
	SpritePtr srcSpriteP,
	CollideProcPtr collideProc);

void SWSetSpriteColor(
	SpritePtr srcSpriteP,
	SDL_Color *color);

void SWSetSpriteFrameAdvance(
	SpritePtr srcSpriteP,
	long frameAdvance);

void SWSetSpriteFrameAdvanceMode(
	SpritePtr srcSpriteP,
	AdvanceType advanceMode);

void SWSetSpriteFrameRange(
	SpritePtr srcSpriteP,
	long firstFrameIndex,
	long lastFrameIndex);

void SWSetSpriteFrameTime(
	SpritePtr srcSpriteP,
	long timeInterval);

void SWSetSpriteFrameProc(
	SpritePtr srcSpriteP,
	FrameProcPtr frameProc);

void SWSetSpriteLocation(
	SpritePtr srcSpriteP,
	SWMovementType horizLoc,
	SWMovementType vertLoc);

void SWSetSpriteMoveBounds(
	SpritePtr srcSpriteP,
	SWRect* moveBoundsRect);

void SWSetSpriteMoveDelta(
	SpritePtr srcSpriteP,
	SWMovementType horizDelta,
	SWMovementType vertDelta);

void SWSetSpriteMoveTime(
	SpritePtr srcSpriteP,
	long timeInterval);

void SWSetSpritePseudoMoveTime(
	SpritePtr srcSpriteP,
	long timeInterval);

void SWSetSpriteMoveProc(
	SpritePtr srcSpriteP, MoveProcPtr moveProc);

SWError SWSetSpriteDrawProc(
	SpritePtr srcSpriteP,
	DrawProcPtr drawProc);

void SWSetSpriteVisible(
	SpritePtr srcSpriteP,
	int isVisible);

void SWSetSpriteTranslucencyLevel(
	SpritePtr srcSpriteP,
	unsigned long level);

void SWSetSpriteFastMovingCheck(
	SpritePtr srcSpriteP,
	int makeChecks);

SWMovementType SWGetSpriteVertLoc(
	SpritePtr srcSpriteP);

SWMovementType SWGetSpriteHorizLoc(
	SpritePtr srcSpriteP);

short SWGetSpriteWidth(
	SpritePtr srcSpriteP);

short SWGetSpriteHeight(
	SpritePtr srcSpriteP);

///--------------------------------------------------------------------------------------
//	Sprite Runtime functions
///--------------------------------------------------------------------------------------

void SWMoveSprite(
	SpritePtr srcSpriteP,
	SWMovementType horizLoc,
	SWMovementType vertLoc);

void SWOffsetSprite(
	SpritePtr srcSpriteP,
	SWMovementType horizOffset,
	SWMovementType vertOffset);

void SWOffsetSpriteBasedOnTime(
	SpritePtr theSpriteP,
	SWMovementType horizOffset,
	SWMovementType vertOffset);

double SWConvertOffsetBasedOnTime(
	SpriteWorldPtr spriteWorldP,
	double theOffset,
	double framesPerSecond);

void SWResetSpriteFrameTimer(
	SpriteWorldPtr spriteWorldP,
	SpritePtr srcSpriteP);

int SWHasMoveProcFired(
	SpritePtr srcSpriteP);

int SWBounceSprite(
	SpritePtr srcSpriteP);

int SWWrapSprite(
	SpritePtr srcSpriteP);

int SWInRadiusMask(
	SpritePtr spriteP,
	int testPointH, int testPointV);

int SWInPixelMask(
	SpritePtr spriteP,
	int testPointH, int testPointV);

int SWRadiusCollision(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP);

int SWPixelCollision(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP);

int SWIsSpriteInSprite(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP);

int SWIsSpriteInRect(
	SpritePtr srcSpriteP,
	SWRect* testRect);

int SWIsSpriteFullyInRect(
	SpritePtr srcSpriteP,
	SWRect* testRect);

int SWIsPointInSprite(
	SpritePtr srcSpriteP,
	int testPointH, int testPointV);

int SWIsPointInSpriteMask(
	SpritePtr srcSpriteP,
	int testPointH, int testPointV,
	SpriteInMaskProcPtr testProc);

void SWInvalidateSprite(
	SpritePtr srcSpriteP);

void SWTempSpriteMoveProc(
	SpritePtr srcSpriteP);




#ifdef __cplusplus
}
#endif

#endif	/* __SPRITE__ */

