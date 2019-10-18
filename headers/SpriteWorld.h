///--------------------------------------------------------------------------------------
//	SpriteWorld.h
//
//	Portions are copyright: © 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	constants, structures, and prototypes for sprite worlds
///--------------------------------------------------------------------------------------


#ifndef __SPRITEWORLD__
#define __SPRITEWORLD__

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

#ifndef __SPRITELAYER__
#include <SpriteLayer.h>
#endif

#ifndef __SPRITEFRAME__
#include <SpriteFrame.h>
#endif

#ifndef __SPRITE__
#include <Sprite.h>
#endif


#ifndef __TILING__
#include <Tiling.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

  // 0xfede:
#define LOG_SDL_ERROR() \
  fprintf(stderr, "SDL error: %s\n", SDL_GetError()); \
  SDL_ClearError()

#define SDL2_DATA(x) \
  (x)? &(x)->sdl2 : &gSWCurrentSpriteWorld->sdl2
  

  
///--------------------------------------------------------------------------------------
//	data structures
///--------------------------------------------------------------------------------------

typedef struct UpdateRectStruct UpdateRectStruct;
typedef UpdateRectStruct *UpdateRectStructPtr;

struct UpdateRectStruct
{
	SWRect 				updateRect;			// this SWRect
	UpdateRectStructPtr nextRectStructP;	// next SWRect struct
};

///--------------------------------------------------------------------------------------
//	sprite world type definitions
///--------------------------------------------------------------------------------------

typedef void (*WorldMoveProcPtr)(
	SpriteWorldPtr spriteWorldP,
	SpritePtr followSpriteP);

typedef void (*CallBackPtr)(
	SpriteWorldPtr spriteWorldP);

typedef void (*CleanUpCallBackPtr)( void );

///--------------------------------------------------------------------------------------
//	sprite world data structure
///--------------------------------------------------------------------------------------

// 0xfede: SDL2 new stuff:
  
struct SpriteWorldRec
{
	SpriteLayerPtr headSpriteLayerP;	// head of the sprite layer linked list
	SpriteLayerPtr tailSpriteLayerP;	// tail of the sprite layer linked list
	SpriteLayerPtr deadSpriteLayerP;    // where SWRemoveSpriteFromAnimation puts Sprites
	
	UpdateRectStructPtr headUpdateRectP; // used by SWFlagRectAsChanged
	UpdateRectStructPtr secondBufHeadUpdateRectP; // used by SWFlagRectAsChanged
	int	useUpdateRectOptimizations; //should try to merge update rects 

	FramePtr extraBackFrameP;			// used when adding a background behind tiles
	FramePtr backFrameP;				// frame for the background
	FramePtr workFrameP;				// work, or "mixing" frame
	FramePtr screenFrameP;				// frame for drawing to the screen
	
	int	useHWDoubleBuf;

	DrawProcPtr offscreenDrawProc;		// callback for erasing sprites offscreen normally
	DrawProcPtr screenDrawProc;			// callback for drawing sprite pieces onscreen normally

	CallBackPtr postEraseCallBack;		// called after erasing sprites
	CallBackPtr postDrawCallBack;		// called after drawing sprites
	
	SWRect	screenRect;					// holds windowFrameP->frameRect for easier access
	SWRect	worldRect;					// size and position of actual sprite world
	SWRect	backRect;					// holds backFrameP->frameRect for easier access

	SWRect	visScrollRect;				// SWRect that is copied to screen when scrolling
	SWRect	oldVisScrollRect;			// visScrollRect from last frame
	SWRect	offscreenScrollRect;		// same as visScrollRect, but local to offscreen
	short	horizScrollRectOffset;		// offset from offscreenScrollRect to visScrollRect
	short	vertScrollRectOffset;		// offset from offscreenScrollRect to visScrollRect
	short	horizScrollDelta;			// horizontal scrolling delta
	short	vertScrollDelta;			// vertical scrolling delta
	SWRect	scrollRectMoveBounds;		// move bounds for visScrollRect

	WorldMoveProcPtr worldMoveProc;		// pointer to the scrolling world move procedure
	SpritePtr followSpriteP;			// pointer to the "follow sprite", or NULL
	
        
        
