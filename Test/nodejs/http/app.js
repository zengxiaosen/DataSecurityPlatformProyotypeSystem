var http=require('http')
var server=http.createServer(function(req,res){
	res.writeHead(200,{'Content-Type':'text/html'});
	res.write('<h1>Node.js</h1>');
	res.end('<p>PCAT</p>');
})
server.listen(3000);
server.on('close',function(){
	console.log('server is close');
})
console.log('HTTP server is listening at port 3000');