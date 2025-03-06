
#include<string>

const char* INDEX_HTML_OPEN = R"(
    <!DOCTYPE html>
    <html>
        <head>
            <title>ESP32 WiFi Station</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
        </head>
        <body>
            <h1>Simple ESP32 Web Server</h1>
        
            <script>
            let speech = new SpeechSynthesisUtterance();
            
            function talk() {
                speech.text = "1"
                // window.speechSynthesis.cancel()
                window.speechSynthesis.speak(speech)
                console.log("Talking....")
            }

            document.addEventListener("click", () => {
                var doIt = setInterval(talk, 250);
            });
            </script>

)";
const char* INDEX_HTML_CLOSE = R"(
        </body>
    </html>
)";