        TileMapStructPtr *tileLayerArray; 	// an array of all the tileMap layers
	int		tilingIsInitialized;	// has the tiling been initialized yet?
	int		tilingIsOn;				// are the tiling routines turned on?
	short		lastActiveTileLayer;	// the last active tile layer
	short		**tilingCache;			// two-dimensional tiling cache
	short		numTilingCacheRows;		// number of rows in tilingCache array
	short		numTilingCacheCols;		// number of cols in tilingCache array
	FramePtr	*tileFrameArray;		// array of tile framePtrs (the part below sprites)
	//FramePtr	*tilePartialFrameArray;	// array of tile framePtrs (the part above sprites)
	short		*curTileImage;			// array specifying the current frame of each tile
	short		maxNumTiles;			// number of elements in tileFrameArray
	short		tileWidth;				// width of each tile
	short		tileHeight;				// height of each tile
	long		numTilesChanged;		// number of rects in changedTiles array to update
	SWRect		*changedTiles;			// array of rects of tiles that changed
	long		changedTilesArraySize;	// number of elements in changedTiles array
	TileChangeProcPtr tileChangeProc;	// pointer to tile frame changing procedure
	TileRectDrawProcPtr tileRectDrawProc; // pointer to the function that draws tiles in a rect
	DrawProcPtr tileMaskDrawProc;		// drawProc for drawing masked tiles in tile layers
	//DrawProcPtr tileMaskHardwareDrawProc;		// drawProc for drawing masked tiles in tile layers in hardware
	//DrawProcPtr partialMaskDrawProc;	// drawProc for drawing partialMask tiles above sprites
	//DrawProcPtr partialMaskHardwareDrawProc;	// drawProc for drawing partialMask tiles above sprites in hardware
	CustomTileDrawProcPtr customTileDrawProc;	// DrawProc that is also passed the tileID and tileLayer
        
	
	unsigned char	pixelDepth;				// SpriteWorld's depth

	short 			fpsTimeInterval;	// milliseconds per frame of animation (1000/fps)
	double			pseudoFPS; 			// milliseconds per pseudo frame of animation (for time-based animations)
	unsigned long	runningTimeCount;	// running total time in milliseconds
	unsigned long 	timeOfLastFrame;	// time (from runningTimeCount) of last frame
	
			// For time-based animation
	unsigned long	minimumFrameRate;	// maximum number of microseconds that can go by in a frame (used only for time-based movement)
	unsigned long	microsecsSinceLastFrame; // # microseconds passed since last frame
	//UnsignedWide 	lastMicrosecondsB;	// used to compute the microsecsSinceLastFrame
	double			pseudoFrameAccum;	// used to set the pseudoFrameHasFired variable.
	int				pseudoFrameHasFired;// used in time-based animations.

	int		 		frameHasOccurred;	// Has the SpriteWorld been processed?
	int				thereAreNonScrollingLayers; // used internally by SWProcessSpriteWorld

	int				makeTranslucencyAdjustments;
	int				doFastMovingSpritesChecks;

	SDL_Color		backgroundColor;	// background color for clear drawproc
	
	
	long userData;						// reserved for user
};

  // 0xfede: SDL2 stuff
  
  typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    int fullscreenFlag;
    SDL_DisplayMode* dmodes;
    int dmodeIdx;
    int dmodeCount;
    uint32_t cmask[4];
    Uint32 (*wflags)(void);
    void(*show)(SDL_Texture* tx);
  } SDL2Context;
  
///--------------------------------------------------------------------------------------
//	sprite world globals
///--------------------------------------------------------------------------------------

// 0xfede: SDL2 stuff

extern SDL2Context sdl2ctx;
  
extern DrawDataPtr		gSWCurrentElementDrawData;
extern SpriteWorldPtr		gSWCurrentSpriteWorld;

extern CleanUpCallBackPtr	gSWCleanUpCallBackP;
extern SpriteWorldPtr		gSWCleanUpSpriteWorldP;

///--------------------------------------------------------------------------------------
//	sprite world function prototypes
///--------------------------------------------------------------------------------------


SWError SWEnterSpriteWorld(int shouldInitSDL);

SWError SWCreateSpriteWorld(
	SpriteWorldPtr	*spriteWorldPP,
	int				w,
	int				h,
	int				depth,
	int				makeFullScreen);

SWError SWCreateSpriteWorldFromVideoSurface(
	SpriteWorldPtr		*spriteWorldPP,
	SDL_Texture			*videoSurfaceP,
	SWRect				*worldRectP,
	SWRect				*backRectP,
	int					maxDepth );

