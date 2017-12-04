#include<iostream>
using namespace std;

int mycount(int* pStart, int* pEnd, int value){
	int n=0;
	while(pStart != pEnd){
		if(*pStart == value){
			n++;
		}
		pStart++;
	}
	return n;
}

int main(){
	int arr[] = {3,2,7,5};
	int* pStart = arr;
	int* pEnd = &arr[sizeof(arr)/sizeof(int)];

	int n = mycount(pStart, pEnd, 2);
	cout << "n:" << n << endl;

	return 0;
}