var gateway = `ws://${window.location.hostname}:81/`;
var websocket;

window.addEventListener('load', onLoad);

function onLoad(event) {
  initWebSocket();
}

function initWebSocket() {
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log('Connection opened');
}

function onClose(event) {
  console.log('Connection closed');
  setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
  console.log(event.data);
  document.getElementById('serial').innerHTML += event.data + '<br>';
}
