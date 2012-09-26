var io = require('socket.io').listen(8000);

io.sockets.on('connection', function (socket) {

  socket.emit('news', { hello: 'world' });
  
	//	��¼
  socket.on('login', function (data) {
    console.log(data);
  });
  
	//	��ȡ��һ�����յĳ�ʼ����
  socket.on('next day', function (data) {
    console.log(data);
  });
  
  //	��ȡָ��ʱ�ε�tick����
  socket.on('get ticks', function (data) {
    console.log(data);
  });
  
  //	���׳ɽ�
  socket.on('do trade', function (data) {
    console.log(data);
  });
  
  //	��ѯ���׼�¼
  socket.on('trade record', function (data) {
    console.log(data);
  });
  	
  
  socket.on('my other event', function (data) {
    console.log(data);
  });


});