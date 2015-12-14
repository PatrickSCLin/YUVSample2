#pragma once

using namespace Microsoft::Graphics::Canvas;

namespace Win2D_YUVCanvasBitmap
{
    public ref class YUVCanvasBitmap sealed
    {
    public:
		static void render(CanvasRenderTarget^ target, const Platform::Array<byte>^ dataY, const Platform::Array<byte>^ dataU, const Platform::Array<byte>^ dataV);
    };
}
