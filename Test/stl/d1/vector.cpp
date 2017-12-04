#include<iostream>
#include<vector>
using namespace std;
void test01(){
	vector<int> v;
	vector<int> v2(10,5);
	vector<int> v3(v2.begin(), v2.end());
	vector<int> v4(v3);

	int arr[] = {2,3,6,7};
	vector<int> v5(arr,arr+ sizeof(arr)/sizeof(int));

	for(int i=0; i< v4.size(); i++){
		cout << v4[i] << " ";
	}
	cout << endl;

	for(int i=0; i< v5.size(); i++){
		cout << v5[i] << " ";
	}
	cout << endl;
}
int main(){
	test01();
	return 0;
}