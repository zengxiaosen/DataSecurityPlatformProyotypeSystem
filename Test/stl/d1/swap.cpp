#include<iostream>
#include<vector>
using namespace std;

void print(vector<int>& v){
	for(vector<int>::iterator it = v.begin(); it!= v.end(); it++){
		cout << *it << " ";
	}
	cout << endl;
}

int main(){
	vector<int> v1, v2;
	for(int i=0; i< 5; i++){
		v1.push_back(i);
	}

	for(int i=6; i< 10; i++){
		v2.push_back(i);
	}

	print(v1);
	print(v2);

	v1.swap(v2);
	cout << "v1 and v2 swap , then : " << endl;
	print(v1);
	print(v2);

	vector<int> v;
	v.resize(100);
	cout << "rong liang: " << v.capacity() << endl;
	cout << "da xiao: " << v.size() << endl;

	v.clear();
	v.push_back(2);
	v.push_back(3);

	cout << "rong liang: " << v.capacity() << endl;
	cout << "da xiao: " << v.size() << endl;

	vector<int>(v).swap(v);
	cout << "rong liang: " << v.capacity() << endl;
	cout << "da xiao: " << v.size() << endl;
	return 0;



}