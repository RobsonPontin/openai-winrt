#include "pch.h"
#include "ImageUtils.h"

#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Graphics.Imaging.h"
#include "winrt/Windows.Graphics.Display.h"

using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;

namespace Utils
{
	/// <summary>
	/// Create a mask image to be used with DALL-E image editing.
	/// The result is an image which has alpha value set to zero
	/// in the region the editing must happen.
	/// </summary>
	IAsyncOperation<WriteableBitmap> ImageUtils::CreateMaskAsync(
		int32_t width, 
		int32_t height, 
		Windows::Foundation::Rect rect)
	{
		WriteableBitmap wb{ width, height };
		uint32_t totalSize = width * height * 4;

		auto imageArray{ winrt::com_array<uint8_t>(totalSize) };
		uint8_t* pTargetBytes{ wb.PixelBuffer().data() };

		// track x,y positioning
		float line = 0;
		float column = 0;

		float x_editStart = rect.X;
		float y_editStart = rect.Y;
		float width_edit = rect.Width;
		float height_edit = rect.Height;

		uint8_t alphaVal = 0;

		for (int i = 0; i < imageArray.size(); i += 4)
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

			//BGRA format
			*(pTargetBytes++) = 255; // Blue
			*(pTargetBytes++) = 255;  // Green
			*(pTargetBytes++) = 255; // Red

			// Deals with alpha separately
			if ((column >= x_editStart && column < x_editStart + width_edit)
				&& (line >= y_editStart && line < y_editStart + height_edit))
			{
				*(pTargetBytes++) = alphaVal;
			}
			else
			{
				*(pTargetBytes++) = 255; // Alpha
			}
		}

		co_return wb;
	}
}