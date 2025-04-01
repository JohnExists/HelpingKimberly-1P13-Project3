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
            <h1>Hello Kimberly!</h1>
            <button id='launch'>Press Me To Launch Program!</button>
            <h1>The value given is: <span id='adc'>-</span></h1>
        
            <script>

                let speech = new SpeechSynthesisUtterance();
                var Socket;
            
                function talk(msg) {
                    speech.text = msg;
                    if(msg === '10') speech.text = 'Start'
                    if(msg === '11') speech.text = 'Cancel'
                    window.speechSynthesis.cancel();
                    window.speechSynthesis.speak(speech);

                }
                function init() {
                    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
                    Socket.onmessage = function(event) {
                        processCommand(event);
                    };
                }
                function processCommand(event) {
                    output = event.data;
                    if(output === '10') output = 'START'
                    if(output === '11') output = 'CANCEL'
                    document.getElementById('adc').innerHTML = output;
    
                    talk(event.data);                    
                }
                document.getElementById('launch').onclick = function() {
                    talk('');
                    init();
                };
            </script>
        </body>
    </html>
)";

#endif
