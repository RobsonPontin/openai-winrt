using System;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using System.Text.Json.Nodes;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI;
using Windows.UI.WebUI;

namespace SampleApp.WinUI3.Views
{
    public sealed partial class PdfPage : Page
    {
        public PdfPage()
        {
            this.InitializeComponent();

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

            // NOTE: settings to keep in mind, so far not need to tweak
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
            PreviewBrowser.CoreWebView2.ProcessFailed += CoreWebView2_ProcessFailed;
            PreviewBrowser.CoreWebView2.ScriptDialogOpening += CoreWebView2_ScriptDialogOpening;

            try
            {
                // TODO: TEST: just for testing 
                var htmlUri = new Uri("ms-appx:///Assets/Web/MainPage.html");
                var htmlFile = await StorageFile.GetFileFromApplicationUriAsync(htmlUri);
                PreviewBrowser.CoreWebView2.Navigate(htmlFile.Path);

                // NOTE: Uncomment the following line if you would like to debug JavaScritp code
                PreviewBrowser.CoreWebView2.OpenDevToolsWindow();
            }
            catch(Exception ex) 
            {
                // todo:
            }
        }

        private void CoreWebView2_ScriptDialogOpening(Microsoft.Web.WebView2.Core.CoreWebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2ScriptDialogOpeningEventArgs args)
        {
            throw new NotImplementedException();
        }

        private void CoreWebView2_ProcessFailed(Microsoft.Web.WebView2.Core.CoreWebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2ProcessFailedEventArgs args)
        {
            throw new NotImplementedException();
        }

        private void PreviewBrowser_WebMessageReceived(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2WebMessageReceivedEventArgs args)
        {
            // TODO: get web message and parse it, ideally this will be the PDF text content
            var msg = args.WebMessageAsJson;
        }

        // TEST PLACE 

        private async void btnTest_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                JsonObject jsonObj = new JsonObject();
                jsonObj.Add("verb", JsonValue.Create("getPdfPageText"));
                jsonObj.Add("format", JsonValue.Create("text"));

                PreviewBrowser.CoreWebView2.PostWebMessageAsJson(jsonObj.ToJsonString());
            }
            catch(Exception ex) 
            {
                // TODO:
            }
        }

        private void PreviewBrowser_CoreProcessFailed(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2ProcessFailedEventArgs args)
        {

        }

        private async void PreviewBrowser_NavigationCompleted(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationCompletedEventArgs args)
        {
            // TODO: TEST: once navigation has completed we will add the script we need to extra PDF content
            var scriptUri = new Uri("ms-appx:///Assets/Web/WebView2Events.js");
            var scriptFile = await StorageFile.GetFileFromApplicationUriAsync(scriptUri);
            var scriptContent = await FileIO.ReadTextAsync(scriptFile);

            await PreviewBrowser.CoreWebView2.AddScriptToExecuteOnDocumentCreatedAsync(scriptContent);

            /* NOTE: Registering the script must be called after the navigation has been completed to
             * guarantee it will not get ignored in the midst of WebView2 processing other requests */
            var result = await PreviewBrowser.CoreWebView2.ExecuteScriptAsync("registerEvents()");
        }
    }
}
