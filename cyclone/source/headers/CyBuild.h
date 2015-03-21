// FilterBuild.h
// © 1998 Matt Lewinski

#pragma once

#ifndef CYCLONE_BUILD_H
#define CYCLONE_BUILD_H

#include <assert.h>

// Useful debugging tools.

#if CYCLONE_BUILDING_DEBUG
	#define DEBUG(x) (x)
	#define ASSERT(x) ((x) ? ((void) 0) : __std(__assertion_failed)(#x, __FILE__, __LINE__))
#else
	#define DEBUG(x)
	#define ASSERT(x)
#endif


// Import/Export Defines
#if __INTEL__
	#if CYCLONE_BUILDING_STATIC
		#define _IMPEXP_CYCLONE
	#else
		#if CYCLONE_BUILDING_SHARED
			#define	_IMPEXP_CYCLONE	__declspec(dllexport)
		#else
			#define	_IMPEXP_CYCLONE	__declspec(dllimport)
		#endif
	#endif
#endif

#if __POWERPC__
	#define _IMPEXP_CYCLONE
#endif

class _IMPEXP_CYCLONE CyTarget;
class _IMPEXP_CYCLONE CyDispatcher;

#endif // FILTER_BUILD_H