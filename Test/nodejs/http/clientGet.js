var http=require('http');
var url=require('url');
var util=require('util');

http.createServer(function(req,res){
	console.log('qing qiu dao lai jie xi can shu');
	var params=url.parse(req.url,true);
	console.log('jie xi wan cehng');
	console.log(util.inspect(params));
	console.log('xiang kehu dua fan hui');
	res.end(params.query.name);
}).listen(3000);

var request=http.get({
	host:'localhost',
	path:'/user?name=marico&age=21',
	port:3000
}, function(res){
	res.setEncoding('utf-8');
	res.on('data',function(data){
		console.log('fu wu duan xaing ying hui lai de shu ju wei : '+data);
	})
});