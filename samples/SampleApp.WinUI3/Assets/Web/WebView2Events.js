
function registerEvents()
{
    window.chrome.webview.addEventListener('message', (event) => 
    {
        const payload = event.data
        if (payload.verb === 'testEventCall')
        {
            const testEventResponse = 'The test event was called succefully.';

            // Notify result
            window.chrome.webview.postMessage(testEventResponse);
        }
        else
        {
            alert(event.data);
        }
    });
}
