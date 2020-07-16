
#include <time.h>
#include <stdlib.h>

#ifndef __SWUTILS__
#include <SpriteWorldUtils.h>
#endif

///--------------------------------------------------------------------------------------
//	Randomize
///--------------------------------------------------------------------------------------

void Randomize()
{
	srand( (unsigned int)time(NULL) );
}

///--------------------------------------------------------------------------------------
//	GetRandom - generates pseudo random number. min and max are inclusive
///--------------------------------------------------------------------------------------

int GetRandom( int min, int max )
{
	int num;
  
  // 0xfede: NOTE: RAND_MAX+1 overflows. Did this work with a previous version of RAND_MAX where the last nibble was less than 7?
	num = ((float)rand() / (float)(RAND_MAX)) * (max - min + 1);
	num = num + min;
	
	return num;
}

#pragma mark -

void CenterSWRect( SWRect *centerThis, SWRect *insideThis )
{
	int offsetX;
	int offsetY;
	
	offsetX = (insideThis->left + insideThis->right - centerThis->left - centerThis->right) / 2;
	offsetY = (insideThis->top + insideThis->bottom - centerThis->top - centerThis->bottom) / 2;
	
   	SW_OFFSET_RECT( *centerThis, offsetX, offsetY );
}

int SectSWRect( SWRect *rect1, SWRect *rect2, SWRect *outSectRect )
{
	SWRect sectRect;
	
	SW_SET_RECT(
		sectRect,
		SW_MAX( rect1->left, rect2->left ),
		SW_MAX( rect1->top, rect2->top ),
		SW_MIN( rect1->right, rect2->right ),
		SW_MIN( rect1->bottom, rect2->bottom ) );
	
	if( outSectRect ) *outSectRect = sectRect;
	
	return( sectRect.top < sectRect.bottom && sectRect.left < sectRect.right );
}

#pragma mark -

///--------------------------------------------------------------------------------------
//	ReportError
///--------------------------------------------------------------------------------------

void ReportError( SWError err, char *fileName, char *functionName, int lineNumber)
{
	fprintf( stderr, "Fatal error (#%d).\n  File:%s\n  Function:%s\n  Line:%i\n",
		err, fileName, functionName, lineNumber );
}
