#include<iostream>
using namespace std;
int main(){
	string email = "zhaosi@itcast.cn";

	int pos1 = email.find("@");
	int pos2 = email.find(".");
	if(pos1 == -1 || pos2 == -1){
		cout << "email not contain @ or . !" << endl;
		system("pause");
		return 0;
	}

	if(pos1 > pos2){
		cout << "input email abnormal! " << endl;
	}

	string username = email.substr(0, pos1);
	string::iterator pStart = username.begin();
	string::iterator pEnd = username.end();

	while(pStart != pEnd){
		if(*pStart < 97 || *pStart > 122){
			cout << "input char contain some char that is not small" << endl;
			system("pause");
			return 0;
		}
		pStart++;
	}
	cout << endl;

	string rightEmail = "zhaosi@itcast.cn";
	int ret = email.compare(rightEmail);
	if(ret != 0){
		cout << "email is not contain!" << endl;
		system("pause");
		return 0;

	}
	cout << "email is correct" << endl;
	system("pause");
	return 0;

}