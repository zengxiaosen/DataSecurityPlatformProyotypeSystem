#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
using namespace std;

class print{
public:
	print(): count(0){

	}
	void operator()(int v){
		count++;
		cout << v << endl;
	}
	int count;
};

void test01(){
	vector<int> v;
	for(int i=0; i< 10; i++){
		v.push_back(i);
	}
	print p1;
	print p2 = for_each(v.begin(), v.end(), p1);
	cout << "count: " << p1.count << endl;
	cout << endl;
	cout << "count: " << p2.count << endl;

}

class myplus100{
public:
	int operator()(int v){
		return v + 100;
	}
};

class myminute{
public:
	int operator()(int v1, int v2){
		return v2-v1;
	}
};

void test02(){
	// vector<int> v1, v2;
	// for(int i=0; i< 10; i++){
	// 	v1.push_back(i);
	// }
	// v2.resize(v1.size());
	// for_each(v2.begin(), v2.end(), p1);
	// transform(v1.begin(), v1.end(), v2.begin(), myplus100());
	// print p1;
	// for_each(v2.begin(), v2.end(), p1);

	vector<int> v1, v2, v3;
	for(int i=0; i< 10; i++){
		v1.push_back(i);
		v2.push_back(i+1);
	}

	v3.resize(v1.size());
	for_each(v3.begin(), v3.end(), print());
	cout << endl;

	transform(v1.begin(), v1.end(), v2.begin(), v3.begin(), myminute());

	for_each(v3.begin(), v3.end(), print());
	cout << endl;
}