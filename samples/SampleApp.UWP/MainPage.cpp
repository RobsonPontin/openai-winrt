﻿#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt/Windows.Storage.Pickers.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.UI.Xaml.Media.Imaging.h"
#include "winrt/Windows.Graphics.Imaging.h"

#include "StorageUtils.h"
#include "ImageUtils.h"

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Graphics::Imaging;


namespace winrt::SampleApp::implementation
{
	void MainPage::MenuFlyoutItem_Click(IInspectable const& sender, RoutedEventArgs const&)
	{
		auto flyoutItem = sender.try_as<MenuFlyoutItem>();
		if (flyoutItem)
		{
			auto tag = winrt::unbox_value_or<hstring>(flyoutItem.Tag(), L"");
			if (tag == L"Variant")
			{
				tbImagePromt().IsEnabled(false);
				m_actionSelected = ActionType::ImageVariation;

				btnProcessImage().Content(winrt::box_value(L"Browse and Generate Image"));
			}
			else if (tag == L"Create")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ActionType::ImageCreate;

				btnProcessImage().Content(winrt::box_value(L"Generate Image"));
			}
			else if (tag == L"Edit")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ActionType::ImageEdit;

				btnProcessImage().Content(winrt::box_value(L"Browse and Edit Image"));
			}
			else if (tag == L"TextCompletion")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ActionType::TextCompletion;

				btnProcessImage().Content(winrt::box_value(L"Text Completion"));
			}
			else if (tag == L"TextEmbedding")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ActionType::TextEmbedding;

				btnProcessImage().Content(winrt::box_value(L"Text Embedding"));
			}
			else if (tag == L"TextModeration")
			{
				tbImagePromt().IsEnabled(true);
				m_actionSelected = ActionType::TextModeration;

				btnProcessImage().Content(winrt::box_value(L"Text Moderation"));
			}

			ddbAction().Content(winrt::box_value(flyoutItem.Text()));
		}
	}

	IAsyncAction MainPage::btnProcessImage_Click(IInspectable const& sender, RoutedEventArgs const& e)
	{
		if (tbOpenAiKey().Text() != L"")
		{
			m_openAiService.SetOpenAiSecretKey(tbOpenAiKey().Text());
		}
		else
		{
			return;
		}

		switch (m_actionSelected)
		{
		case ActionType::ImageCreate:
			if (tbImagePromt().Text() != L"")
			{
				co_await ProcessImageCreationAsync(tbImagePromt().Text());
			}
			break;

		case ActionType::ImageEdit:
			if (tbImagePromt().Text() != L"")
			{
				co_await ProcessImageEditAsync(tbImagePromt().Text());
			}
			break;

		case ActionType::ImageVariation:
			co_await ProcessImageVariantAsync();
			break;


		case ActionType::TextCompletion:
			co_await ProcessTextCompletionAsync(tbImagePromt().Text());
			break;

		case ActionType::TextEmbedding:
			co_await ProcessTextEmbeddingAsync(tbImagePromt().Text());
			break;
		case ActionType::TextModeration:
			co_await ProcessTextModerationAsync(tbImagePromt().Text());
			break;
		}
	}

	IAsyncAction MainPage::btnSaveImage_Click(IInspectable const& sender, RoutedEventArgs const& e)
	{
		co_await ::SampleApp::Utils::StorageUtils::CreateFileFromImageAsync(image(), false);
	}