SWError SWCreateSpriteWorldFromFrames(
	SpriteWorldPtr		*spriteWorldP,
	FramePtr			screenFrameP,
	FramePtr			backFrameP,
	FramePtr			workFrameP );

void SWDisposeSpriteWorld(
	SpriteWorldPtr	*spriteWorldPP);
	
void SWExitSpriteWorld(void);

void SWAddSpriteLayer(
	SpriteWorldPtr		spriteWorldP,
	SpriteLayerPtr		newSpriteLayerP);

void SWRemoveSpriteLayer(
	SpriteWorldPtr	spriteWorldP,
	SpriteLayerPtr	oldSpriteLayerP);

void SWSwapSpriteLayer(
	SpriteWorldPtr	spriteWorldP,
	SpriteLayerPtr	srcSpriteLayerP,
	SpriteLayerPtr	dstSpriteLayerP);

SpriteLayerPtr SWGetNextSpriteLayer(
	SpriteWorldPtr	spriteWorldP,
	SpriteLayerPtr	curSpriteLayerP);

DrawDataPtr SWCreateDrawData (void);

void SWLockSpriteWorld(
	SpriteWorldPtr	spriteWorldP);

void SWUnlockSpriteWorld(
	SpriteWorldPtr	spriteWorldP);

void SWSetPostEraseCallBack(
	SpriteWorldPtr	spriteWorldP,
	CallBackPtr		callBack);

void SWSetPostDrawCallBack(
	SpriteWorldPtr	spriteWorldP,
	CallBackPtr		callBack);

void SWSetSpriteWorldMaxFPS(
	SpriteWorldPtr spriteWorldP,
	short framesPerSec);

void SWSetSpriteWorldPseudoFPS(
	SpriteWorldPtr spriteWorldP,
	short framesPerSec);

void SWSetMinimumFrameRate(
	SpriteWorldPtr spriteWorldP,
	long minimumFrameRate);

void SWSetBackgroundColor(
	SpriteWorldPtr spriteWorldP,
	SDL_Color color);

void SWSetTranslucencyAdjustments(
	SpriteWorldPtr spriteWorldP,
	int makeAdjustments);

void SWStdOffscreenDrawProc(
	FramePtr	srcFrameP,
	FramePtr	dstFrameP,
	SWRect*		srcRect,
	SWRect*		dstRect);
	
void SWStdScreenDrawProc(
	FramePtr	srcFrameP,
	FramePtr	dstFrameP,
	SWRect*		srcRect,
	SWRect*		dstRect);

void SWCopyBackgroundToWorkArea(
	SpriteWorldPtr spriteWorldP);

void SWUpdateSpriteWorld(
	SpriteWorldPtr spriteWorldP,
	int updateWindow);

void SWProcessSpriteWorld(
	SpriteWorldPtr spriteWorldP);

void SWProcessSpriteLayer(
	SpriteWorldPtr spriteWorldP,
	SpriteLayerPtr curSpriteLayerP);

void SWProcessNonScrollingLayers(
	SpriteWorldPtr spriteWorldP);

void SWAnimateSpriteWorld(
	SpriteWorldPtr spriteWorldP);

void SWCheckIdleSpriteOverlap(
	SpriteWorldPtr	spriteWorldP,
	SpritePtr		idleSpriteP,
	SpritePtr		headActiveSpriteP);

void SWCollectIdleSpriteIntersectingRects(
	SWRect checkRect1,
	SWRect checkRect2,
	SWRect *enclosingRect);

void SWFindSpritesToBeRemoved(
	SpriteWorldPtr spriteWorldP );

SWError SWFlagRectAsChanged(
	SpriteWorldPtr spriteWorldP,
	SWRect*	theChangedRect);

void SWMergeUpdateRect(
	UpdateRectStructPtr *headRectPP,
	SWRect*	theChangedRect,
	int *countP);

void SWRemoveUpdateRectStruct(
	UpdateRectStructPtr* headRectPP,
	UpdateRectStructPtr	removeRect);

void SWDisposeUpdateRectList(
	UpdateRectStructPtr *headRectPP );

void SWResetMovementTimer(
	SpriteWorldPtr spriteWorldP);

unsigned long SWGetMilliseconds(SpriteWorldPtr spriteWorldP);

void SWClearStickyError(void);
SWError SWStickyError(void);
void SWSetStickyIfError(SWError errNum);
void SWAssertFail(char *filename, char *functionName, int lineNum);

#ifdef __cplusplus
}
#endif

#endif /* __SPRITEWORLD__ */

