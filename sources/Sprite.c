///--------------------------------------------------------------------------------------
//	Sprite.c
//
//	Portions are copyright: � 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	implementation of the sprites
///--------------------------------------------------------------------------------------

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SPRITEWORLD__
#include <SpriteWorld.h>
#endif

#ifndef __SPRITE__
#include <Sprite.h>
#endif

#ifndef __SPRITEFRAME__
#include <SpriteFrame.h>
#endif


///--------------------------------------------------------------------------------------
//	SWRemoveSpriteFromAnimation
///--------------------------------------------------------------------------------------

void  SWRemoveSpriteFromAnimation(
	SpriteWorldPtr	spriteWorldP,
	SpritePtr		spriteP,
	int			disposeOfSprite)
{
	SW_ASSERT(spriteWorldP != NULL && spriteP != NULL);

	SWSetSpriteVisible( spriteP, false );
	SWRemoveSprite(spriteP);
	SWAddSprite(spriteWorldP->deadSpriteLayerP, spriteP);

	if ( disposeOfSprite )
	{
		spriteP->spriteRemoval = kSWRemoveAndDisposeSprite;
	}
	else
	{
		spriteP->spriteRemoval = kSWRemoveSprite;
	}
}


///--------------------------------------------------------------------------------------
//	SWDisposeSprite
///--------------------------------------------------------------------------------------

void SWDisposeSprite(
	SpritePtr *deadSpritePP)
{
	SpritePtr	deadSpriteP = *deadSpritePP;
	short 			frame;
	int				framesDisposed;

	if (deadSpriteP != NULL)
	{

		for (frame = 0; frame < deadSpriteP->numFrames; frame++)
		{
			framesDisposed = SWDisposeFrame(&deadSpriteP->frameArray[frame]);
		}

		if ( framesDisposed )
		{
			if ( deadSpriteP->sharedSurface != NULL )
			{
				SDL_DestroyTexture(deadSpriteP->sharedSurface);
				deadSpriteP->sharedSurface = NULL;
			}

		}
    
    if (deadSpriteP->drawData) {
      free(deadSpriteP->drawData);
      deadSpriteP->drawData = NULL;
    }
    
    if (deadSpriteP->frameArray) {
      free(deadSpriteP->frameArray);
      deadSpriteP->frameArray = NULL;
    }
    
		free(deadSpriteP);

		*deadSpritePP = NULL;	// Set the original pointer to NULL
	}
}


///--------------------------------------------------------------------------------------
//	SWLockSprite
///--------------------------------------------------------------------------------------

void SWLockSprite(
	SpritePtr srcSpriteP)
{
	register long 		curFrame;

	SW_ASSERT(srcSpriteP != NULL);

	for (curFrame = 0; curFrame < srcSpriteP->numFrames; curFrame++)
	{
		SWLockFrame(srcSpriteP->frameArray[curFrame]);
	}
}


///--------------------------------------------------------------------------------------
//	SWUnlockSprite
///--------------------------------------------------------------------------------------

void SWUnlockSprite(
	SpritePtr srcSpriteP)
{
	register long 		curFrame;

	SW_ASSERT(srcSpriteP != NULL);

	for (curFrame = 0; curFrame < srcSpriteP->numFrames; curFrame++)
	{
		SWUnlockFrame(srcSpriteP->frameArray[curFrame]);
	}

}



///--------------------------------------------------------------------------------------
//	SWAddFrame
///--------------------------------------------------------------------------------------

SWError SWAddFrame(
	SpritePtr srcSpriteP,
	FramePtr newFrameP)
{
	SWError err = kNoError;
	
	SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);

		// don�t exceed maximum number of frames
	if (srcSpriteP->numFrames < srcSpriteP->maxFrames)
	{      
		srcSpriteP->frameArray[srcSpriteP->numFrames] = newFrameP;
		
		if(srcSpriteP->numFrames == 0)
		  srcSpriteP->curFrameP = newFrameP;
		  
			// increment the number of frames
		srcSpriteP->numFrames++;
		newFrameP->useCount++;
	}
	else
	{
		err = kMaxFramesErr;
	}

	SWSetStickyIfError( err );
	return err;
}



///--------------------------------------------------------------------------------------
//	SWInsertFrame
///--------------------------------------------------------------------------------------

SWError SWInsertFrame(
	SpritePtr	srcSpriteP,
	FramePtr	newFrameP,
	long		frameIndex)
{
	register long		index;
	register FramePtr	*frameArray;
	SWError				err = kNoError;

	SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);

	frameArray = srcSpriteP->frameArray;

		// don�t exceed maximum number of frames
	if (frameIndex < srcSpriteP->maxFrames && frameIndex >= 0 &&
		srcSpriteP->numFrames < srcSpriteP->maxFrames)
	{
			// move frames above frameIndex up
		for (index = srcSpriteP->numFrames; index > frameIndex; index--)
			frameArray[index] = frameArray[index - 1];

			// fill the hole
		frameArray[frameIndex] = newFrameP;

		srcSpriteP->numFrames++;
		newFrameP->useCount++;
	}
	else
	{
		err = kMaxFramesErr;
	}

	SWSetStickyIfError( err );
	return err;
}


