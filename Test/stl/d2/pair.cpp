#include <iostream>
#include <string>
using namespace std;

int main(){
	pair<int, string> mypair(10, "aaa");
	cout << mypair.first << endl;
	cout << mypair.second << endl;

	pair<string, string> mypair2 = make_pair("aaa", "bbb");
	cout << mypair2.first << " " << mypair2.second << endl;

	pair<int, string> mypair3 = mypair;

	return 0;
}