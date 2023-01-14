#include "pch.h"
#include "ImageUtils.h"

#include "winrt/Windows.Graphics.Imaging.h"

namespace OpenAI::Utils
{
    WF::IAsyncOperation<WS::Streams::IBuffer> Convert::FileToPngBufferAsync(WS::StorageFile const& file)
    {
        try
        {
            // Open and decode
            auto stream = co_await file.OpenReadAsync();
            auto decoder = co_await WGI::BitmapDecoder::CreateAsync(stream);
            auto pixels = co_await decoder.GetPixelDataAsync();
            auto imageBytes = pixels.DetachPixelData();
            auto dpiX = decoder.DpiX();
            auto dpiY = decoder.DpiY();
            auto pixelHeight = static_cast<uint32_t>(decoder.PixelHeight());
            auto pixelWidth = static_cast<uint32_t>(decoder.PixelWidth());

            // encode
            WS::Streams::InMemoryRandomAccessStream encoded{};
            auto encoder = co_await WGI::BitmapEncoder::CreateAsync(WGI::BitmapEncoder::PngEncoderId(), encoded);
            encoder.SetPixelData(WGI::BitmapPixelFormat::Bgra8, WGI::BitmapAlphaMode::Straight, pixelWidth, pixelHeight, dpiX, dpiY, imageBytes);
            co_await encoder.FlushAsync();
            encoded.Seek(0);

            auto size = static_cast<uint32_t>(encoded.Size());
            WS::Streams::Buffer buffer{ size };
            co_await encoded.ReadAsync(buffer, size, WS::Streams::InputStreamOptions::None);

            co_return buffer;
        }
        catch (...)
        {
            // Sigh...
        }

        co_return nullptr;
    }

}
