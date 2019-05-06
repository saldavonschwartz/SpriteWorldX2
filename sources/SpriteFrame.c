///--------------------------------------------------------------------------------------
//	SpriteFrame.c
//
//	Portions are copyright: � 1991-94 Tony Myles, All rights reserved worldwide.
//
//	Description:	implementation of the frame stuff
///--------------------------------------------------------------------------------------


#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef __SPRITEFRAME__
#include <SpriteFrame.h>
#endif

#include <SpriteWorld.h>

///--------------------------------------------------------------------------------------
//	SWCreateFrame
///--------------------------------------------------------------------------------------

SWError SWCreateFrame (
                       FramePtr* newFrameP )
{
  SWError err = kNoError;
  FramePtr tempFrameP = 0;
  
  tempFrameP = calloc(1,sizeof(FrameRec));
  
  if (!tempFrameP)
    err = kMemoryAllocationError;
  
  if (err == kNoError)
  {
    tempFrameP->frameSurfaceP = NULL;
    tempFrameP->originalSurfaceP = NULL;
    
    *newFrameP = tempFrameP;
  }
  else
  {
    if ( tempFrameP )
    {
      free( tempFrameP );
    }
  }
  
  return err;
}

///--------------------------------------------------------------------------------------
//	SWCreateFrameFromSurface
///--------------------------------------------------------------------------------------

SWError SWCreateFrameFromSurface (
                                  FramePtr* newFrameP,
                                  SDL_Surface *surfaceP,
                                  int isVideoSurface )
{
  SWError err = kNoError;
  FramePtr tempFrameP = 0;
  
  
  err = SWCreateFrame( &tempFrameP );
  
  if (err == kNoError)
  {
    tempFrameP->frameSurfaceP = surfaceP;
    tempFrameP->isVideoSurface = isVideoSurface;
    
    err = SWUpdateFrame( tempFrameP );
  }
  
  if (err == kNoError)
  {
    *newFrameP = tempFrameP;
  }
  else
  {
    if ( tempFrameP )
    {
      free( tempFrameP );
    }
  }
  
  return err;
}

///--------------------------------------------------------------------------------------
//	SWCreateFrameFromSurfaceAndRect
///--------------------------------------------------------------------------------------

SWError SWCreateFrameFromSurfaceAndRect(
                                        FramePtr* newFrameP,
                                        SDL_Surface *surfaceP,
                                        SWRect* frameRectP )
{
  SWError		err = kNoError;
  FramePtr 	tempFrameP;
  
  SW_ASSERT( surfaceP != 0 );
  SW_ASSERT( frameRectP->top >= 0 && frameRectP->left >= 0 &&
            frameRectP->right > frameRectP->left && frameRectP->bottom > frameRectP->top);
  
  tempFrameP = NULL;
  *newFrameP = NULL;
  
  err = SWCreateFrame( &tempFrameP );
  
  if( err == kNoError )
  {
    tempFrameP->frameSurfaceP = surfaceP;
    tempFrameP->frameRect = *frameRectP;
    tempFrameP->sharesSurface = true;
    
    err = SWUpdateFrame( tempFrameP );
  }
  
  if( err == kNoError )
  {
    *newFrameP = tempFrameP;
  }
  else
  {
    if ( tempFrameP )
    {
      free( tempFrameP );
    }
  }
  
  return err;
}

///--------------------------------------------------------------------------------------
//	SWCreateBlankFrame
///--------------------------------------------------------------------------------------

SWError SWCreateBlankFrame (
                            FramePtr* newFrameP,
                            int w, int h, unsigned char depth, int createAlphaChannel )
{
  SWError err = kNoError;
  FramePtr tempFrameP = 0;
  SDL_Surface *tempSurfaceP;
  uint32_t* cmask = &sdl2ctx.cmask[0];
  err = SWCreateFrame( &tempFrameP );
  
  if (err == kNoError)
  {
    uint32_t a = createAlphaChannel ? cmask[3] : 0;
    /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
    tempSurfaceP = SDL_CreateRGBSurface(0, w, h, depth, cmask[0], cmask[1], cmask[2], a);
    if (!tempSurfaceP)
      err = kSDLCreateSurface;
  }
  
  if (err == kNoError)
  {
    tempFrameP->originalSurfaceP = tempSurfaceP;
    
    err = SWUpdateFrame( tempFrameP );
  }
  
  if (err == kNoError)
  {
    *newFrameP = tempFrameP;
  }
  else
  {
    if ( tempSurfaceP )
    {
      SDL_FreeSurface( tempSurfaceP );
    }
    if ( tempFrameP )
    {
      free( tempFrameP );
    }
  }
  
  return err;
}

