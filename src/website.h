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
            <p>The distance read is: <span id='adc'>-</span> cm</p>
        
            <script>

                let speech = new SpeechSynthesisUtterance();
                var Socket;
            
                function talk(msg) {
                    speech.text = msg;
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
                    if(output == 0) {
                        output = '0, An ERROR seems to have occured, please check your sensor'
                    }                
                    document.getElementById('adc').innerHTML = outpit;
    
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