#pragma region DALL-E
	IAsyncAction MainPage::ProcessImageVariantAsync()
	{
		if (!m_openAiService.IsRunning())
		{
			auto openPicker = Pickers::FileOpenPicker();
			openPicker.ViewMode(Pickers::PickerViewMode::Thumbnail);
			openPicker.SuggestedStartLocation(Pickers::PickerLocationId::PicturesLibrary);
			openPicker.FileTypeFilter().Append(L".png");

			auto file = co_await openPicker.PickSingleFileAsync();

			if (file != nullptr)
			{
				auto imageReq = OpenAI::Image::ImageVariationRequest{};
				imageReq.ImageName(file.Name());
				co_await imageReq.SetImageAsync(file);

				auto response = co_await m_openAiService.RunRequestAsync(imageReq);
				if (response.IsResponseSuccess())
				{
					auto imgBuffer = response.Images().GetAt(0);
					if (imgBuffer != nullptr)
					{
						auto stream = co_await ::SampleApp::Utils::StorageUtils::ToStreamAsync(imgBuffer);

						if (stream != nullptr)
						{
							auto img = BitmapImage{};
							img.SetSource(stream);
							image().Source(img);

							ShowImageResult();
						}
					}
				}
				else
				{
					ShowResponseError(response.Error());
				}
			}
		}
	}

	IAsyncAction MainPage::ProcessImageCreationAsync(winrt::hstring prompt)
	{
		if (!m_openAiService.IsRunning())
		{
			auto imageReq = OpenAI::Image::ImageCreateRequest{};
			imageReq.ImageName(L"ai image");
			imageReq.Prompt(prompt);

			auto response = co_await m_openAiService.RunRequestAsync(imageReq);
			if (response.IsResponseSuccess())
			{
				auto buffer = response.Images().GetAt(0);
				auto stream = co_await ::SampleApp::Utils::StorageUtils::ToStreamAsync(buffer);

				if (stream != nullptr)
				{
					auto img = BitmapImage{};
					img.SetSource(stream);
					image().Source(img);

					ShowImageResult();
				}
			}
			else
			{
				ShowResponseError(response.Error());
			}
		}

		co_return;
	}

	IAsyncAction MainPage::ProcessImageEditAsync(winrt::hstring prompt)
	{
		if (!m_openAiService.IsRunning())
		{
			auto openPicker = Pickers::FileOpenPicker();
			openPicker.ViewMode(Pickers::PickerViewMode::Thumbnail);
			openPicker.SuggestedStartLocation(Pickers::PickerLocationId::PicturesLibrary);
			openPicker.FileTypeFilter().Append(L".png");

			auto file = co_await openPicker.PickSingleFileAsync();

			if (file != nullptr)
			{
				auto imageReq = OpenAI::Image::ImageEditRequest{};
				imageReq.ImageName(file.Name());
				imageReq.Prompt(prompt);
				co_await imageReq.SetImageAsync(file);

				/* For this example the mask will always cover the half bottom section of the
				 * full image, so it will edit anything in it. */
				
				// Gathering image info
				auto img = co_await file.OpenReadAsync();
				auto decoder = co_await winrt::Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(img);

				auto fileWidth = static_cast<float>(decoder.PixelWidth());
				auto fileHeight = static_cast<float>(decoder.PixelHeight());

				Windows::Foundation::Rect rect{
					0,   // Start x
					fileHeight/2,  // Start y
					fileWidth,   // Width
					fileHeight/2}; // Height

				auto mask = co_await ::Utils::ImageUtils::CreateMaskAsync(fileWidth, fileHeight, rect);

				auto softBitmap = SoftwareBitmap::CreateCopyFromBuffer(
					mask.PixelBuffer(),
					BitmapPixelFormat::Bgra8,
					mask.PixelWidth(),
					mask.PixelHeight());

				// Encode it to PNG
				InMemoryRandomAccessStream stream{};
				auto encoder = co_await BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId(), stream);
				encoder.SetSoftwareBitmap(softBitmap);

				co_await encoder.FlushAsync();

				Buffer buffer{static_cast<uint32_t>(stream.Size())};
				auto result = co_await stream.ReadAsync(buffer, stream.Size(), InputStreamOptions::None);

				co_await imageReq.SetMaskAsync(result);

				auto response = co_await m_openAiService.RunRequestAsync(imageReq);
				if (response.IsResponseSuccess())
				{
					auto imgBuffer = response.Images().GetAt(0);
					if (imgBuffer != nullptr)
					{
						auto stream = co_await ::SampleApp::Utils::StorageUtils::ToStreamAsync(imgBuffer);

						if (stream != nullptr)
						{
							auto img = BitmapImage{};
							img.SetSource(stream);
							image().Source(img);

							ShowImageResult();
						}
					}
				}
				else
				{
					ShowResponseError(response.Error());
				}
			}
		}

		co_return;
	}
#pragma endregion

#pragma region GPT
	IAsyncAction MainPage::ProcessTextCompletionAsync(winrt::hstring prompt)
	{
		if (!m_openAiService.IsRunning())
		{
			auto completReq = OpenAI::Completion::CompletionRequest{};
			completReq.Prompt(prompt);

			auto response = co_await m_openAiService.RunRequestAsync(completReq);
			if (response.IsResponseSuccess())
			{
				textBlock().Text(response.Choices().GetAt(0).Text());
				ShowTextResult();
			}
			else
			{
				ShowResponseError(response.Error());
			}
		}
	}

	IAsyncAction MainPage::ProcessTextEmbeddingAsync(winrt::hstring prompt)
	{
		if (!m_openAiService.IsRunning())
		{
			auto embReq = OpenAI::Embedding::EmbeddingRequest{};
			embReq.Input(prompt);

			auto response = co_await m_openAiService.RunRequestAsync(embReq);
			if (response.IsResponseSuccess())
			{
				auto result = response.Data().First();
				auto embedding = result.Current().Embedding();

				winrt::hstring resultText = L"";
				for (uint32_t i = 0; i < embedding.Size(); ++i)
				{
					if (i == 0)
					{
						resultText = winrt::to_hstring(embedding.GetAt(i));
					}
					else
					{
						resultText = resultText + L"; " + winrt::to_hstring(embedding.GetAt(i));
					}
				}

				textBlock().Text(resultText);
				ShowTextResult();
			}
			else
			{
				ShowResponseError(response.Error());
			}
		}
	}

	IAsyncAction MainPage::ProcessTextModerationAsync(winrt::hstring prompt)
	{
		if (!m_openAiService.IsRunning())
		{
			auto embReq = OpenAI::Moderation::ModerationRequest{};
			embReq.Input(prompt);

			auto response = co_await m_openAiService.RunRequestAsync(embReq);
			if (response.IsResponseSuccess())
			{			
				auto moderationValues = response.Data();
							
				winrt::hstring violenceLevel = L"Error!" ;
				
				for (uint32_t i = 0; i < moderationValues.Size(); ++i)
				{
					auto value = moderationValues.GetAt(i);
					if (value.Category() == OpenAI::Moderation::ModerationCategory::Violence)
					{
						violenceLevel = L"Violence Score: " + winrt::to_hstring(value.Score());
					}
				}

				textBlock().Text(violenceLevel);
				ShowTextResult();
			}
			else
			{
				ShowResponseError(response.Error());
			}
		}
	}
#pragma endregion

	void MainPage::ShowTextResult()
	{
		textBlock().Visibility(Windows::UI::Xaml::Visibility::Visible);
		image().Visibility(Windows::UI::Xaml::Visibility::Collapsed);
	}

	void MainPage::ShowImageResult()
	{
		textBlock().Visibility(Windows::UI::Xaml::Visibility::Collapsed);
		image().Visibility(Windows::UI::Xaml::Visibility::Visible);
	}

	void MainPage::ShowResponseError(OpenAI::ResponseError const& error)
	{
		if (error == nullptr)
		{
			return;
		}

		textBlock().Text(L"Error:\n" + error.Message());
		ShowTextResult();
	}
}

