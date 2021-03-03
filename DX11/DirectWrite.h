#pragma once
#include <dwrite.h>
#include <d2d1.h>
class CDirectWrite
{
public:
	void CreateDeviceIndependentResources();
	void CreateDeviceResources();

	void DiscardDeviceResources();
	void Draw();
private:
	IDWriteFactory* pDWriteFactory_;
	IDWriteTextFormat* pTextFormat_;

	const wchar_t* wszText_;
	UINT32 cTextLength_;

	ID2D1Factory* pD2DFactory_;
	ID2D1HwndRenderTarget* pRT_;
	ID2D1SolidColorBrush* pBlackBrush_;
};
