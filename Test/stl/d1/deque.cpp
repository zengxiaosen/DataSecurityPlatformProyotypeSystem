#include<iostream>
#include<deque>
using namespace std;

void test01(){
	deque<int> d1;
	deque<int> d2(10, 5);
	deque<int> d3(d2.begin(), d2.end());
	deque<int> d4(d3);
}

void test02(){
	deque<int> d1(10, 3);
	deque<int> d;
	d = d1;
}

void test03(){
	deque<int> d1(10, 3);
	cout << d1.size() << endl;
	if(d1.empty()){
		cout << "kong !" << endl;
	}else{
		cout << "bu kong !" << endl;
	}
	d1.resize(15);
}

void test04(){
	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_back(30);
	d.push_back(40);

	for(int i=0; i< d.size(); i++){
		cout << d[i] << " ";
	}
	cout << endl;

	for(int i=0; i< d.size(); i++){
		cout << d.at(i) << " ";
	}
	cout << endl;

	for(deque<int>::iterator it = d.begin(); it != d.end(); it++){
		cout << *it << " ";
	}
	cout << endl;

	while(!d.empty()){
		cout << d.front() << " bei shan chu " << endl;
		d.pop_front();
	}
}

void test06(){
	deque<int> d;
	d.insert(d.begin(), 100);
	d.insert(d.end(), 200);

	for(deque<int>::iterator it=d.begin(); it != d.end(); it++){
		cout << *it << " ";
	}
	cout << endl;
}

int main(){
	test06();
	return 0;
}





