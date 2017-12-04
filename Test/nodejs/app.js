var http=require('http');
http.createServer(function(req,res){
	res.writeHead(200,{'Content-Type':'text/html'});
	res.write('<h1>Node.js wo men yi jing you fu wu le<h1>');
	res.end('<p>PCAT</p>');
}).listen(5858);
