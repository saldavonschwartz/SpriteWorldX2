///--------------------------------------------------------------------------------------
//	Tiling.h
//
//	By: Vern Jensen 10/27/95, Updated 7/05/03 for SWX
///--------------------------------------------------------------------------------------


#ifndef __TILING__
#define __TILING__

#ifdef __cplusplus
extern "C" {
#endif

///--------------------------------------------------------------------------------------
//	Tiling definitions
///--------------------------------------------------------------------------------------

//#define	kMaskIsPartialMask				true
//#define kMaskIsNotPartialMask			false


typedef enum	// Used to SWCheckSpriteWithTiles
{
	kSWEntireSprite = 0,
	kSWTopSide,
	kSWRightSide,
	kSWBottomSide,
	kSWLeftSide
} SWTileSearchType;

#ifndef kSWNumTileLayers
    #define	kSWNumTileLayers		10
#endif

typedef short** TileMapPtr;

typedef struct TileMapStruct
{
	short		numRows;
	short		numCols;
	TileMapPtr	tileMap;
	//short		*tileMapDataP;
	//short		**arrayOfPointersH;
	int		isLocked;
} TileMapStruct, *TileMapStructPtr;

typedef void (*TileChangeProcPtr)(
	SpriteWorldPtr spriteWorldP);

typedef void (*TileRectDrawProcPtr)(
	SpriteWorldPtr	spriteWorldP,
	SWRect*			updateRectP,
	int			optimizingOn);

typedef void (*CustomTileDrawProcPtr)(
	SpriteWorldPtr srcSpriteWorldP,
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect *srcRectP,
	SWRect *dstRectP,
	short tileLayer,
	short tileID,
	short curTileImage,
        int useMask );


///--------------------------------------------------------------------------------------
//	tile function prototypes
///--------------------------------------------------------------------------------------

SWError SWInitTiling(
	SpriteWorldPtr	spriteWorldP,
	short 			tileHeight,
	short			tileWidth,
	short			maxNumTiles);

void SWExitTiling(
	SpriteWorldPtr	spriteWorldP);

//SWError SWDisposeAllTilesFast(
//	SpriteWorldPtr	spriteWorldP);

SWError SWInitTilingCache(
	SpriteWorldPtr	spriteWorldP);

SWError SWCreateTileMap(
	TileMapStructPtr	*tileMapStructPP,
	short 				numTileMapRows,
	short				numTileMapCols);

void SWDisposeTileMap(
	TileMapStructPtr	*tileMapStructPP);

void SWLockTileMap(
	TileMapStructPtr	tileMapStructP);

void SWUnlockTileMap(
	TileMapStructPtr	tileMapStructP);

void SWInstallTileMap(
	SpriteWorldPtr		spriteWorldP,
	TileMapStructPtr	tileMapStructP,
	short				tileLayer);

SWError SWLoadTileMap( const char * file,
	TileMapStructPtr	*tileMapStructPP );

SWError SWSaveTileMap( const char * file,
	TileMapStructPtr	tileMapStructP );

SWError SWResizeTileMap(
	TileMapStructPtr	tileMapStructP,
	short 				numNewTileMapRows,
	short				numNewTileMapCols);

SWError SWLoadTilesFromSingleFileXY( 
	const char* fileName,
	int frameWidth,
	int frameHeight,
	int borderWidth,
	int borderHeight,
	int hasOutsideBorder,
	int		numTiles,
        SpriteWorldPtr	spriteWorldP,
	short		startTileID );
        
SWError SWLoadTilesFromFileSequence(
        const char 	*filename,
        int 		start,
        int		numTiles,
        SpriteWorldPtr	spriteWorldP,
	short		startTileID );


SWError SWCreatePartialTile(
	SpriteWorldPtr	spriteWorldP,
	short			tileID );


void SWDisposeTile(
	SpriteWorldPtr	spriteWorldP,
	short			tileID);

void SWLockTiles(
	SpriteWorldPtr	spriteWorldP);

void SWUnlockTiles(
	SpriteWorldPtr	spriteWorldP);

SWError SWCreateExtraBackFrame(
	SpriteWorldPtr	spriteWorldP,
	SWRect			*frameRect);

void SWDisposeExtraBackFrame(
	SpriteWorldPtr	spriteWorldP);

void SWSetTilingOn(
	SpriteWorldPtr	spriteWorldP,
	int			tilingIsOn);

SWError SWChangeTileSize(
	SpriteWorldPtr	spriteWorldP,
	short 			tileHeight,
	short			tileWidth);

void SWSetSpriteLayerUnderTileLayer(
	SpriteLayerPtr	spriteLayerP,
	short			tileLayer);
/*
SWError SWSetTileMaskDrawProc(
	SpriteWorldPtr	spriteWorldP,
	DrawProcPtr		drawProc);
	
SWError SWSetPartialMaskDrawProc(
	SpriteWorldPtr	spriteWorldP,
	DrawProcPtr		drawProc);
*/

void SWSetCustomTileDrawProc(
	SpriteWorldPtr			spriteWorldP,
	CustomTileDrawProcPtr	customTileDrawProc);

void SWStdCustomTileDrawProc(
	SpriteWorldPtr spriteWorldP,
	FramePtr srcFrameP,
	FramePtr dstFrameP,
	SWRect *srcRectP,
	SWRect *dstRectP,
	short tileLayer,
	short tileID,
	short curTileImage,
        int useMask );

void SWSetTileChangeProc(
	SpriteWorldPtr		spriteWorldP,
	TileChangeProcPtr	tileChangeProc);

SWError SWDrawTilesInBackground(
	SpriteWorldPtr	spriteWorldP);

void SWDrawTile(
	SpriteWorldPtr	spriteWorldP,
	short			dstTileLayer,
	short			tileRow,
	short			tileCol,
	short			tileID);

void SWDrawTilesInRect(
	SpriteWorldPtr	spriteWorldP,
	SWRect*			updateRectP,
	int			faster);

void SWDrawTileLayersInRect(
	SpriteWorldPtr	spriteWorldP,
	SWRect*			updateRectP,
	int			optimizingOn);

void SWDrawTilesAboveSprite(
	SpriteWorldPtr	spriteWorldP,
	SWRect*			updateRectP,
	short			startLayer);

void SWResetTilingCache(
	SpriteWorldPtr	spriteWorldP);

void SWAddChangedRect(
	SpriteWorldPtr	spriteWorldP,
	SWRect			*changedRectP);

void SWChangeTileImage(
	SpriteWorldPtr	spriteWorldP,
	short			tileID,
	short			currentFrame);

void SWUpdateTileOnScreen(
	SpriteWorldPtr	spriteWorldP,
	short			tileID);

void SWUpdateTilesOnScreenFast(
	SpriteWorldPtr	spriteWorldP,
	short			firstTileID,
	short			lastTileID);

void SWResetCurrentTileImages(
	SpriteWorldPtr	spriteWorldP);

short SWReturnTileUnderPixel(
	SpriteWorldPtr	spriteWorldP,
	short	tileLayer,
	short	pixelCol,
	short	pixelRow);

int SWCheckSpriteWithTiles(
	SpriteWorldPtr	spriteWorldP,
	SpritePtr		srcSpriteP,
	SWTileSearchType searchType,
	SWRect			*insetRectP,
	short			startTileLayer,
	short			endTileLayer,
	short			firstTileID,
	short			lastTileID,
	int			fixPosition);

void SWWrapRectToWorkArea(
	SpriteWorldPtr	spriteWorldP,
	SWRect*			dstRectP);

void SWWrapRectFromExtraBackFrame(
	SpriteWorldPtr	spriteWorldP,
	SWRect			*dstRectP);


#ifdef __cplusplus
}
#endif

#endif /* __TILING__ */