///--------------------------------------------------------------------------------------
//	SWRemoveFrame
///--------------------------------------------------------------------------------------

void SWRemoveFrame(
	SpritePtr srcSpriteP,
	FramePtr oldFrameP)
{
	register long curFrame;
	register FramePtr *frameArray;

	SW_ASSERT(srcSpriteP != NULL && oldFrameP != NULL);

	curFrame = srcSpriteP->numFrames;
	frameArray = srcSpriteP->frameArray;

		// find the frame to be removed
	while (curFrame--)
	{
		if (frameArray[curFrame] == oldFrameP)
		{
			srcSpriteP->numFrames--;
			oldFrameP->useCount--;

				// move the rest of the frames down
			while (curFrame <= (srcSpriteP->numFrames - 1L))
			{
				frameArray[curFrame] = frameArray[curFrame + 1L];
				curFrame++;
			}

			break;
		}
	}
}


///--------------------------------------------------------------------------------------
//	SWGetNextFrame
///--------------------------------------------------------------------------------------

FramePtr SWGetNextFrame(
	SpritePtr spriteP,
	FramePtr curFrameP)
{
	int			frame;

	SW_ASSERT(spriteP != NULL);

	if ( curFrameP == NULL )
		return spriteP->frameArray[0];
	else
	{
		for ( frame = 0; frame < spriteP->numFrames - 2; frame++ )
		{
			if ( curFrameP == spriteP->frameArray[frame] )
			{
				return spriteP->frameArray[frame+1];
			}
		}
		return NULL;
	}
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	SWSetSpriteDrawProc
///--------------------------------------------------------------------------------------

SWError SWSetSpriteDrawProc(
	SpritePtr srcSpriteP,
	DrawProcPtr drawProc)
{
	SWError		err = kNoError;

	srcSpriteP->frameDrawProc = drawProc;

	return err;
}



///--------------------------------------------------------------------------------------
//	SWStdSpriteDrawProc - for drawing sprites
///--------------------------------------------------------------------------------------

void SWStdSpriteDrawProc(
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect* srcSWRect,
	SWRect* dstSWRect)
{
	SWError err = kNoError;
	SDL_Rect srcSDLRect;
	SDL_Rect dstSDLRect;
  SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
  uint8_t alphaMod = 0;
  int alphaBlend = (
      gSWCurrentElementDrawData != NULL &&
      gSWCurrentElementDrawData->translucencyLevel != 0xFFFF
                    );
  
	SW_CONVERT_SW_TO_SDL_RECT( (*srcSWRect), srcSDLRect );
	SW_CONVERT_SW_TO_SDL_RECT( (*dstSWRect), dstSDLRect );
	
	if (alphaBlend)
	{
    SDL_GetTextureBlendMode(srcFrameP->frameSurfaceP, &blendMode);
    SDL_GetTextureAlphaMod(srcFrameP->frameSurfaceP, &alphaMod);
    SDL_SetTextureBlendMode(srcFrameP->frameSurfaceP, SDL_BLENDMODE_BLEND);
    
    unsigned long alpha = gSWCurrentElementDrawData->translucencyLevel >> 8;
    if (SDL_SetTextureAlphaMod(srcFrameP->frameSurfaceP, alpha) == -1) {
      err = kSDLSetAlpha;
    }
	}
		
	if( err == kNoError )
    SDL_SetRenderTarget(sdl2ctx.renderer, dstFrameP->frameSurfaceP);
    err = SDL_RenderCopy(sdl2ctx.renderer, srcFrameP->frameSurfaceP, &srcSDLRect, &dstSDLRect);
    SDL_SetRenderTarget(sdl2ctx.renderer, NULL);
  
		if(alphaBlend) {
    SDL_SetTextureBlendMode(srcFrameP->frameSurfaceP, blendMode);
    SDL_SetTextureAlphaMod(srcFrameP->frameSurfaceP, alphaMod);
  }

	SWSetStickyIfError( err );
}


///--------------------------------------------------------------------------------------
//  SWTempSpriteMoveProc - this is installed as the Sprite's MoveProc when the Sprite is
//	first created. When this is called, it replaces itself with the real MoveProc. This
//	ensures that the Sprite is drawn in its original position before it is moved by
//	SWProcessSpriteWorld.
///--------------------------------------------------------------------------------------

void SWTempSpriteMoveProc(SpritePtr srcSpriteP)
{
		// Change the moveProc to the real one. This TempSpriteMoveProc
		// will never be called for this Sprite again.
	srcSpriteP->spriteMoveProc = srcSpriteP->realSpriteMoveProc;

		// If the Sprite has already been drawn, call its real MoveProc.
		// (We can go ahead and move it since it has been drawn in its original location.)
	if (srcSpriteP->needsToBeDrawn == false)
	{
		if (srcSpriteP->realSpriteMoveProc != NULL)
			(*srcSpriteP->realSpriteMoveProc)(srcSpriteP);
	}
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	SWSetCurrentFrame
///--------------------------------------------------------------------------------------

SWError SWSetCurrentFrame(
	SpritePtr srcSpriteP,
	FramePtr newFrameP)
{
	short		frameIndex = 0;
	int		foundFrame = false;
	SWError		err = kNoError;

	SW_ASSERT(srcSpriteP != NULL && newFrameP != NULL);

	for (frameIndex = 0; frameIndex < srcSpriteP->numFrames; frameIndex++)
	{
		if (srcSpriteP->frameArray[frameIndex] == newFrameP)
		{
			SWSetCurrentFrameIndex(srcSpriteP, frameIndex);
			foundFrame = true;
			break;
		}
	}

	if (foundFrame == false)
		err = kBadParameterErr;

	SWSetStickyIfError( err );
	return err;
}


///--------------------------------------------------------------------------------------
//	SWSetCurrentFrameIndex
///--------------------------------------------------------------------------------------

SWError SWSetCurrentFrameIndex(
	SpritePtr srcSpriteP,
	long frameIndex)
{
	SWError err = kNoError;
	register FramePtr newFrameP;
	short horizOffset, vertOffset;

	SW_ASSERT(srcSpriteP != NULL);
	SW_ASSERT(frameIndex >= 0);

	if (frameIndex < srcSpriteP->numFrames)
	{
		newFrameP = srcSpriteP->frameArray[frameIndex];

			// If this is a brand new sprite, it won't have a current frame.
			// (This is used when calculating the hotSpot offset from old to new frame.)
		if (srcSpriteP->curFrameP == NULL)
			srcSpriteP->curFrameP = newFrameP;

		horizOffset = (srcSpriteP->destFrameRect.left - newFrameP->frameRect.left) +
					(srcSpriteP->curFrameP->hotSpotH - newFrameP->hotSpotH);
		vertOffset = (srcSpriteP->destFrameRect.top - newFrameP->frameRect.top) +
					(srcSpriteP->curFrameP->hotSpotV - newFrameP->hotSpotV);

		srcSpriteP->curFrameP = newFrameP;
		srcSpriteP->destFrameRect = newFrameP->frameRect;

		if (srcSpriteP->drawData->scaledWidth > 0)
			srcSpriteP->destFrameRect.right = srcSpriteP->destFrameRect.left +
												srcSpriteP->drawData->scaledWidth;
		if (srcSpriteP->drawData->scaledHeight > 0)
			srcSpriteP->destFrameRect.bottom = srcSpriteP->destFrameRect.top +
												srcSpriteP->drawData->scaledHeight;

		srcSpriteP->drawData->frameHotSpotH = srcSpriteP->curFrameP->hotSpotH;
		srcSpriteP->drawData->frameHotSpotV = srcSpriteP->curFrameP->hotSpotV;

		srcSpriteP->destFrameRect.left += horizOffset;
		srcSpriteP->destFrameRect.right += horizOffset;
		srcSpriteP->destFrameRect.top += vertOffset;
		srcSpriteP->destFrameRect.bottom += vertOffset;

		srcSpriteP->curFrameIndex = frameIndex;

		srcSpriteP->needsToBeDrawn = true;
	}
	else
	{
		err = kInvalidFramesIndexErr;
	}
	
	return err;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteCollideProc
///--------------------------------------------------------------------------------------

void SWSetSpriteCollideProc(
	SpritePtr srcSpriteP,
	CollideProcPtr collideProc)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->spriteCollideProc = collideProc;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteColor
///--------------------------------------------------------------------------------------

void SWSetSpriteColor(
	SpritePtr srcSpriteP,
	SDL_Color *color)
{
	srcSpriteP->drawData->color = *color;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteFrameAdvance (default value for new Sprites is 1)
///--------------------------------------------------------------------------------------

void SWSetSpriteFrameAdvance(
	SpritePtr srcSpriteP,
	long frameAdvance)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->frameAdvance = frameAdvance;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteFrameAdvanceMode
///--------------------------------------------------------------------------------------

void SWSetSpriteFrameAdvanceMode(
	SpritePtr srcSpriteP,
	AdvanceType advanceMode)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->frameAdvanceMode = advanceMode;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteFrameRange
///--------------------------------------------------------------------------------------

void SWSetSpriteFrameRange(
	SpritePtr srcSpriteP,
	long firstFrameIndex,
	long lastFrameIndex)
{
	SW_ASSERT(srcSpriteP != NULL);

	if ( firstFrameIndex < 0 )
		firstFrameIndex = 0;

	if ( lastFrameIndex > srcSpriteP->maxFrames - 1 )
		lastFrameIndex = srcSpriteP->maxFrames - 1;

	srcSpriteP->firstFrameIndex = firstFrameIndex;
	srcSpriteP->lastFrameIndex = lastFrameIndex;

		// Make sure the sprite's curFrameIndex is within the new frame range
	if (srcSpriteP->curFrameIndex < firstFrameIndex)
		SWSetCurrentFrameIndex(srcSpriteP, firstFrameIndex);
	else if (srcSpriteP->curFrameIndex > lastFrameIndex)
		SWSetCurrentFrameIndex(srcSpriteP, lastFrameIndex);
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteFrameTime - pass -1 to never change frames (default state for new Sprites),
//	0 to advance as fast as possible, or a millisecond time interval.
///--------------------------------------------------------------------------------------

void SWSetSpriteFrameTime(
	SpritePtr srcSpriteP,
	long timeInterval)
{
	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->frameTimeInterval = timeInterval;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteFrameProc
///--------------------------------------------------------------------------------------

void SWSetSpriteFrameProc(
	SpritePtr srcSpriteP,
	FrameProcPtr frameProc)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->frameChangeProc = frameProc;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteLocation
///--------------------------------------------------------------------------------------

void SWSetSpriteLocation(
	SpritePtr srcSpriteP,
	SWMovementType floatHorizLoc,
	SWMovementType floatVertLoc)
{
	short	horizLoc, vertLoc;

	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->drawData->horizLoc = floatHorizLoc;
	srcSpriteP->drawData->vertLoc = floatVertLoc;

	horizLoc = (short) floatHorizLoc - srcSpriteP->drawData->frameHotSpotH;
	vertLoc = (short) floatVertLoc - srcSpriteP->drawData->frameHotSpotV;

	srcSpriteP->destFrameRect.bottom = vertLoc +
		(srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top);
	srcSpriteP->destFrameRect.right = horizLoc +
		(srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left);
	srcSpriteP->destFrameRect.top = vertLoc;
	srcSpriteP->destFrameRect.left = horizLoc;

	srcSpriteP->deltaFrameRect = srcSpriteP->destFrameRect;
	srcSpriteP->oldFrameRect = srcSpriteP->destFrameRect;

	srcSpriteP->needsToBeDrawn = true;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteMoveBounds
///--------------------------------------------------------------------------------------

void SWSetSpriteMoveBounds(
	SpritePtr srcSpriteP,
	SWRect* moveBoundsRect)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->moveBoundsRect = *moveBoundsRect;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteMoveDelta
///--------------------------------------------------------------------------------------

void SWSetSpriteMoveDelta(
	SpritePtr srcSpriteP,
	SWMovementType horizDelta,
	SWMovementType vertDelta)
{
	SW_ASSERT(srcSpriteP != NULL);

		// set sprite�s velocity
	srcSpriteP->drawData->horizMoveDelta = horizDelta;
	srcSpriteP->drawData->vertMoveDelta = vertDelta;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteMoveTime
///--------------------------------------------------------------------------------------

void SWSetSpriteMoveTime(
	SpritePtr srcSpriteP,
	long timeInterval)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->moveTimeInterval = timeInterval;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteMoveProc
///--------------------------------------------------------------------------------------

void SWSetSpriteMoveProc(
	SpritePtr srcSpriteP,
	MoveProcPtr moveProc)
{
	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->realSpriteMoveProc = moveProc;

		// Set the current MoveProc if it isn't SWTempSpriteMoveProc.
	if (srcSpriteP->spriteMoveProc != SWTempSpriteMoveProc)
		srcSpriteP->spriteMoveProc = moveProc;
}


///--------------------------------------------------------------------------------------
//	SWSetSpritePseudoMoveTime
///--------------------------------------------------------------------------------------

void SWSetSpritePseudoMoveTime(
	SpritePtr srcSpriteP,
	long timeInterval)
{
	SW_ASSERT(srcSpriteP != NULL);

		// Note: a timeInterval is NOT the same as a FPS value!
	srcSpriteP->pseudoMoveTimeInterval = timeInterval;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteVisible
///--------------------------------------------------------------------------------------

void SWSetSpriteVisible(
	SpritePtr srcSpriteP,
	int isVisible)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->isVisible = isVisible;
	srcSpriteP->needsToBeDrawn = true;
	srcSpriteP->needsToBeErased = !isVisible;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteTranslucencyLevel
///--------------------------------------------------------------------------------------

void SWSetSpriteTranslucencyLevel(
	SpritePtr srcSpriteP,
	unsigned long level)
{
	SW_ASSERT(srcSpriteP != NULL);	
	srcSpriteP->drawData->translucencyLevel = level;
}


///--------------------------------------------------------------------------------------
//	SWSetSpriteFastMovingCheck - set whether we need to check this sprite to see
// if we should deal with its oldSWRects and destSWRects separately when animating, or if
// should just use the deltaSWRects.  If no checking is done then deltaSWRects will always be
// used.  Setting this to true overides SWSetGlobalFastMovingSpritesChecks() to false
///--------------------------------------------------------------------------------------

void SWSetSpriteFastMovingCheck(
	SpritePtr srcSpriteP,
	int makeChecks)
{
	SW_ASSERT(srcSpriteP != NULL);
	srcSpriteP->doFastMovingCheck = makeChecks;
}


///--------------------------------------------------------------------------------------
//	SWInvalidateSprite
///--------------------------------------------------------------------------------------

void SWInvalidateSprite(
	SpritePtr srcSpriteP)
{
	srcSpriteP->needsToBeDrawn = true;
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	SWGetSpriteVertLoc
///--------------------------------------------------------------------------------------

SWMovementType SWGetSpriteVertLoc(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	return srcSpriteP->drawData->vertLoc;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteHorizLoc
///--------------------------------------------------------------------------------------

SWMovementType SWGetSpriteHorizLoc(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	return srcSpriteP->drawData->horizLoc;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteWidth
///--------------------------------------------------------------------------------------

short SWGetSpriteWidth(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left;
}


///--------------------------------------------------------------------------------------
//	SWGetSpriteHeight
///--------------------------------------------------------------------------------------

short SWGetSpriteHeight(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top;
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	SWMoveSprite
///--------------------------------------------------------------------------------------

void SWMoveSprite(
	SpritePtr srcSpriteP,
	SWMovementType floatHorizLoc,
	SWMovementType floatVertLoc)
{
	short	horizLoc, vertLoc;

	SW_ASSERT(srcSpriteP != NULL);

	srcSpriteP->drawData->horizLoc = floatHorizLoc;
	srcSpriteP->drawData->vertLoc = floatVertLoc;

	horizLoc = (short) floatHorizLoc - srcSpriteP->drawData->frameHotSpotH;
	vertLoc = (short) floatVertLoc - srcSpriteP->drawData->frameHotSpotV;

	if ((horizLoc != srcSpriteP->destFrameRect.left) ||
		(vertLoc != srcSpriteP->destFrameRect.top) )
	{
		srcSpriteP->destFrameRect.bottom = vertLoc +
			(srcSpriteP->destFrameRect.bottom - srcSpriteP->destFrameRect.top);
		srcSpriteP->destFrameRect.right = horizLoc +
			(srcSpriteP->destFrameRect.right - srcSpriteP->destFrameRect.left);
		srcSpriteP->destFrameRect.top = vertLoc;
		srcSpriteP->destFrameRect.left = horizLoc;

		srcSpriteP->needsToBeDrawn = true;
	}
}


///--------------------------------------------------------------------------------------
//	SWOffsetSprite
///--------------------------------------------------------------------------------------

void SWOffsetSprite(
	SpritePtr srcSpriteP,
	SWMovementType floatHorizOffset,
	SWMovementType floatVertOffset)
{
	SW_ASSERT(srcSpriteP != NULL);

	if ((floatHorizOffset != 0) || (floatVertOffset != 0))
	{
		srcSpriteP->drawData->horizLoc += floatHorizOffset;
		srcSpriteP->drawData->vertLoc += floatVertOffset;

		SWMoveSprite(srcSpriteP, srcSpriteP->drawData->horizLoc, srcSpriteP->drawData->vertLoc);
	}
}


///--------------------------------------------------------------------------------------
//	SWOffsetSpriteBasedOnTime - Use this, along with a maxFPS of 0, to let computers
//  with a lot of horsepower make your animation much smoother than it would be otherwise.
///--------------------------------------------------------------------------------------

void SWOffsetSpriteBasedOnTime(
	SpritePtr theSpriteP,
	SWMovementType horizOffset,
	SWMovementType vertOffset)
{
	SpriteWorldPtr	spriteWorldP;
	double			vertMoveDelta, horizMoveDelta;
	double			frameRate;

	SW_ASSERT(theSpriteP != NULL);

	if (theSpriteP->parentSpriteLayerP != NULL)
		spriteWorldP = theSpriteP->parentSpriteLayerP->parentSpriteWorldP;

	if (spriteWorldP != NULL)
	{
		if (theSpriteP->pseudoMoveTimeInterval > 0)
			frameRate = 1000.0 / theSpriteP->pseudoMoveTimeInterval;
		else
			frameRate = spriteWorldP->pseudoFPS;

		horizMoveDelta = SWConvertOffsetBasedOnTime(spriteWorldP, horizOffset, frameRate);
		vertMoveDelta = SWConvertOffsetBasedOnTime(spriteWorldP, vertOffset, frameRate);

		SWOffsetSprite(theSpriteP, (float)horizMoveDelta, (float)vertMoveDelta);
	}
}


///--------------------------------------------------------------------------------------
//	SWConvertOffsetBasedOnTime - Used for time-based animations.
//	framesPerSecond is the "pseudo" speed of this object.
///--------------------------------------------------------------------------------------

double SWConvertOffsetBasedOnTime(
	SpriteWorldPtr spriteWorldP,
	double theOffset,
	double framesPerSecond)
{
	double	timePassed = spriteWorldP->microsecsSinceLastFrame;

	return (theOffset * timePassed * framesPerSecond) * (1.0 / 1000000.0);
}


///--------------------------------------------------------------------------------------
//	SWResetSpriteFrameTimer - ensures that the full frameTime delay passes between the
//  time that this function is called, and the Sprite's frameProc is called again.
///--------------------------------------------------------------------------------------

void SWResetSpriteFrameTimer(
	SpriteWorldPtr spriteWorldP,
	SpritePtr srcSpriteP)
{
	srcSpriteP->timeOfLastFrameChange = spriteWorldP->runningTimeCount;
}


///--------------------------------------------------------------------------------------
//	SWHasMoveProcFired
///--------------------------------------------------------------------------------------

int SWHasMoveProcFired(
	SpritePtr srcSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);

	return srcSpriteP->moveProcHasFired;
}


///--------------------------------------------------------------------------------------
//	SWBounceSprite
///--------------------------------------------------------------------------------------

int SWBounceSprite(
	SpritePtr srcSpriteP)
{
	short	horizOffset, vertOffset;

	SW_ASSERT(srcSpriteP != NULL);

		// Hit left side or right side
	if (srcSpriteP->destFrameRect.left < srcSpriteP->moveBoundsRect.left &&
		(srcSpriteP->drawData->horizMoveDelta < 0))
	{
		srcSpriteP->drawData->horizMoveDelta = -srcSpriteP->drawData->horizMoveDelta;
		horizOffset = (srcSpriteP->moveBoundsRect.left - srcSpriteP->destFrameRect.left) * 2;
	}
	else if (srcSpriteP->destFrameRect.right > srcSpriteP->moveBoundsRect.right &&
			 (srcSpriteP->drawData->horizMoveDelta > 0))
	{
		srcSpriteP->drawData->horizMoveDelta = -srcSpriteP->drawData->horizMoveDelta;
		horizOffset = (srcSpriteP->moveBoundsRect.right - srcSpriteP->destFrameRect.right) * 2;
	}
	else
		horizOffset = 0;


		// Hit top or bottom
	if (srcSpriteP->destFrameRect.top < srcSpriteP->moveBoundsRect.top &&
		(srcSpriteP->drawData->vertMoveDelta < 0))
	{
		srcSpriteP->drawData->vertMoveDelta = -srcSpriteP->drawData->vertMoveDelta;
		vertOffset = (srcSpriteP->moveBoundsRect.top - srcSpriteP->destFrameRect.top) * 2;
	}
	else if (srcSpriteP->destFrameRect.bottom > srcSpriteP->moveBoundsRect.bottom &&
		  	 (srcSpriteP->drawData->vertMoveDelta > 0))
	{
		srcSpriteP->drawData->vertMoveDelta = -srcSpriteP->drawData->vertMoveDelta;
		vertOffset = (srcSpriteP->moveBoundsRect.bottom - srcSpriteP->destFrameRect.bottom) * 2;
	}
	else
		vertOffset = 0;


		// Bounce the sprite if it had a collision with the wall
	if ((horizOffset != 0) || (vertOffset != 0))
	{
		SWOffsetSprite(srcSpriteP, horizOffset, vertOffset);
		return true;
	}
	else
	{
		return false;
	}
}


///--------------------------------------------------------------------------------------
//	SWWrapSprite
///--------------------------------------------------------------------------------------

int SWWrapSprite(
	SpritePtr srcSpriteP)
{
	SWMovementType	horizOffset, vertOffset;

	SW_ASSERT(srcSpriteP != NULL);

		// Wrap to left or right
	if (srcSpriteP->oldFrameRect.left > srcSpriteP->moveBoundsRect.right &&
		srcSpriteP->destFrameRect.left > srcSpriteP->oldFrameRect.left)
	{
		horizOffset = - ((srcSpriteP->moveBoundsRect.right -
						srcSpriteP->moveBoundsRect.left) +
						(srcSpriteP->destFrameRect.right -
						srcSpriteP->destFrameRect.left) + srcSpriteP->drawData->horizMoveDelta);
	}
	else if (srcSpriteP->oldFrameRect.right < srcSpriteP->moveBoundsRect.left &&
			 srcSpriteP->destFrameRect.left < srcSpriteP->oldFrameRect.left)
	{
		horizOffset = (srcSpriteP->moveBoundsRect.right -
						srcSpriteP->moveBoundsRect.left) +
						(srcSpriteP->destFrameRect.right -
						srcSpriteP->destFrameRect.left) - srcSpriteP->drawData->horizMoveDelta;
	}
	else
		horizOffset = 0;


		// Wrap to top or bottom
	if (srcSpriteP->oldFrameRect.top > srcSpriteP->moveBoundsRect.bottom &&
		srcSpriteP->destFrameRect.top > srcSpriteP->oldFrameRect.top)
	{
		vertOffset = -((srcSpriteP->moveBoundsRect.bottom -
							srcSpriteP->moveBoundsRect.top) +
							(srcSpriteP->destFrameRect.bottom -
							srcSpriteP->destFrameRect.top) + srcSpriteP->drawData->vertMoveDelta);
	}
	else if (srcSpriteP->oldFrameRect.bottom < srcSpriteP->moveBoundsRect.top &&
			 srcSpriteP->destFrameRect.top < srcSpriteP->oldFrameRect.top)
	{
		vertOffset = (srcSpriteP->moveBoundsRect.bottom -
							srcSpriteP->moveBoundsRect.top) +
							(srcSpriteP->destFrameRect.bottom -
							srcSpriteP->destFrameRect.top) - srcSpriteP->drawData->vertMoveDelta;
	}
	else
		vertOffset = 0;


		// Wrap sprite, and set delta and old rects to current rect
	if ((horizOffset != 0) || (vertOffset != 0))
	{
		SWOffsetSprite(srcSpriteP, horizOffset, vertOffset);

		srcSpriteP->deltaFrameRect = srcSpriteP->destFrameRect;
		srcSpriteP->oldFrameRect = srcSpriteP->destFrameRect;

		return true;
	}
	else
	{
		return false;
	}
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	SWInPixelMask - check whether a point is within the sprite mask
///--------------------------------------------------------------------------------------

int SWInPixelMask(
	SpritePtr spriteP,
	int testPointH, int testPointV)
{
	/*
 	SW_ASSERT(spriteP != NULL);
 	SW_ASSERT(spriteP->curFrameP != NULL);
 	SW_ASSERT(spriteP->curFrameP->isFrameLocked);

		// sprites without maskPort's will not be checked
 	if (spriteP->curFrameP->maskPort == NULL)
 		return false;

	if ( testPoint.h >= spriteP->destFrameRect.left &&
		 testPoint.v >= spriteP->destFrameRect.top &&
		 testPoint.h < spriteP->destFrameRect.right &&
		 testPoint.v < spriteP->destFrameRect.bottom )
	{
		return BlitPixieMaskTestProc( spriteP,
			testPoint.h - spriteP->destFrameRect.left + spriteP->curFrameP->frameRect.left,
			testPoint.v - spriteP->destFrameRect.top + spriteP->curFrameP->frameRect.top);
	}
	else
		return false;
	*/
	return false;
}


#pragma mark -

///--------------------------------------------------------------------------------------
//	SWRadiusCollision
///--------------------------------------------------------------------------------------

int SWRadiusCollision(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP)
{
	int		 		centerOneH,centerOneV , centerTwoH,centerTwoV ;
	register short		radiusOne, radiusTwo;
	register short		horizDistance, vertDistance;

	SW_ASSERT(srcSpriteP != NULL && dstSpriteP != NULL);

		// get radii - - assumes circular sprites!
	radiusOne = (srcSpriteP->destFrameRect.right-srcSpriteP->destFrameRect.left)/2;
	radiusTwo = (dstSpriteP->destFrameRect.right-dstSpriteP->destFrameRect.left)/2;

		// find centers
	centerOneH = srcSpriteP->destFrameRect.left + radiusOne;
	centerOneV = srcSpriteP->destFrameRect.top + radiusOne;
	centerTwoH = dstSpriteP->destFrameRect.left + radiusTwo;
	centerTwoV = dstSpriteP->destFrameRect.top + radiusTwo;

	horizDistance = centerOneH - centerTwoH;
	vertDistance = centerOneV - centerTwoV;

	return ( (horizDistance*horizDistance) + (vertDistance*vertDistance) <
		((radiusOne+radiusTwo)*(radiusOne+radiusTwo)) );
}


///--------------------------------------------------------------------------------------
//	SWPixelCollision
///--------------------------------------------------------------------------------------

int SWPixelCollision(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP)
{
//todo ben
/*
	int			collision;
	SWRect			sectSWRect;
	SWRect			srcSectSWRect;
	SWRect			dstSectSWRect;
	FramePtr		srcFrameP,
					dstFrameP;

	SW_ASSERT(srcSpriteP != NULL);
	SW_ASSERT(dstSpriteP != NULL);
 	SW_ASSERT(srcSpriteP->curFrameP->isFrameLocked);
 	SW_ASSERT(dstSpriteP->curFrameP->isFrameLocked);
	SW_ASSERT(dstSpriteP->curFrameP->frameDepth >= 8);
 	SW_ASSERT(srcSpriteP->curFrameP->frameDepth == dstSpriteP->curFrameP->frameDepth);

		// sprites without maskPort's will not be checked
 	if (srcSpriteP->curFrameP->maskPort == NULL || dstSpriteP->curFrameP->maskPort == NULL)
 		return false;

		// calculate intersection rect in spriteworld
	sectSWRect.left = SW_MAX(srcSpriteP->destFrameRect.left, dstSpriteP->destFrameRect.left);
	sectSWRect.top = SW_MAX(srcSpriteP->destFrameRect.top, dstSpriteP->destFrameRect.top);
	sectSWRect.right = SW_MIN(srcSpriteP->destFrameRect.right, dstSpriteP->destFrameRect.right);
	sectSWRect.bottom = SW_MIN(srcSpriteP->destFrameRect.bottom, dstSpriteP->destFrameRect.bottom);

	if ( sectSWRect.right > sectSWRect.left && sectSWRect.bottom > sectSWRect.top )
	{
		srcFrameP = srcSpriteP->curFrameP;
		dstFrameP = dstSpriteP->curFrameP;

			// calculate sectSWRect in source frame
		srcSectSWRect.left = srcFrameP->frameRect.left + (sectSWRect.left-srcSpriteP->destFrameRect.left);
		srcSectSWRect.top = srcFrameP->frameRect.top + (sectSWRect.top-srcSpriteP->destFrameRect.top);
		srcSectSWRect.right = srcFrameP->frameRect.left + (sectSWRect.right-srcSpriteP->destFrameRect.left);
		srcSectSWRect.bottom = srcFrameP->frameRect.top +(sectSWRect.bottom-srcSpriteP->destFrameRect.top);

			// calculate sectSWRect in dest frame
		dstSectSWRect.left = dstFrameP->frameRect.left + (sectSWRect.left-dstSpriteP->destFrameRect.left);
		dstSectSWRect.top = dstFrameP->frameRect.top +(sectSWRect.top-dstSpriteP->destFrameRect.top);
		dstSectSWRect.right = dstFrameP->frameRect.left + (sectSWRect.right-dstSpriteP->destFrameRect.left);
		dstSectSWRect.bottom = dstFrameP->frameRect.top +(sectSWRect.bottom-dstSpriteP->destFrameRect.top);

		if ( (srcSpriteP->drawData->flippedHorizontal || srcSpriteP->drawData->flippedVertical) ||
			 (dstSpriteP->drawData->flippedHorizontal || dstSpriteP->drawData->flippedVertical) )
		{
			collision = BlitPixieFlipMaskCollisionProc(srcSpriteP, dstSpriteP, &srcSectSWRect, &dstSectSWRect );
		}
		else
		{
			collision = BlitPixieMaskCollisionProc(srcSpriteP, dstSpriteP, &srcSectSWRect, &dstSectSWRect );
		}
	}
	else
		collision = false;

	return collision;
*/
return false;
}


#pragma mark -

///--------------------------------------------------------------------------------------
//	SWIsSpriteInSprite - do the sprites' collisionSWRects overlap?
///--------------------------------------------------------------------------------------

int SWIsSpriteInSprite(
	SpritePtr srcSpriteP,
	SpritePtr dstSpriteP)
{
	SW_ASSERT(srcSpriteP != NULL);
	SW_ASSERT(dstSpriteP != NULL);

	return ((SW_COLLISION_RECT_TOP(srcSpriteP) < SW_COLLISION_RECT_BOTTOM(dstSpriteP)) &&
			(SW_COLLISION_RECT_BOTTOM(srcSpriteP) > SW_COLLISION_RECT_TOP(dstSpriteP)) &&
			(SW_COLLISION_RECT_LEFT(srcSpriteP) < SW_COLLISION_RECT_RIGHT(dstSpriteP)) &&
			(SW_COLLISION_RECT_RIGHT(srcSpriteP) > SW_COLLISION_RECT_LEFT(dstSpriteP)) );
}


///--------------------------------------------------------------------------------------
//	SWIsSpriteInRect
///--------------------------------------------------------------------------------------

int SWIsSpriteInRect(
	SpritePtr srcSpriteP,
	SWRect* testSWRect)
{
	SW_ASSERT(srcSpriteP != NULL);

	return ((SW_COLLISION_RECT_TOP(srcSpriteP) < testSWRect->bottom) &&
			(SW_COLLISION_RECT_BOTTOM(srcSpriteP) > testSWRect->top) &&
		 	(SW_COLLISION_RECT_LEFT(srcSpriteP) < testSWRect->right) &&
		 	(SW_COLLISION_RECT_RIGHT(srcSpriteP) > testSWRect->left));
}


///--------------------------------------------------------------------------------------
//	SWIsSpriteFullyInRect
///--------------------------------------------------------------------------------------

int SWIsSpriteFullyInRect(
	SpritePtr srcSpriteP,
	SWRect* testSWRect)
{
	SW_ASSERT(srcSpriteP != NULL);

	return ((SW_COLLISION_RECT_TOP(srcSpriteP) >= testSWRect->top) &&
			(SW_COLLISION_RECT_BOTTOM(srcSpriteP) <= testSWRect->bottom) &&
		 	(SW_COLLISION_RECT_LEFT(srcSpriteP) >= testSWRect->left) &&
		 	(SW_COLLISION_RECT_RIGHT(srcSpriteP) <= testSWRect->right));
}


///--------------------------------------------------------------------------------------
//	SWIsPointInSprite -- checks whether a point is in the sprite's "collision rect"
///--------------------------------------------------------------------------------------

int SWIsPointInSprite(
	SpritePtr srcSpriteP,
	int testPointH, int testPointV)
{
	SW_ASSERT(srcSpriteP != NULL);

	return	(testPointH >= SW_COLLISION_RECT_LEFT(srcSpriteP)) &&
			(testPointH < SW_COLLISION_RECT_RIGHT(srcSpriteP)) &&
			(testPointV >= SW_COLLISION_RECT_TOP(srcSpriteP)) &&
			(testPointV < SW_COLLISION_RECT_BOTTOM(srcSpriteP));
}


///--------------------------------------------------------------------------------------
//	SWIsPointInSpriteMask -- checks whether a point is in the sprite's actual mask
///--------------------------------------------------------------------------------------

int SWIsPointInSpriteMask(
	SpritePtr srcSpriteP,
	int testPointH, int testPointV,
	SpriteInMaskProcPtr testProc)
{
	SW_ASSERT(srcSpriteP != NULL);

	if (testProc == NULL)
		return SWIsPointInSprite(srcSpriteP, testPointH, testPointV);
	else
		return (*testProc)(srcSpriteP, testPointH, testPointV);
}
