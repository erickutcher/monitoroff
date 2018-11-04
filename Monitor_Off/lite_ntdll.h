/*
	Monitor Off will turn off the computer's monitor.
	Copyright (C) 2017-2018 Eric Kutcher

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _LITE_NTDLL_H
#define _LITE_NTDLL_H

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#define NTDLL_USE_STATIC_LIB

#ifdef NTDLL_USE_STATIC_LIB

	//__pragma( comment( lib, "msvcrt.lib" ) )

	#include <stdio.h>

	#define _memcpy			memcpy
	#define _memset			memset

	#define _memcpy_s		memcpy_s
	#define _wmemcpy_s( dest, size, src, count )	memcpy_s( dest, sizeof( wchar_t ) * ( size ), src, sizeof( wchar_t ) * ( count ) )

#else

	#define NTDLL_STATE_SHUTDOWN		0
	#define NTDLL_STATE_RUNNING			1

	typedef void * ( WINAPIV *pmemcpy )( void *dest, const void *src, size_t count );
	typedef void * ( WINAPIV *pmemset )( void *dest, int c, size_t count );

	extern pmemcpy			_memcpy;
	extern pmemset			_memset;

	extern unsigned char ntdll_state;

	bool InitializeNTDLL();
	bool UnInitializeNTDLL();

	void * WINAPIV _memcpy_s( void *dest, size_t size, const void *src, size_t count );
	void * WINAPIV _wmemcpy_s( void *dest, size_t size, const void *src, size_t count );

#endif

#define _memzero( dest, count ) _memset( dest, 0, count )

#endif
