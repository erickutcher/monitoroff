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

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "lite_ntdll.h"
#include "lite_user32.h"
#include "lite_shell32.h"

#include "resource.h"

#define WM_TRAY_NOTIFY		WM_APP

#define MIN_WIDTH			120
#define MIN_HEIGHT			60

#define BTN_EXIT			1000
#define MENU_EXIT			10000

HWND g_hWnd_main = NULL;

UINT WM_TASKBARCREATED = 0;

bool is_system_tray_initialized = false;
HMENU g_hMenuSub_tray = NULL;

NOTIFYICONDATA g_nid;

BYTE vk_list[ 4 ];
BYTE vk_list_count;

void InitializeSystemTray( HWND hWnd )
{
	if ( !is_system_tray_initialized )
	{
		_memzero( &g_nid, sizeof( NOTIFYICONDATA ) );
		g_nid.cbSize = NOTIFYICONDATA_V2_SIZE;	// 5.0 (Windows 2000) and newer.
		g_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		g_nid.hWnd = hWnd;
		g_nid.uCallbackMessage = WM_TRAY_NOTIFY;
		g_nid.uID = 1000;
		g_nid.hIcon = ( HICON )_LoadImageW( GetModuleHandleW( NULL ), MAKEINTRESOURCE( IDI_ICON ), IMAGE_ICON, 16, 16, LR_SHARED );
		_wmemcpy_s( g_nid.szTip, sizeof( g_nid.szTip ) / sizeof( g_nid.szTip[ 0 ] ), L"Middle click the system tray icon or press Ctrl + Shift + F4 to turn the monitor off.\0", 86 );
		g_nid.szTip[ 85 ] = 0;	// Sanity.

		is_system_tray_initialized = true;
	}

	_Shell_NotifyIconW( NIM_ADD, &g_nid );
}

LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( nCode == HC_ACTION )
	{
		KBDLLHOOKSTRUCT *kbdllhs = ( KBDLLHOOKSTRUCT * )lParam;

		// A key up might follow key down and prevent the monitor from turning off. Use key up instead.
		if ( kbdllhs != NULL && wParam == WM_KEYUP )
		{
			if ( vk_list_count > 0 )
			{
				bool modifiers_are_down = false;
				for ( BYTE i = 0; i < ( vk_list_count - 1 ); ++i )
				{
					if ( _GetAsyncKeyState( vk_list[ i ] ) & 0x8000 )
					{
						modifiers_are_down = true;
					}
					else
					{
						modifiers_are_down = false;
						break;
					}
				}

				if ( ( kbdllhs->vkCode == vk_list[ vk_list_count - 1 ] ) && modifiers_are_down )
				{
					_SendMessageW( g_hWnd_main, WM_SYSCOMMAND, SC_MONITORPOWER, 2 );
				}
			}
			else
			{
				if ( ( kbdllhs->vkCode == VK_F4 ) &&
					 ( _GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) &&
					 ( _GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) )
				{
					_SendMessageW( g_hWnd_main, WM_SYSCOMMAND, SC_MONITORPOWER, 2 );
				}
			}
		}
	}

	return _CallNextHookEx( NULL, nCode, wParam, lParam );
}

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			g_hMenuSub_tray = _CreatePopupMenu();

			MENUITEMINFO mii;
			_memzero( &mii, sizeof( MENUITEMINFO ) );
			mii.cbSize = sizeof( MENUITEMINFO );
			mii.fMask = MIIM_TYPE | MIIM_ID;
			mii.fType = MFT_STRING;
			mii.dwTypeData = L"Exit";
			mii.cch = 4;
			mii.wID = MENU_EXIT;
			_InsertMenuItemW( g_hMenuSub_tray, 0, TRUE, &mii );

			InitializeSystemTray( hWnd );

			WM_TASKBARCREATED = _RegisterWindowMessageW( L"TaskbarCreated" );

			return 0;
		}
		break;

		case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
				case MENU_EXIT:
				{
					_DestroyWindow( hWnd );
				}
				break;
			}

			return 0;
		}
		break;

		case WM_TRAY_NOTIFY:
		{
			switch ( lParam )
			{
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_CONTEXTMENU:
				{
					_SetForegroundWindow( hWnd );	// Must set so that the menu can close.

					// Show our edit context menu as a popup.
					POINT p;
					_GetCursorPos( &p ) ;
					_TrackPopupMenu( g_hMenuSub_tray, 0, p.x, p.y, 0, hWnd, NULL );
				}
				break;

				case WM_MBUTTONDOWN:
				{
					_SendMessageW( g_hWnd_main, WM_SYSCOMMAND, SC_MONITORPOWER, 2 );
				}
				break;
			}
		}
		break;

		case WM_CLOSE:
		{
			_DestroyWindow( hWnd );

			return 0;
		}
		break;

		case WM_DESTROY:
		{
			_DestroyMenu( g_hMenuSub_tray );

			// Remove the icon from the notification area.
			_Shell_NotifyIconW( NIM_DELETE, &g_nid );

			_PostQuitMessage( 0 );

			return 0;
		}
		break;

		default:
		{
			if ( msg == WM_TASKBARCREATED )
			{
				InitializeSystemTray( hWnd );
			}

			return _DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return TRUE;
}

