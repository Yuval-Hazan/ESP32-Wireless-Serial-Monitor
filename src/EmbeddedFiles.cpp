#include "EmbeddedFiles.h"

// Define the HTML content
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 Serial Monitor</title>
  <link rel="stylesheet" type="text/css" href="/style.css">
  <script src="/script.js"></script>
</head>
<body>
  <h2>ESP32 Wireless Serial Monitor</h2>
  <div id="serial"></div>
</body>
</html>
)rawliteral";

// Define the CSS content
const char style_css[] PROGMEM = R"rawliteral(
:root {
    --background-color: #121212;
    --serial-background-color: #1e1e1e;
    --blue: #0b4af9;
    --pink: #ed006c;
}

body {
    font-family: "Courier New", Courier, monospace;
    background-color: var(--background-color);
    color: var(--blue);
    margin: 0;
    padding: 24px; 
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    height: 100vh;
    box-sizing: border-box; 
}

h2 {
    color: var(--blue); 
    margin-bottom: 24px;
    text-align: center;
}

#serial {
    width: 80%; 

    height: 80%; 
    overflow-y: auto; 
    border: 1px solid var(--pink);
    padding: 12px;
    background-color: var(--serial-background-color);
    color: white;
    box-shadow: 0px 0px 12px black;
    box-sizing: border-box; 
}

::-webkit-scrollbar {
    width: 12px;
}

::-webkit-scrollbar-thumb {
    background: var(--blue);
}

::-webkit-scrollbar-thumb:hover {
    background: var(--pink);
}
)rawliteral";

// Define the JavaScript content
const char script_js[] PROGMEM = R"rawliteral(
var gateway = `ws://${window.location.hostname}:81/`;
var websocket;

window.addEventListener("load", onLoad);

function onLoad(event) {
    initWebSocket();
}

function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log("Connection opened");
}

function onClose(event) {
    console.log("Connection closed");
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    console.log(event.data);
    var serialDiv = document.getElementById("serial");
    serialDiv.innerHTML += event.data + "<br>";
    serialDiv.scrollTop = serialDiv.scrollHeight; // Auto-scroll to the bottom
}

)rawliteral";

// Function to serve the HTML content
void serveIndexHtml(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
}

// Function to serve the CSS content
void serveStyleCss(AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", style_css);
}

// Function to serve the JS content
void serveScriptJs(AsyncWebServerRequest *request) {
    request->send_P(200, "application/javascript", script_js);
}
