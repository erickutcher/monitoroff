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

#include "lite_dlls.h"
#include "lite_shell32.h"

#ifndef SHELL32_USE_STATIC_LIB

	pCommandLineToArgvW		_CommandLineToArgvW;
	pShell_NotifyIconW		_Shell_NotifyIconW;

	HMODULE hModule_shell32 = NULL;

	unsigned char shell32_state = SHELL32_STATE_SHUTDOWN;

	bool InitializeShell32()
	{
		if ( shell32_state != SHELL32_STATE_SHUTDOWN )
		{
			return true;
		}

		hModule_shell32 = LoadLibraryDEMW( L"shell32.dll" );

		if ( hModule_shell32 == NULL )
		{
			return false;
		}

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_CommandLineToArgvW, "CommandLineToArgvW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_Shell_NotifyIconW, "Shell_NotifyIconW" ) )

		shell32_state = SHELL32_STATE_RUNNING;

		return true;
	}

	bool UnInitializeShell32()
	{
		if ( shell32_state != SHELL32_STATE_SHUTDOWN )
		{
			shell32_state = SHELL32_STATE_SHUTDOWN;

			return ( FreeLibrary( hModule_shell32 ) == FALSE ? false : true );
		}

		return true;
	}

#endif
