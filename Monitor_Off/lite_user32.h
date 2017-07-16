/*
	Monitor Off will turn off the computer's monitor.
	Copyright (C) 2017 Eric Kutcher

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

#ifndef _LITE_USER32_H
#define _LITE_USER32_H

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define USER32_USE_STATIC_LIB

#ifdef USER32_USE_STATIC_LIB

	//__pragma( comment( lib, "user32.lib" ) )

	#define _CallNextHookEx			CallNextHookEx
	#define _CreatePopupMenu		CreatePopupMenu
	#define _DefWindowProcW			DefWindowProcW
	#define _DestroyMenu			DestroyMenu
	#define _DestroyWindow			DestroyWindow
	#define _DispatchMessageW		DispatchMessageW
	#define _GetAsyncKeyState		GetAsyncKeyState
	#define _GetCursorPos			GetCursorPos
	#define _GetMessageW			GetMessageW
	#define _InsertMenuItemW		InsertMenuItemW
	#define _LoadImageW				LoadImageW
	#define _PostQuitMessage		PostQuitMessage
	#define _RegisterClassExW		RegisterClassExW
	#define _SendMessageW			SendMessageW
	#define _SetForegroundWindow	SetForegroundWindow
	#define _SetWindowsHookExW		SetWindowsHookExW
	#define _TrackPopupMenu			TrackPopupMenu
	#define _TranslateMessage		TranslateMessage
	#define _UnhookWindowsHookEx	UnhookWindowsHookEx

	#define _CreateWindowW			CreateWindowW

#else

	#define USER32_STATE_SHUTDOWN	0
	#define USER32_STATE_RUNNING	1

	typedef LRESULT ( WINAPI *pCallNextHookEx )( HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam );
	typedef HMENU ( WINAPI *pCreatePopupMenu )( void );
	typedef LRESULT ( WINAPI *pDefWindowProcW )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef BOOL ( WINAPI *pDestroyMenu )( HMENU hMenu );
	typedef BOOL ( WINAPI *pDestroyWindow )( HWND hWnd );
	typedef LRESULT ( WINAPI *pDispatchMessageW )( const MSG *lpmsg );
	typedef SHORT ( WINAPI *pGetAsyncKeyState )( int vKey );
	typedef BOOL ( WINAPI *pGetCursorPos )( LPPOINT lpPoint );
	typedef BOOL ( WINAPI *pGetMessageW )( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax );
	typedef BOOL ( WINAPI *pInsertMenuItemW )( HMENU hMenu, UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii );
	typedef HANDLE ( WINAPI *pLoadImageW )( HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad );
	typedef VOID ( WINAPI *pPostQuitMessage )( int nExitCode );
	typedef ATOM ( WINAPI *pRegisterClassExW )( const WNDCLASSEX *lpwcx );
	typedef LRESULT ( WINAPI *pSendMessageW )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	typedef BOOL ( WINAPI *pSetForegroundWindow )( HWND hWnd );
	typedef HHOOK ( WINAPI *pSetWindowsHookExW )( int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId );
	typedef BOOL ( WINAPI *pTrackPopupMenu )( HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, const RECT *prcRect );
	typedef BOOL ( WINAPI *pTranslateMessage )( const MSG *lpMsg );
	typedef BOOL ( WINAPI *pUnhookWindowsHookEx )( HHOOK hhk );

	extern pCallNextHookEx			_CallNextHookEx;
	extern pCreatePopupMenu			_CreatePopupMenu;
	extern pDefWindowProcW			_DefWindowProcW;
	extern pDestroyMenu				_DestroyMenu;
	extern pDestroyWindow			_DestroyWindow;
	extern pDispatchMessageW		_DispatchMessageW;
	extern pGetAsyncKeyState		_GetAsyncKeyState;
	extern pGetCursorPos			_GetCursorPos;
	extern pGetMessageW				_GetMessageW;
	extern pInsertMenuItemW			_InsertMenuItemW;
	extern pLoadImageW				_LoadImageW;
	extern pPostQuitMessage			_PostQuitMessage;
	extern pRegisterClassExW		_RegisterClassExW;
	extern pSendMessageW			_SendMessageW;
	extern pSetForegroundWindow		_SetForegroundWindow;
	extern pSetWindowsHookExW		_SetWindowsHookExW;
	extern pTrackPopupMenu			_TrackPopupMenu;
	extern pTranslateMessage		_TranslateMessage;
	extern pUnhookWindowsHookEx		_UnhookWindowsHookEx;

	extern unsigned char user32_state;

	#define _CreateWindowW( lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam ) _CreateWindowExW( 0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam )

	bool InitializeUser32();
	bool UnInitializeUser32();

#endif

#endif
