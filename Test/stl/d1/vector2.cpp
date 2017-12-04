#include<iostream>
#include<vector>
using namespace std;

void test01(){
	vector<int> v;
	v.assign(5, 1);

	vector<int> v1;
	v1.assign(v.begin(), v.end());

	vector<int> v2;
	v2 = v1;
}

void test02(){
	int arr[] = {6, 8, 3, 9};
	vector<int> v(arr, arr+sizeof(arr)/sizeof(int));
	cout << "da xiao:" << v.size() << endl;
	cout << "rong liang:" << v.capacity() << endl;

	if(v.empty()){
		cout << "rong qi wei kong!" << endl;
	}else{
		cout << "rong qi bu wei kong!" << endl;
	}
}

void test03(){
	int arr[] = {6,8,3,9};
	vector<int> v(arr, arr+sizeof(arr)/sizeof(int));

	try{
		cout << v.at(100) << endl;
	}catch(...){
		cout << "shu zu fang wen yue jie!" << endl;
	}

	cout << "first:" << v.front() << endl;
	cout << "last:" << v.back() << endl;
}

void test04(){
	int arr[] = {6, 8, 3, 9};
	vector<int> v(arr, arr+sizeof(arr)/sizeof(int));
	v.insert(v.begin(), 100);
	v.push_back(100);

	vector<int>::iterator pStart = v.begin();
	vector<int>::iterator pEnd = v.end();

	while(pStart != pEnd){
		cout << *pStart << " ";
		pStart++;
	}
	cout << endl;
	int val = v.back();
	cout << val << "bei shan chu " << endl;
	v.pop_back();

	for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
		cout << *it << " ";
	}
	cout << endl;

	v.erase(v.begin());
	for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
		cout << *it << " ";
	}
	cout << endl;
	v.clear();
	cout << "size:" << v.size() << endl;
}
int main(){
	test04();
	return 0;
}