///--------------------------------------------------------------------------------------
//	SWCreateFrameFromFile
///--------------------------------------------------------------------------------------

SWError SWCreateFrameFromFile (
                               FramePtr* newFrameP,
                               const char * filename)
{
  SWError err = kNoError;
  FramePtr tempFrameP = 0;
  SDL_Surface *tempSurfaceP;
  
  err = SWCreateFrame( &tempFrameP );
  
  if (err == kNoError)
  {
    tempSurfaceP = IMG_Load(filename);
    
    if (!tempSurfaceP) {
      fprintf(stderr, "SDL error: %s\n", SDL_GetError());
      SDL_ClearError();
      err = kSDLCreateSurfaceFromFile;
    }
  }
  
  if (err == kNoError)
  {
    tempFrameP->originalSurfaceP = tempSurfaceP;
    //tempFrameP->hasAlpha = true;
    
    err = SWUpdateFrame( tempFrameP );
  }
  
  if (err == kNoError)
  {
    *newFrameP = tempFrameP;
  }
  else
  {
    if ( tempSurfaceP )
    {
      SDL_FreeSurface( tempSurfaceP );
    }
    if ( tempFrameP )
    {
      free( tempFrameP );
    }
  }
  
  return err;
}

///--------------------------------------------------------------------------------------
//	SWDisposeFrame
///--------------------------------------------------------------------------------------

