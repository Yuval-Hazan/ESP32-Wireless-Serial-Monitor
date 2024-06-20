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
