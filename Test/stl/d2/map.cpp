#include <iostream>
#include <map>
#include <string>
using namespace std;

void test01(){
	map<int, string> mymap;
	map<int, string> mymap2(mymap);
}

void test02(){
	map<int, int> mymap;
	mymap.insert(pair<int, int>(1, 5));
	pair<map<int, int>::iterator, bool> ret = mymap.insert(make_pair(2, 10));
	if(ret.second){
		cout << "cha ru cheng gong! " << endl;
	}else{
		cout << "cha ru shi bai! " << endl;
	}
	mymap.insert(map<int, int>::value_type(3, 15));
	mymap[4] = 20;

	mymap[2] = 100;
	for(map<int, int>::iterator it = mymap.begin(); it != mymap.end(); it++){
		cout << "key:" << it->first << " value: " << it->second << endl;
	}
	cout << endl;

	cout << "key:" << mymap[20] << endl;

	for(map<int, int>::iterator it = mymap.begin(); it != mymap.end(); it ++){
		cout << "key: " << it->first << " value: " << it->second << endl;
	}
	cout << endl;

	ret = mymap.insert(map<int, int>::value_type(2, 10));
	if(ret.second){
		cout << "cha ru cheng gong ! " << endl;
	}else{
		cout << "cha ru shi bai ! " << endl;
	}
}

void print(map<int, int>& mymap){
	for(map<int, int>::iterator it = mymap.begin(); it != mymap.end(); it++){
		cout << it->first << " " << it->second << " ";
	}
	cout << endl;
}


void test03(){
	map<int, int> mymap;
	mymap.insert(make_pair(1, 2));
	mymap.insert(make_pair(2, 3));
	mymap.insert(make_pair(3, 4));

	// find
	map<int, int>::iterator pos = mymap.find(3);
	if(pos == mymap.end()){
		cout <<  "not found ! " << endl;
	}else{
		cout << " found: " << pos->first << " value: " << pos->second << endl;
	}

	cout << "---------------------" << endl;
	pos = mymap.lower_bound(2);
	if(pos == mymap.end()){
		cout << "not found ! " << endl;
	}else{
		cout << "cha zhao dao: " << pos->first << " value: " << pos->second << endl;
	}

	pos = mymap.upper_bound(2);
	if(pos == mymap.end()){
		cout << "mei you zhao dao ! " << endl;
	}else{
		cout << "cha zhao dao: " << pos->first << " value: " << pos->second << endl;
	}

	pair<map<int, int>::iterator, map<int, int>::iterator> pos2 = mymap.equal_range(2);
	if(pos2.first == mymap.end()){
		cout << "mei you zhao dao! " << endl;
	}
	else{
		cout << "cha zhao dao: " << pos2.first->first << " value: " << pos2.first->second << endl;
	}

	if(pos2.second == mymap.end()){
		cout << "mei you zhao dao! " << endl;
	}else{
		cout << "cha zhao dao: " << pos2.second->first << " value: " << pos2.second->second << endl;
	}
}
int main(){
	test03();
	return 0;
}