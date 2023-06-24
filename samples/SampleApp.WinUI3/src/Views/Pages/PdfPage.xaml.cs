using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI;

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

                    // transparent background when loading the page
                    PreviewBrowser.DefaultBackgroundColor = Color.FromArgb(0, 0, 0, 0);

                    PreviewBrowser.CoreWebView2.Settings.AreDefaultScriptDialogsEnabled = false;
                    PreviewBrowser.CoreWebView2.Settings.AreDefaultContextMenusEnabled = false;
                    PreviewBrowser.CoreWebView2.Settings.AreDevToolsEnabled = false;
                    PreviewBrowser.CoreWebView2.Settings.AreHostObjectsAllowed = false;
                    PreviewBrowser.CoreWebView2.Settings.IsGeneralAutofillEnabled = false;
                    PreviewBrowser.CoreWebView2.Settings.IsPasswordAutosaveEnabled = false;
                    PreviewBrowser.CoreWebView2.Settings.IsWebMessageEnabled = false;
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
    }
}
