var websocketObj = {
}

$(function(){

	if(window["WebSocket"])
	{
			websocketObj.socket = new WebSocket("ws://127.0.0.1:8000");
			
			websocketObj.socket.onopen = function(e){
				console.log('websocket connection established.');
			};
			
			websocketObj.socket.onmessage = function(e) {
				console.log(e.data);
			};
			
			websocketObj.socket.onclose = function(e) {
				console.log('websocket connection closed.');
			};
	}
});