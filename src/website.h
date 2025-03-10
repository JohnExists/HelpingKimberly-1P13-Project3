#ifndef WEBSITE_H
#define WEBSITE_H

const char* INDEX_HTML = R"(
    <!DOCTYPE html>
    <html>
        <head>
            <title>ESP32 WiFi Station</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
        </head>
        <body>
            <h1>Simple ESP32 Web Server</h1>
            <button id='launch'>Press Me To Launch Program!</button>
            <p>The analog signal read is <span id='adc'>-</span></p>
        
            <script>

                 let speech = new SpeechSynthesisUtterance();
            
                function talk(msg) {
                    speech.text = msg;
                    window.speechSynthesis.cancel();
                    window.speechSynthesis.speak(speech);
                    console.log('Talking...');

                }
                var Socket;
                function init() {
                    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
                    Socket.onmessage = function(event) {
                        processCommand(event);
                    };
                }
                function processCommand(event) {
                    document.getElementById('adc').innerHTML = event.data;
                    talk(event.data);
                }
                document.getElementById('launch').onclick = function() {
                    init();
                };
            </script>
        </body>
    </html>
)";

#endif
