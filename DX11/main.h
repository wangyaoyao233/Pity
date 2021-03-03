#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <memory>
#include <list>
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>
#include <DirectXMath.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

#define SCREEN_WIDTH	(1080)			// ウインドウの幅
#define SCREEN_HEIGHT	(720)			// ウインドウの高さ

HWND GetWindow();

using namespace DirectX;