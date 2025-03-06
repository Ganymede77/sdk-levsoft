#ifndef __OBSERVERBASE_H_
#define __OBSERVERBASE_H_


#include "levprimitive.h"


class CObserverBase
{
	public:

		CObserverBase() { }
		virtual ~CObserverBase() { }

		virtual void Update(GEOMETRICOPERATION , CLevPrimitive *pPrimitive = NULL) = 0;
};


#endif
