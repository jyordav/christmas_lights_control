var state = null;
var connection;

function start(){
    connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

    connection.onopen = function () {
    connection.send("Success");
    };
    connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
    };
    connection.onmessage = function (e) {
    console.log('Server: ', e.data);
    };
    connection.onclose = function () {
    console.log('WebSocket connection closed');
    };
}

function turnReindeersOn(){
    state = "ON";
    connection.send(state);
}

function turnReindeersOff(){
    state = "OFF";
    connection.send(state);
}
