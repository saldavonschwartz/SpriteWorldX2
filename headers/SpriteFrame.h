///--------------------------------------------------------------------------------------
//	SpriteFrame.h
//
//	Portions are copyright: � 1991-94 Tony Myles, All rights reserved worldwide.
///--------------------------------------------------------------------------------------


#ifndef __SPRITEFRAME__
#define __SPRITEFRAME__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef _SDL_H
#include <SDL2/SDL.h>
#endif


#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef _IMG_h
#include <SDL2_image/SDL_image.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef enum MaskType
{
    kNoMask 			= 0,
    kAlphaChannelMask,			// mask has alpha grayscale [in alphaPort]
    kColorKeyMask
    
} MaskType;

///--------------------------------------------------------------------------------------
//	frame data structure
///--------------------------------------------------------------------------------------

struct FrameRec
{
	SDL_Texture	*frameSurfaceP;			// pointer to screen optimized SDL_Texture for the frame
	SDL_Texture	*originalSurfaceP;		// SDL_Texture before optimizations
	int		isVideoSurface;			// is the frame's surface the SDL video surface (mapped directly to screen)
	int		sharesSurface;			//should we free the surface when disposing the frame?
	
	int 		isFrameLocked;	  		// has the frame been locked?
	MaskType	maskType;			// frame mask type
        //int		hasAlpha;
        
	SWRect 		frameRect;			// source image rectangle
	int		hotSpotH;			// horizontal hot point for this frame
	int		hotSpotV;			// vertical hot point for this frame
	SWRect		collisionInset;			// if used, makes the collision rect smaller/large for this frame
	
	unsigned short	useCount;			// number of sprites using this frame
	
	long		userData;			// reserved for user
};



SWError SWCreateFrame (FramePtr* newFrameP);
int SWDisposeFrame(FramePtr *oldFramePP);
SWError SWCreateFrameFromSurfaceAndRect(FramePtr* newFrameP,
	SDL_Texture *surface,SWRect* frameRect );
SWError SWCreateFrameFromSurface (FramePtr* newFrameP, SDL_Texture *theSurface, int isVideoSurface );
SWError SWCreateBlankFrame (
	FramePtr* newFrameP,
	int w, int h, Uint8 depth, int createAlphaChannel );
SWError SWCreateFrameFromFile (FramePtr* newFrameP, const char * filename);

void SWLockFrame(FramePtr srcFrameP);
void SWUnlockFrame(FramePtr srcFrameP);
SWError SWUpdateFrame ( FramePtr srcFrameP );

#ifdef __cplusplus
}
#endif

#endif	/* __SPRITEFRAME__ */

