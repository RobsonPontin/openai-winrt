
function registerEvents()
{
    window.chrome.webview.addEventListener('message', (event) => {
        const payload = event.data
        if (payload.verb === 'getPdfPageText') {
            /* TODO: handle PDF page text copy.*/
            const pdfPageContent = 'copy pdf page content...'

            // Notify result
            window.chrome.webview.postMessage(pdfPageContent);
        }
        else {
            alert(event.data);
        }
    });
}