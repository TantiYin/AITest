#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <iostream>
#include "miner.h"
#include "Elsa.h"
#include "MessageDispatcher.h"
#include "entityName.h"
#include "entityManager.h"
#include "vehicle.h"
#include "PrecisionTimer.h"
#include "d2d1.h"
#include "dwrite.h"
#include "GameWorld.h"
//#include "lua.hpp"
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")


using namespace std;

ID2D1Factory* gpFactory;
IDWriteFactory* gpWriteFactory;
ID2D1HwndRenderTarget* gpRenderTarget;
ID2D1SolidColorBrush* gpBrush;
IDWriteTextFormat* gpTextFormat;

GameWorld* gWorld;

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

HRESULT CreateGraphicsResources(HWND hwnd)
{
	HRESULT hr = S_OK;
	if (gpRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = gpFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&gpRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 1.0f);
			hr = gpRenderTarget->CreateSolidColorBrush(color, &gpBrush);

			static const WCHAR msc_fontName[] = L"Courier New";//L"Consolas";//L"Verdana";
			static const FLOAT msc_fontSize = 15;
			// Create a DirectWrite text format object.
			hr = gpWriteFactory->CreateTextFormat(
				msc_fontName,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				msc_fontSize,
				L"", //locale
				&gpTextFormat
				);

			if (SUCCEEDED(hr))
			{
				// Center the text horizontally and vertically.
				gpTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

				gpTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			}
		}
	}
	return hr;
}

void DiscardGraphicsResources()
{
	SafeRelease(&gpRenderTarget);
	SafeRelease(&gpBrush);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		// Create a DirectWrite factory.
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&gpWriteFactory)
			);
		if (FAILED(hr))
		{
			return -1;
		}
		RECT rect;

		GetClientRect(hwnd, &rect);
		gWorld = new GameWorld(rect.right, rect.bottom);
		return 0;
	}

	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&gpFactory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		if (gWorld)
		{
			HRESULT hr = CreateGraphicsResources(hwnd);
			if (SUCCEEDED(hr))
			{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				gpRenderTarget->BeginDraw();
				gpRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
				gpBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));

				D2D1_SIZE_F size = gpRenderTarget->GetSize();
				WCHAR LineText[20] = {0};
				wsprintf(LineText, L"0, 0");
				gpRenderTarget->DrawText(LineText, ARRAYSIZE(LineText) - 1, gpTextFormat, D2D1::RectF(0, 0, 50, 20), gpBrush);
				wsprintf(LineText, L"%d, %d", static_cast<int>(size.width), static_cast<int>(size.height));
				gpRenderTarget->DrawText(LineText, ARRAYSIZE(LineText) - 1, gpTextFormat, D2D1::RectF(size.width - 100, size.height - 20, size.width, size.height), gpBrush);

				for (int x = 0; x < size.width; x += 10)
				{
					gpRenderTarget->DrawLine(D2D1::Point2F(x, 0.0f), D2D1::Point2F(x, size.height), gpBrush, 1);
				}
				for (int y = 0; y < size.height; y += 10)
				{
					gpRenderTarget->DrawLine(D2D1::Point2F(0, y), D2D1::Point2F(size.width, y), gpBrush, 1);
				}

				gWorld->Render();

				HRESULT hr = gpRenderTarget->EndDraw();
				if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
				{
					DiscardGraphicsResources();
				}
				EndPaint(hwnd, &ps);
			}

		}
		return 0;
	}
	case WM_SIZE:
		if (gpRenderTarget != NULL)
		{
			RECT rc;
			GetClientRect(hwnd, &rc);
			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			gpRenderTarget->Resize(size);

		}
		return 0;

	case WM_LBUTTONDOWN:
	{
		gWorld->SetCrosshair(MAKEPOINTS(lParam));
	}
	break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS WinClass = { 0 };

	WinClass.hInstance = hInstance;
	WinClass.lpfnWndProc = WindowProc;
	WinClass.lpszClassName = L"SampleWindow";

	if (!RegisterClass(&WinClass))
	{
		MessageBox(NULL, L"Registration Failed!", L"Error", 0);

		//exit the application
		return 0;
	}

	hWnd = CreateWindowEx(NULL, WinClass.lpszClassName, L"Steering", WS_OVERLAPPEDWINDOW, 300, 300, 400, 400, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, L"CreateWindowEx Failed!", L"Error!", 0);
	}

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	bool bDone = false;

	CTimer Timer;
	Timer.Start();
	while (!bDone)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				//stop loop if it's a quit message
				bDone = true;
			}

			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (msg.message != WM_QUIT)
		{
			//update
			float TimeElapse = Timer.Update();
			gWorld->Update(TimeElapse);

			//render
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		}

	}//end while

	delete gWorld;
	return 0;
}

//int main()
//{
//	Miner *bob = new Miner(ent_Miner_Bob, Vector2(10, 20), 3);
//	EntityManager::Instance()->RegisterEntity(bob);
//	Elsa *elsa = new Elsa(ent_Elsa, Vector2(40, 20), 3);
//	EntityManager::Instance()->RegisterEntity(elsa);
//	
//	while (true)
//	{
//		bob->Update(1);
//		cout << " Energy: " << bob->GetEnergy() << " Water: " << bob->GetWater() << " Gold: " << bob->GetGold();
//		elsa->Update(1);
//
//		//dispatch any delayed messages
//		MessageDispatcher::Instance()->DispatchDelayedMessage();
//		Sleep(1000);
//	}
//	/*lua_State * pL = luaL_newstate();
//
//	lua_register(pL, "cpp_Add", cpp_Add);
//
//	int ret = luaL_dofile(pL, "test.lua");
//	if (ret )
//	{
//		cout << "load file failed" << endl;
//		return 0;
//	}
//
//
//	int age = 0;
//	string name = "";
//
//	lua_settop(pL, 0);
//	lua_getglobal(pL, "age");
//	lua_getglobal(pL, "name");
//
//	name = lua_tostring(pL, 2);
//	age = (int)lua_tonumber(pL, 1);
//
//	cout << "name: " << name << " age:" << age << endl;
//
//	lua_pop(pL, 2);
//
//	lua_getglobal(pL, "sample_table");
//	if (!lua_istable(pL,1))
//	{
//		cout << "not table" << endl;
//		
//	}
//	else
//	{
//
//		lua_getfield(pL, 1, "age");
//		lua_getfield(pL, 1, "name");
//		int age = 0;
//		string name = "";
//		if (lua_isnumber(pL, -2))
//		{
//			age = (int)lua_tonumber(pL, -2);
//		}
//		if (lua_isstring(pL, -1))
//		{
//			name = lua_tostring(pL, -1);
//		}
//
//		cout << "name: " << name << " age:" << age << endl; 
//
//		lua_pop(pL, 2);
//
//	}
//	lua_pop(pL, 1);
//
//	lua_getglobal(pL, "add");
//
//	lua_pushnumber(pL, 5);
//	lua_pushnumber(pL, 8);
//
//	lua_call(pL, 2, 1);
//
//	if (lua_isnumber(pL,1))
//	{
//		int result = lua_tonumber(pL, 1);
//		cout << "result: " << result << endl;
//	}
//	lua_pop(pL, 1);
//
//	lua_close(pL);*/
//
//	system("pause");
//	return 0;
//}