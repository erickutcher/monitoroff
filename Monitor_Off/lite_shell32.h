/*
	Monitor Off will turn off the computer's monitor.
	Copyright (C) 2017-2024 Eric Kutcher

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

#ifndef _LITE_SHELL32_H
#define _LITE_SHELL32_H

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shellapi.h>
#include <shlobj.h>

//#define SHELL32_USE_STATIC_LIB

#ifdef SHELL32_USE_STATIC_LIB

	//__pragma( comment( lib, "shell32.lib" ) )
	//__pragma( comment( lib, "shlwapi.lib" ) )

	#include <shlwapi.h>

	#define	_CommandLineToArgvW	CommandLineToArgvW
	#define _Shell_NotifyIconW	Shell_NotifyIconW

#else

	#define SHELL32_STATE_SHUTDOWN		0
	#define SHELL32_STATE_RUNNING		1

	typedef LPWSTR * ( WINAPI *pCommandLineToArgvW )( LPCWSTR lpCmdLine, int *pNumArgs );
	typedef BOOL ( WINAPI *pShell_NotifyIconW )( DWORD dwMessage, PNOTIFYICONDATA lpdata );

	extern pCommandLineToArgvW	_CommandLineToArgvW;
	extern pShell_NotifyIconW	_Shell_NotifyIconW;

	extern unsigned char shell32_state;

	bool InitializeShell32();
	bool UnInitializeShell32();

#endif

#endif
