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
#include "lite_user32.h"

#ifndef USER32_USE_STATIC_LIB

	pCallNextHookEx			_CallNextHookEx;
	pCreatePopupMenu		_CreatePopupMenu;
	pDefWindowProcW			_DefWindowProcW;
	pDestroyMenu			_DestroyMenu;
	pDestroyWindow			_DestroyWindow;
	pDispatchMessageW		_DispatchMessageW;
	pGetAsyncKeyState		_GetAsyncKeyState;
	pGetCursorPos			_GetCursorPos;
	pGetMessageW			_GetMessageW;
	pInsertMenuItemW		_InsertMenuItemW;
	pLoadImageW				_LoadImageW;
	pPostQuitMessage		_PostQuitMessage;
	pRegisterClassExW		_RegisterClassExW;
	pRegisterWindowMessageW	_RegisterWindowMessageW;
	pSendMessageW			_SendMessageW;
	pSetForegroundWindow	_SetForegroundWindow;
	pSetWindowsHookExW		_SetWindowsHookExW;
	pTrackPopupMenu			_TrackPopupMenu;
	pTranslateMessage		_TranslateMessage;
	pUnhookWindowsHookEx	_UnhookWindowsHookEx;

	HMODULE hModule_user32 = NULL;

	unsigned char user32_state = 0;	// 0 = Not running, 1 = running.

	bool InitializeUser32()
	{
		if ( user32_state != USER32_STATE_SHUTDOWN )
		{
			return true;
		}

		hModule_user32 = LoadLibraryDEMW( L"user32.dll" );

		if ( hModule_user32 == NULL )
		{
			return false;
		}

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CallNextHookEx, "CallNextHookEx" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CreatePopupMenu, "CreatePopupMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DefWindowProcW, "DefWindowProcW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DestroyMenu, "DestroyMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DestroyWindow, "DestroyWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DispatchMessageW, "DispatchMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetAsyncKeyState, "GetAsyncKeyState" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetCursorPos, "GetCursorPos" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetMessageW, "GetMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_InsertMenuItemW, "InsertMenuItemW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_LoadImageW, "LoadImageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_PostQuitMessage, "PostQuitMessage" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_RegisterClassExW, "RegisterClassExW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_RegisterWindowMessageW, "RegisterWindowMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SendMessageW, "SendMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetForegroundWindow, "SetForegroundWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetWindowsHookExW, "SetWindowsHookExW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_TrackPopupMenu, "TrackPopupMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_TranslateMessage, "TranslateMessage" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_UnhookWindowsHookEx, "UnhookWindowsHookEx" ) )

		user32_state = USER32_STATE_RUNNING;

		return true;
	}

	bool UnInitializeUser32()
	{
		if ( user32_state != USER32_STATE_SHUTDOWN )
		{
			user32_state = USER32_STATE_SHUTDOWN;

			return ( FreeLibrary( hModule_user32 ) == FALSE ? false : true );
		}

		return true;
	}

#endif
