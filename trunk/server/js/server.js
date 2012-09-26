var io = require('socket.io').listen(8000);

io.sockets.on('connection', function (socket) {

  socket.emit('news', { hello: 'world' });
  
	//	登录
  socket.on('login', function (data) {
    console.log(data);
  });
  
	//	获取下一交易日的初始数据
  socket.on('next day', function (data) {
    console.log(data);
  });
  
  //	获取指定时段的tick数据
  socket.on('get ticks', function (data) {
    console.log(data);
  });
  
  //	交易成交
  socket.on('do trade', function (data) {
    console.log(data);
  });
  
  //	查询交易记录
  socket.on('trade record', function (data) {
    console.log(data);
  });
  	
  
  socket.on('my other event', function (data) {
    console.log(data);
  });


});