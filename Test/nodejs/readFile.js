var fs=require('fs');
fs.readFile('content.txt',function(err,data){
	if(err){
		console.log(err);
	}else{
		console.log(data);
	}
});
fs.readFile('content.txt','UTF-8',function(err,data){
	if(err){
		console.log(err);
	}else{
		console.log(data);
	}
});

try{
	var data=fs.readFileSync('content.txt','UTF-8');
	console.log(data+"tong bu shi du qu");
}catch(e){
	console.log(e)
}