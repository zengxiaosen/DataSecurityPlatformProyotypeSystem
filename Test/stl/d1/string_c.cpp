#include<iostream>
#include<string>
using namespace std;

void test01(){
	string s = "abc";
	const char* str = s.c_str();
	char* str2 = "acbd";
	string s2(str2);
}

void test02(){
	string s;
	string s2 = "acdb";
	string s3(s2);
	string s4(10, 'c');

	cout << s2 << endl;
	cout << s3 << endl;
	cout << s4 << endl;

}

void test03(){
	string s;
	s = "abcd";

	string s2;
	s2.assign("pppp");

	string s3 = "abcdefg";
	for(int i=0; i< s3.size(); i++){
		cout << s3[i] << " ";
	}
	cout << endl;

	for(int i=0; i< s3.size(); i++){
		cout << s3.at(i) << " ";
	}
	cout << endl;

	try{
		cout << s3.at(100) << endl;
	}catch(...){
		cout << "fang wen yue jie!" << endl;
	}
}

void test04(){
	string s1 = "aaa";
	string s2 = "bbb";

	s1.append(s2);
	cout << s1 << endl;
}

void test05(){
	string s = "abcdefg";
	string target = "bd";
	int pos = s.find(target);
	char* target2 = "ef";
	int pos2 = s.find(target2);

	cout << "pos:" << pos << endl;
	cout << "pos2:" << pos2 << endl;

	int pos3 = s.rfind(target);
	cout << "pos3:" << pos3 << endl;

	string s1 = "acbd";
	s.replace(0, 2, s1);

	cout << s << endl;
}


void test06(){
	string s1 = "abc";
	string s2 = "abd";

	int ret = s1.compare(s2);
	if(ret == 1){
		cout << "s1 > s2" << endl;
	}
	else if(ret == -1){
		cout << "s1 < s2" << endl;
	}
	else{
		cout << "s1 == s2" << endl;
	}

	string s3 = "abcdefg";
	string s4 = s3.substr(0, 2);
	cout << "s4" << s4 << endl;

	string s5 = "abcd";
	s5.insert(0, "pppp");
	string s6 = "qqqq";
	s5.insert(s5.size(), s6);
	cout << s5 << endl;

	s5.erase(0,4);
	cout << s5 << endl;

}
int main(){
	test06();
	return 0;
}