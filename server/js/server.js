var ws = require('websocket-server');

//Web Socket server
var server = ws.createServer();

// Handle WebSocket Requests
server.addListener("connection", function(conn){
	console.log('connection got', conn.id);
  conn.send('hello world!'); //发送后台数据
});

server.addListener("error", function(){
 console.log(Array.prototype.join.call(arguments, ", "));
});

server.addListener("disconnected", function(conn){
  console.log("disconnected");
});

server.listen(8000);

console.log('WebSocket server is running.');
console.log('Listening to port 8000.');