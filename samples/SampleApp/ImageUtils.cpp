#include "pch.h"
#include "ImageUtils.h"

#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Graphics.Imaging.h"
#include "winrt/Windows.Graphics.Display.h"
#include "winrt/Windows.UI.Xaml.Media.Imaging.h"


namespace SampleApp::Utils
{
	// TODO: WIP on how to create a mask for DALL-E edit API.

	/// <summary>
	/// Small logic on how to edit every pixel data in BGRA8
	/// </summary>
	IAsyncAction ImageUtils::CreateMaskFromImage(StorageFile const& file, Windows::Foundation::Rect rect)
	{
		auto img = co_await file.OpenReadAsync();
		auto decoder = co_await winrt::Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(img);

		auto width = decoder.PixelWidth();
		auto height = decoder.PixelHeight();

		auto writeableBitmap = Windows::UI::Xaml::Media::Imaging::WriteableBitmap(width, height);

		// Scale the image to the appropriate size.
		Windows::Graphics::Imaging::BitmapTransform transform;
		transform.ScaledWidth(width);
		transform.ScaledHeight(height);

		Windows::Graphics::Imaging::PixelDataProvider pixelData{ co_await decoder.GetPixelDataAsync(
			Windows::Graphics::Imaging::BitmapPixelFormat::Bgra8, // WriteableBitmap uses BGRA format 
			Windows::Graphics::Imaging::BitmapAlphaMode::Straight,
			transform,
			Windows::Graphics::Imaging::ExifOrientationMode::IgnoreExifOrientation, // This sample ignores Exif orientation 
			Windows::Graphics::Imaging::ColorManagementMode::DoNotColorManage
		) };

		winrt::com_array<uint8_t> sourcePixels{ pixelData.DetachPixelData() };
		uint8_t* pTargetBytes{ writeableBitmap.PixelBuffer().data() };

		// track x,y positioning
		float line = 0;
		float column = 0;

		float x_editStart = rect.X;
		float y_editStart = rect.Y;
		float width_edit = rect.Width;
		float height_edit = rect.Height;

		// BGRA8 == 4 bytes/uint8_t per pixel (Blue, Green, Red, and Alpha)
		int every4 = 0;
		uint8_t alphaVal = 0;
		for (int i = 0; i < sourcePixels.size(); ++i)
		{
			if (every4 == 3)
			{
				// Track column and line number
				if (column >= width - 1)
				{
					line++;
					column = 0;
				}
				else
				{
					column++;
				}

				if ((column >= x_editStart && column < x_editStart + width_edit)
					&& (line >= y_editStart && line < y_editStart + height_edit))
				{
					*(pTargetBytes++) = alphaVal;
				}
				else
				{
					*(pTargetBytes++) = sourcePixels[i];
				}

				every4 = 0;

				continue;
			}

			*(pTargetBytes++) = sourcePixels[i];

			every4++;
		}

		// TODO: buffer? image? soft bitmap?
		// return writeableBitmap;
	}
}