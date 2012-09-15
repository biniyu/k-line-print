var gl=require("getline");
var fs=require("fs");

var gl=new gl.GetlineSync("cu02_20040910.csv");

var db_options = {  
    host: 'localhost',  
    port: 3306,  
    user: 'root',  
    password: 'root',  
    database: 'klineprint'  
};  
//º”‘ÿmysql Module    
var mysql = require('mysql'),client = null;  
  
if(mysql.createClient) {  
    client = mysql.createClient(db_options);  
} else {  
    client = new mysql.Client(db_options);  
    client.connect(function(err) {  
        if(err) {  
            console.error('connect db ' + client.host + ' error: ' + err);  
            process.exit();  
        }  
    });  
}  

try
{
	var seq;
	var lasttime ='00:00:00';
	
  for (var count = 1;; ++count) {

	var str = gl.next();
	
	if(count==1)continue;
	
	var items = str.split(',');
	console.log(count + ":\t" + items[1]);
	
	if(items[1] != lasttime) 
	{
		seq=0;
	}
	else 
	{
		seq++;
	}

	client.query(  
      'INSERT INTO tick '+  
      'SET time = ?, price = ?, vol = ?, contract = ?, seq = ?, interest = ?,' +
      'b1price = ?, b1vol = ?, b2price = ?, b2vol = ?, b3price = ?, b3vol = ?,' +
      's1price = ?, s1vol = ?, s2price = ?, s2vol = ?, s3price = ?, s3vol = ?, bs = ? ',  
      [items[0]+' ' + items[1], items[2], items[3], 'cu02', seq, items[5], 
       items[6], items[7], items[8], items[9], items[10], items[11],
       items[12], items[13], items[14], items[15],items[16],items[17], items[18]]  
    );
        
  lasttime = items[1]; 

    }
}
catch(e)
{
	  console.log(e);
		client.end();
}