#ifndef NTDLL_USE_STATIC_LIB
int APIENTRY _WinMain()
#else
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#endif
{
	#ifndef NTDLL_USE_STATIC_LIB
		HINSTANCE hInstance = GetModuleHandle( NULL );
	#endif

	#ifndef USER32_USE_STATIC_LIB
		if ( !InitializeUser32() ){ goto CLEANUP; }
	#endif
	#ifndef NTDLL_USE_STATIC_LIB
		if ( !InitializeNTDLL() ){ goto CLEANUP; }
	#endif
	#ifndef SHELL32_USE_STATIC_LIB
		if ( !InitializeShell32() ){ goto CLEANUP; }
	#endif

	MSG msg;
	_memzero( &msg, sizeof( MSG ) );

	_memzero( vk_list, sizeof( BYTE ) * 4 );
	vk_list_count = 0;

	int argCount = 0;
	LPWSTR *szArgList = _CommandLineToArgvW( GetCommandLineW(), &argCount );
	if ( szArgList != NULL )
	{
		for ( int arg = 1; arg < argCount && arg <= 4; ++arg )
		{
			vk_list[ vk_list_count++ ] = ( BYTE )_wcstoul( szArgList[ arg ], NULL, 16 );
		}

		// Free the parameter list.
		LocalFree( szArgList );
	}

	HHOOK hHook = _SetWindowsHookExW( WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0 );

	// Initialize our window class.
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc	= MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= ( HBRUSH )( COLOR_WINDOW );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"MonitorOff";
	wcex.hIconSm		= NULL;

	if ( !_RegisterClassExW( &wcex ) )
	{
		goto CLEANUP;
	}

	g_hWnd_main = _CreateWindowW( L"MonitorOff", L"Monitor Off", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL );

	if ( !g_hWnd_main )
	{
		goto CLEANUP;
	}

	// Main message loop:
	while ( _GetMessageW( &msg, NULL, 0, 0 ) > 0 )
	{
		_TranslateMessage( &msg );
		_DispatchMessageW( &msg );
	}

CLEANUP:

	 _UnhookWindowsHookEx( hHook );

	#ifndef SHELL32_USE_STATIC_LIB
		UnInitializeShell32();
	#endif
	#ifndef NTDLL_USE_STATIC_LIB
		UnInitializeNTDLL();
	#endif
	#ifndef USER32_USE_STATIC_LIB
		UnInitializeUser32();
	#endif

	#ifndef NTDLL_USE_STATIC_LIB
		ExitProcess( ( UINT )msg.wParam );
	#endif
	return ( int )msg.wParam;
}
