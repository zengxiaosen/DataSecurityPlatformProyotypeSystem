#include<iostream>
#include<vector>
using namespace std;
int main(){
	int count = 0;
	int* p = NULL;
	vector<int> v;
	v.reserve(100000);

	for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
		cout << *it << " ";
	}
	cout << endl;

	for(int i=0; i< 100000; i++){
		v.push_back(i);
		if(p != &v[0]){
			count++;
			
		}
	}

	// for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
	// 	cout << *it << " ";
	// }
	// cout << endl;

	cout << "count:" << count << endl;
	cout << "rong liang: " << v.capacity() << endl;
	cout << "da xiao: " << v.size() << endl;

	vector<int> v2;
	for(int i=0; i< 5; i++){
		v2.push_back(i);
	}

	for(vector<int>::iterator it = v2.begin(); it != v2.end(); it++){
		cout << *it << " ";
	}
	cout << endl;

	v2.resize(3);
	for(vector<int>::iterator it = v2.begin(); it != v2.end(); it++){
		cout << *it << " ";
	}
	cout << endl;

	v2.resize(6);
	for(vector<int>::iterator it = v2.begin(); it != v2.end(); it++){
		cout << *it << " ";
	}
	cout << endl;




	return 0;
}