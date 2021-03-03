#include "main.h"
#include "DirectWrite.h"

#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "D2d1.lib")
void CDirectWrite::CreateDeviceIndependentResources()
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory_
	);

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory_)
		);
	}

	wszText_ = L"Hello World using  DirectWrite!";
	cTextLength_ = (UINT32)wcslen(wszText_);

	if (SUCCEEDED(hr))
	{
		hr = pDWriteFactory_->CreateTextFormat(
			L"Gabriola",                // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.0f,
			L"en-us",
			&pTextFormat_
		);
	}

	// Center align (horizontally) the text.
	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void CDirectWrite::CreateDeviceResources()
{
	RECT rc;
	GetClientRect(GetWindow(), &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	if (!pRT_)
	{
		// Create a Direct2D render target.
		HRESULT hr = pD2DFactory_->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				GetWindow(),
				size
			),
			&pRT_
		);

		// Create a black brush.
		if (SUCCEEDED(hr))
		{
			hr = pRT_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&pBlackBrush_
			);
		}
	}
}

void CDirectWrite::DiscardDeviceResources()
{
	pRT_->Release();
	pBlackBrush_->Release();
}

void CDirectWrite::Draw()
{
	CreateDeviceResources();

	pRT_->BeginDraw();
	pRT_->SetTransform(D2D1::IdentityMatrix());

	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	pRT_->Clear(D2D1::ColorF(0.5f, 0.5f, 0.5f));

	D2D1_RECT_F layoutRect = D2D1::RectF(
		SCREEN_WIDTH / 2 - 100.0f,
		SCREEN_HEIGHT / 2 - 100.0f,
		SCREEN_WIDTH / 2 + 100.0f,
		SCREEN_HEIGHT / 2 + 100.0f
	);

	pRT_->DrawText(
		wszText_,        // The string to render.
		cTextLength_,    // The string's length.
		pTextFormat_,    // The text format.
		layoutRect,       // The region of the window where the text will be rendered.
		pBlackBrush_     // The brush used to draw the text.
	);

	pRT_->EndDraw();
}