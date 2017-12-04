var fs=require('fs');
fs.readFile('file.txt','UTF-8',function(err, data){
	if(err){
		console.log('file read error');
	}else{
		console.log(data);
	}
})
console.log('end,');






























