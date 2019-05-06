

#ifndef __SWUTILS__
#define __SWUTILS__

#ifndef __SWCOMMON__
#include <SWCommonHeaders.h>
#endif

#ifndef FatalError
#define FatalError(err)			if (err) ReportError(err, __FILE__, __func__, __LINE__);
#endif

void Randomize();
int GetRandom( int min, int max );

void CenterSWRect( SWRect *centerThis, SWRect *insideThis );
int SectSWRect( SWRect *rect1, SWRect *rect2, SWRect *outSectRect );

void ReportError( SWError err, char *fileName, char *functionName, int lineNumber);

#endif // __SWUTILS__
