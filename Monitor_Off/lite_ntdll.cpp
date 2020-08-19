/*
	Monitor Off will turn off the computer's monitor.
	Copyright (C) 2017-2020 Eric Kutcher

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

#include "lite_dlls.h"
#include "lite_ntdll.h"

#ifndef NTDLL_USE_STATIC_LIB

	#ifdef __cplusplus
		extern "C"
		{
	#endif
			int _fltused = 1;	// Dummy value so that the linker shuts up.
	#ifdef __cplusplus
		}
	#endif

	pmemcpy			_memcpy;
	pmemset			_memset;

	HMODULE hModule_ntdll = NULL;

	unsigned char ntdll_state = NTDLL_STATE_SHUTDOWN;	// 0 = Not running, 1 = running.

	bool InitializeNTDLL()
	{
		if ( ntdll_state != NTDLL_STATE_SHUTDOWN )
		{
			return true;
		}

		hModule_ntdll = LoadLibraryDEMW( L"ntdll.dll" );

		if ( hModule_ntdll == NULL )
		{
			return false;
		}

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_ntdll, ( void ** )&_memcpy, "memcpy" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_ntdll, ( void ** )&_memset, "memset" ) )

		ntdll_state = NTDLL_STATE_RUNNING;

		return true;
	}

	bool UnInitializeNTDLL()
	{
		if ( ntdll_state != NTDLL_STATE_SHUTDOWN )
		{
			ntdll_state = NTDLL_STATE_SHUTDOWN;

			return ( FreeLibrary( hModule_ntdll ) == FALSE ? false : true );
		}

		return true;
	}

	void * WINAPIV _memcpy_s( void *dest, size_t size, const void *src, size_t count )
	{
		if ( src == NULL || size < count )
		{
			_memzero( dest, size );
			return dest;
		}

		return _memcpy( dest, src, count );
	}

	void * WINAPIV _wmemcpy_s( void *dest, size_t size, const void *src, size_t count )
	{
		size_t wsize = sizeof( wchar_t ) * size;
		size_t wcount = sizeof( wchar_t ) * count;

		if ( src == NULL || wsize < wcount )
		{
			_memzero( dest, wsize );
			return dest;
		}

		return _memcpy( dest, src, wcount );
	}

#endif
