var http=require('http');
var querystring=require('querystring');
http.createServer(function(req,res){
	console.log('qing qiu dao lai, jie xi can shu');
	var post='';
	req.on('data',function(chunk){
		post+=chunk;
	});
	req.on('end',function(){
		post=querystring.parse(post);
		console.log('can shu jie xi wan cheng , fan hui name can shu');
		res.end(post.name);
	});
}).listen(3000);

var contents=querystring.stringify({
	name:'marico',
	age:21,
	address:'beijing'
});
var options={
	host:'localhost',
	path:'/',
	port:3000,
	method:'POST',
	headers:{
		'Content-Type':'application/x-www-form-urlencoded',
		'Content-Length':contents.length
	}
};

var req=http.request(options,function(res){
	res.setEncoding('utf-8');
	res.on('data',function(data){
		console.log('hou tai fan hui shu ju');
		console.log(data);
	})
});

req.write(contents);
req.end();
