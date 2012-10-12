var gl=require("getline");
var fs=require("fs");

var gl=new gl.GetlineSync("SRX09_20120425.csv");

var db_options = {  
    host: 'oscar.iego.net',  
    port: 3306,  
    user: 'oscar',  
    password: 'f517278',  
    database: 'klineprint'  
};  
//加载mysql Module    
var mysql = require('mysql'),client = null;  
  
if(mysql.createClient) 
{  
    client = mysql.createClient(db_options);  
} 
else 
{  
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
    var arr = new Array();
    var lasttime ='00:00:00';
    
    for (var count = 1;; ++count) 
    {
        var str = gl.next();
        
        if(count==1)
            continue;
        
        var tmp_items = str.split(',');
        console.log(count + ":\t" + tmp_items[1]);
        
        if(tmp_items[1] != lasttime) 
        {
            //  将队列中的tick插入数据
            var tickInterval = 1000 / arr.length;
            
            for(var nCnt = 0; nCnt < arr.length; nCnt++)
            {
                var tickTimeInMs = nCnt * tickInterval;
                
                var items = arr[nCnt];
                
                client.query(  
                  'INSERT INTO tick '+  
                  'SET time = ?, price = ?, vol = ?, symbol = ?, month = ?, millisec = ?, interest = ?,' +
                  'b1price = ?, b1vol = ?, b2price = ?, b2vol = ?, b3price = ?, b3vol = ?,' +
                  's1price = ?, s1vol = ?, s2price = ?, s2vol = ?, s3price = ?, s3vol = ?, bs = ? ',  
                  [items[0]+' ' + items[1], items[2], items[3], 'SR', 'X09', tickTimeInMs, items[5], 
                   items[6], items[7], items[8], items[9], items[10], items[11],
                   items[12], items[13], items[14], items[15],items[16],items[17], items[18]]  
                );                
            }
            
            arr.length = 0;
        }

        arr.push(tmp_items);
        
        lasttime = tmp_items[1]; 
    }
}
catch(e)
{
    console.log(e);
    client.end();
}