int SWDisposeFrame(
                   FramePtr *oldFramePP)
{
  int		frameDisposed = false;
  FramePtr	oldFrameP = *oldFramePP;
  
  
  if (oldFrameP != 0)
  {
    // is this frame still in use by another sprite?
    if (oldFrameP->useCount > 1)
    {
      // one less sprite is using it now!
      oldFrameP->useCount--;
    }
    else	// no more sprites are using this frame
    {
      frameDisposed = true;
      
      // If this is a video surface frame, don't dispose of its
      // surfaces, since the video surface is disposed of by SDL_Quit
      if(!oldFrameP->isVideoSurface)
      {
        if (oldFrameP->frameSurfaceP)
        {
          if (!oldFrameP->sharesSurface)
            SDL_FreeSurface(oldFrameP->frameSurfaceP);
          oldFrameP->frameSurfaceP = 0;
        }
        
        if (oldFrameP->originalSurfaceP)
        {
          if (!oldFrameP->sharesSurface)
            SDL_FreeSurface(oldFrameP->originalSurfaceP);
          oldFrameP->originalSurfaceP = 0;
        }
      }
      
      free( oldFrameP );
      *oldFramePP = 0;	// Change the original pointer to 0
    }
  }
  
  return frameDisposed;
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	SWLockFrame
///--------------------------------------------------------------------------------------

void SWLockFrame(
                 FramePtr srcFrameP)
{
  SW_ASSERT(srcFrameP != 0);
  
  if (!srcFrameP->isFrameLocked)
  {
    srcFrameP->isFrameLocked = true;
  }
}

///--------------------------------------------------------------------------------------
//	SWUnlockFrame
///--------------------------------------------------------------------------------------

void SWUnlockFrame(
                   FramePtr srcFrameP)
{
  SW_ASSERT(srcFrameP != 0);
  
  if (srcFrameP->isFrameLocked)
  {
    srcFrameP->isFrameLocked = false;
  }
}

///--------------------------------------------------------------------------------------
//  SWCopyFrame - creates a new Frame and copies oldFrameP into it.
///--------------------------------------------------------------------------------------
/*
 SWError SWCopyFrame(
 SpriteWorldPtr destSpriteWorldP,
 FramePtr oldFrameP,
 FramePtr *newFrameP,
 int copyMasks)
 {
 SWError 		err = kNoError;
 SDL_Surface		*tempSurfaceP = 0;
 FramePtr 		tempFrameP;
 SWRect 			frameRect;
 
 SW_ASSERT(newFrameP != NULL);
 SW_ASSERT(oldFrameP != NULL);
 SW_ASSERT(oldFrameP->frameSurfaceP != NULL);
 SW_ASSERT(destSpriteWorldP != NULL);
 
 *newFrameP = NULL;
 tempFrameP = NULL;
 
 // Get size of new frame
 frameRect = oldFrameP->frameRect;
 SW_OFFSET_RECT( frameRect, -frameRect.left, -frameRect.top );
 
 // Create the new frame
 err = SWCreateFrame( &tempFrameP );
 
 // Copy the image from the old frame into the new frame
 if ( err == kNoError )
 {
 
 if ( copyMasks && oldFrameP->maskType == kAlphaChannelMask )
 {
 tempSurfaceP = SDL_DisplayFormatAlpha( oldFrameP->frameSurfaceP );
 } else
 {
 tempSurfaceP = SDL_DisplayFormat( oldFrameP->frameSurfaceP );
 }
 if (!tempSurfaceP)
 {
 err = kSDLSurfaceConversion;
 }
 
 if (err == kNoError)
 {
 tempFrameP->frameSurfaceP = tempSurfaceP;
 err = SWUpdateFrame( tempFrameP );
 }
 }
 
 if ((oldFrameP->isFrameLocked) && (err == kNoError))
 {
 SWLockFrame(tempFrameP);
 }
 
 if (err == kNoError)
 {
 *newFrameP = tempFrameP;
 }
 else
 {
 // an error occurred so dispose of anything we managed to create
 if ( tempSurfaceP )
 {
 SDL_FreeSurface( tempSurfaceP );
 }
 if (tempFrameP != NULL)
 {
 SWDisposeFrame( &tempFrameP );
 }
 }
 
 return err;
 }
 */

///--------------------------------------------------------------------------------------
//	SWUpdateFrame
///--------------------------------------------------------------------------------------

SWError SWUpdateFrame (
                       FramePtr srcFrameP)
{
  SWError err = kNoError;
  SDL_Surface *tempSurfaceP = 0;
  SDL_BlendMode blendMode;
  
  SW_ASSERT(srcFrameP != 0);
  
  if (srcFrameP->originalSurfaceP)
  {
    /* NOTE: If this is an offscreen frame (used for work area, etc...),
     and it has an alpha channel this will use "SDL_DisplayFormatAlpha"
     to create the formatted surface... which DOES NOT WORK */
    //    saveHardware = SDL_GetVideoSurface()->flags & SDL_HWSURFACE;
    //
    //    if( saveHardware )
    //    {
    //      SDL_GetVideoSurface()->flags &= ~SDL_HWSURFACE;
    //      SDL_GetVideoSurface()->flags |= SDL_SWSURFACE;
    //    }
    
    SDL_GetSurfaceBlendMode(srcFrameP->originalSurfaceP, &blendMode);
    
    if (blendMode == SDL_BLENDMODE_BLEND) {
      tempSurfaceP = SDL_ConvertSurfaceFormat(srcFrameP->originalSurfaceP, SDL_PIXELFORMAT_ARGB8888, 0);
    }
    else {
      tempSurfaceP = SDL_ConvertSurfaceFormat(srcFrameP->originalSurfaceP, SDL_PIXELFORMAT_RGB888, 0);
    }
    
    //    if( saveHardware )
    //    {
    //      SDL_GetVideoSurface()->flags &= ~SDL_SWSURFACE;
    //      SDL_GetVideoSurface()->flags |= SDL_HWSURFACE;
    //    }
    
    if (!tempSurfaceP)
    {
      err = kSDLSurfaceConversion;
    }
    
    if (err == kNoError)
    {
      if( srcFrameP->frameSurfaceP )
      {
        SDL_FreeSurface(srcFrameP->frameSurfaceP);
      }
      
      srcFrameP->frameSurfaceP = tempSurfaceP;
    }
  }
  
  if ( err == kNoError )
  {
    // Update the frame rect if the frame surface isn't shared (it isn't a portion of a surface)
    if ( !srcFrameP->sharesSurface )
    {
      SW_SET_RECT( srcFrameP->frameRect, 0, 0, srcFrameP->frameSurfaceP->w, srcFrameP->frameSurfaceP->h );
    }
    
    // Set mask type
    SDL_GetSurfaceBlendMode(srcFrameP->originalSurfaceP, &blendMode);
    
    if (blendMode != SDL_BLENDMODE_BLEND) {
      srcFrameP->maskType = kAlphaChannelMask;
    } else {
      srcFrameP->maskType = kNoMask;
    }
  }
  else
  {
    if ( tempSurfaceP )
    {
      SDL_FreeSurface( tempSurfaceP );
    }
  }
  
  return err;
}
