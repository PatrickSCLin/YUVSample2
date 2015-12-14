#include "pch.h"
#include "YUVCanvasBitmap.h"

using namespace Win2D_YUVCanvasBitmap;
using namespace Platform;
using namespace Windows::UI;
using namespace Windows::Foundation;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::WRL;

void YUVCanvasBitmap::render(CanvasRenderTarget^ target, const Platform::Array<byte>^ dataY, const Platform::Array<byte>^ dataU, const Platform::Array<byte>^ dataV)
{
	CanvasDrawingSession^ session = target->CreateDrawingSession();

	ComPtr<ID2D1DeviceContext1> native_device_context = GetWrappedResource<ID2D1DeviceContext1>(session);

	ComPtr<ID2D1Effect> native_effect;

	HRESULT result = native_device_context->CreateEffect(CLSID_D2D1YCbCr, &native_effect);

	if (result == S_OK)
	{
		ComPtr<IWICImagingFactory> factory;

		ComPtr<IWICBitmap> bitmapY;

		ComPtr<IWICBitmap> bitmapU;

		ComPtr<IWICBitmap> bitmapV;

		ComPtr<IWICBitmapScaler> scaler;

		ComPtr<IWICPlanarBitmapSourceTransform> planarSource;

		result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory);

		result = factory->CreateBitmapFromMemory(4000, 3000, GUID_WICPixelFormat8bppY, 4000, dataY->Length, dataY->Data, &bitmapY);

		result = factory->CreateBitmapFromMemory(2000, 1500, GUID_WICPixelFormat8bppCb, 2000, dataU->Length, dataU->Data, &bitmapU);

		result = factory->CreateBitmapFromMemory(2000, 1500, GUID_WICPixelFormat8bppCr, 2000, dataV->Length, dataV->Data, &bitmapV);

		factory->CreateBitmapScaler(&scaler);

		scaler.As(&planarSource);

		/*ID2D1Bitmap* bitmapY;

		ID2D1Bitmap* bitmapUV;

		result = native_device_context->CreateBitmap(D2D1::SizeU(4000, 3000), dataY->Data, 4000, &D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_R8_UNORM, D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE)), &bitmapY);

		native_effect->SetInput(0, bitmapY);

		result = native_device_context->CreateBitmap(D2D1::SizeU(2000, 1500), dataUV->Data, 4000, &D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8_UNORM, D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE)), &bitmapUV);

		native_effect->SetInput(1, bitmapUV);

		native_effect->SetValue(D2D1_YCBCR_PROP_CHROMA_SUBSAMPLING, D2D1_YCBCR_CHROMA_SUBSAMPLING_420);

		D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Scale(0.3, 0.3) * D2D1::Matrix3x2F::Translation(D2D1::SizeF(0, 0));

		native_effect->SetValue(D2D1_YCBCR_PROP::D2D1_YCBCR_PROP_TRANSFORM_MATRIX, transform);*/

		native_device_context->DrawImage(native_effect.Get());
	}
}
