using System;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI;
using Microsoft.Web.WebView2.Core;
using System.Linq;
using CommunityToolkit.Mvvm.DependencyInjection;

namespace SampleApp.WinUI3.Views
{
    public sealed partial class PdfPage : Page
    {
        private string _lastSelectedPdfText = string.Empty;
        private readonly OpenAI.OpenAiService _openAiService;

        public PdfPage()
        {
            this.InitializeComponent();

            _openAiService = Ioc.Default.GetRequiredService<OpenAI.OpenAiService>();

            this.Loaded += PdfPage_Loaded;
        }

        private void PdfPage_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                // Avoid re-entrancy fail fast issue by enqueuing the call
                this.DispatcherQueue.TryEnqueue(async () =>
                {
                    await PreviewBrowser.EnsureCoreWebView2Async();
                });
            }
            catch(Exception) 
            {
                // todo: error log
            }
        }

        private void Navigate(StorageFile file)
        {
            if (file == null) 
            {
                return;
            }

            var uriPath = new Uri(file.Path);
            if (uriPath != null && PreviewBrowser.CoreWebView2 != null)
            {
                PreviewBrowser.CoreWebView2.Navigate(uriPath.ToString());
            }
        }

        private async void PreviewBrowser_CoreWebView2Initialized(WebView2 sender, CoreWebView2InitializedEventArgs args)
        {
            // transparent background when loading the page
            PreviewBrowser.DefaultBackgroundColor = Color.FromArgb(0, 0, 0, 0);

            // NOTE: settings to keep in mind, so far no need to tweak them
            //PreviewBrowser.CoreWebView2.Settings.IsWebMessageEnabled = true;
            //PreviewBrowser.CoreWebView2.Settings.IsScriptEnabled = true;
            //PreviewBrowser.CoreWebView2.Settings.AreDefaultScriptDialogsEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.AreDefaultContextMenusEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.AreDevToolsEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.AreHostObjectsAllowed = false;
            //PreviewBrowser.CoreWebView2.Settings.IsGeneralAutofillEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.IsPasswordAutosaveEnabled = false;   

            PreviewBrowser.CoreWebView2.Settings.HiddenPdfToolbarItems =
                Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.ZoomIn
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.ZoomOut
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Search
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.PageLayout
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.PageSelector
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Bookmarks
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.FitPage
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.FullScreen
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Rotate
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.SaveAs
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Print
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Save
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.MoreSettings;

            // Events
            PreviewBrowser.CoreWebView2.ContextMenuRequested += CoreWebView2_ContextMenuRequested;

            try
            {
                // TEST: uncomment if first page should be text HTML assets 
                //var htmlUri = new Uri("ms-appx:///Assets/Web/MainPage.html");
                //var htmlFile = await StorageFile.GetFileFromApplicationUriAsync(htmlUri);
                //PreviewBrowser.CoreWebView2.Navigate(htmlFile.Path);

                var pdfUri = new Uri("ms-appx:///Assets/Docs/Canadian_Rockies.pdf");
                var pdfFile = await StorageFile.GetFileFromApplicationUriAsync(pdfUri);
                Navigate(pdfFile);

                /* NOTE: Uncomment the following line if you would like to debug JavaScript code
                 * 
                 * To make sure the console for debugging Edge works properly:
                 * 1. Once the apps runs, make sure it navigates to the "MainPage.html" above
                 * 2. In the Edge debug console the web assets should be green (online circle)
                 * 3. Open the web files in the console (not in the VS) and add breakpoints
                 * 
                 * Note that if the code navigates away from the "MainPage.html" above it might cause
                 * the web assets to be unloaded (orange color) and breakpoints won't work.
                 */
                //PreviewBrowser.CoreWebView2.OpenDevToolsWindow();
            }
            catch(Exception ex) 
            {
                // todo:
            }
        }

        private void CoreWebView2_ContextMenuRequested(Microsoft.Web.WebView2.Core.CoreWebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2ContextMenuRequestedEventArgs args)
        {
            // Always clear all menu items to focus on the sample
            args.MenuItems.Clear();

            if (!args.ContextMenuTarget.HasSelection)
            {
                _lastSelectedPdfText = string.Empty;
                return;
            }

            _lastSelectedPdfText = args.ContextMenuTarget.SelectionText;
            if (!(string.IsNullOrWhiteSpace(_lastSelectedPdfText)))
            {
                // add new item to end of collection
                var dataExtractMenuItem = PreviewBrowser.CoreWebView2.Environment.CreateContextMenuItem(
                    "Send to ChatGPT",
                    null,
                    CoreWebView2ContextMenuItemKind.Command);

                dataExtractMenuItem.CustomItemSelected += DataExtractMenuItem_CustomItemSelected;
                args.MenuItems.Add(dataExtractMenuItem);
            }
        }

        private async void DataExtractMenuItem_CustomItemSelected(CoreWebView2ContextMenuItem sender, object args)
        {
            if (!string.IsNullOrWhiteSpace(_lastSelectedPdfText))
            {
                var chatReq = new OpenAI.Chat.ChatRequest();
                chatReq.Messages.Add(new OpenAI.Chat.ChatMessage("user", _lastSelectedPdfText));

                var result = await _openAiService.RunRequestAsync(chatReq);
                if (result.IsResponseSuccess)
                {
                    ContentDialog cntDialog = new();
                    cntDialog.XamlRoot = this.XamlRoot;

                    cntDialog.Content = result.Choices.First().Message.Content;
                    cntDialog.PrimaryButtonText = "Ok";

                    await cntDialog.ShowAsync();
                }

                _lastSelectedPdfText = string.Empty;
            }
        }

        private void PreviewBrowser_WebMessageReceived(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2WebMessageReceivedEventArgs args)
        {
            // TODO: get web message and parse it, ideally this will be the PDF text content
            var msg = args.WebMessageAsJson;
        }

        private async void btnOpenFile_Click(object sender, RoutedEventArgs e)
        {
            var filePicker = new FileOpenPicker();

            // Get the current window's HWND by passing in the Window object
            var hwnd = WinRT.Interop.WindowNative.GetWindowHandle((Application.Current as App).MainWindow);

            // Associate the HWND with the file picker
            WinRT.Interop.InitializeWithWindow.Initialize(filePicker, hwnd);

            filePicker.FileTypeFilter.Add(".pdf");

            var file = await filePicker.PickSingleFileAsync();
            if (file != null)
            {
                Navigate(file);
            }
        }

        // NOTE: testing events to comm between WebView2 and Native side.

        //private async void btnTest_Click(object sender, RoutedEventArgs e)
        //{
        //    try
        //    {
        //        JsonObject jsonObj = new JsonObject();
        //        jsonObj.Add("verb", JsonValue.Create("testEventCall"));
        //        jsonObj.Add("format", JsonValue.Create("text"));

        //        PreviewBrowser.CoreWebView2.PostWebMessageAsJson(jsonObj.ToJsonString());
        //    }
        //    catch(Exception ex) 
        //    {
        //        // TODO:
        //    }
        //}

        private async void PreviewBrowser_NavigationCompleted(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationCompletedEventArgs args)
        {
            // NOTE: only needed if using js/html code.
            //var scriptUri = new Uri("ms-appx:///Assets/Web/WebView2Events.js");
            //var scriptFile = await StorageFile.GetFileFromApplicationUriAsync(scriptUri);
            //var scriptContent = await FileIO.ReadTextAsync(scriptFile);

            //await PreviewBrowser.CoreWebView2.AddScriptToExecuteOnDocumentCreatedAsync(scriptContent);

            ///* NOTE: Registering the script must be called after the navigation has been completed to
            // * guarantee it will not get ignored in the midst of WebView2 processing other requests */
            //var result = await PreviewBrowser.CoreWebView2.ExecuteScriptAsync("registerEvents()");
        }
    }
}
