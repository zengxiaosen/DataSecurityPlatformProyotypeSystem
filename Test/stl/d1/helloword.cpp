#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

void test01(){
	vector<int> v;
	v.push_back(10);
	v.push_back(4);
	v.push_back(5);
	v.push_back(4);

	vector<int>::iterator pStart = v.begin();
	vector<int>::iterator pEnd = v.end();

	while(pStart != pEnd){
		count << *pStart << " ";
		pStart++;
	}
	cout << endl;

	int n = count(v.begin(), v.end(), 4);
	cout << "n: " << n << endl;
}

class Teacher{
public:
	Teacher(int id, int age) :ID(id), age(age){

	}
	int ID;
	int age;
};

void test02(){
	vector<Teacher> v;
	Teacher t1(1, 2), t2(3, 4), t4(5, 6);

	v.push_back(t1);
	v.push_back(t2);
	v.push_back(t3);

	vector<Teacher>::iterator pStart = v.begin();
	vector<Teacher>::iterator pEnd = v.end();

	while(pStart != pEnd){
		Teacher t = *pStart;
		cout << "ID: " << t.ID << " Age: " << t.age << endl;
		pStart++;
	}
}

void test03(){
	vector<Teacher*> v;
	Teacher t1(1, 2), t2(3, 4), t3(5, 6);
	Teacher* p1 = &t1;
	Teacher* p2 = &t2;
	Teacher* p3 = &t3;

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);

	vector<Teacher*>::iterator pStart = v.begin();
	vector<Teacher*>::iterator pEnd = v.end();

	while(pStart != pEnd){
		Teacher* ttemp = *pStart;
		cout << ttemp->ID << " " << ttemp->age << endl;
		pStart++;
	}
	cout << endl;
}


int main(){
	test03();
	system("pause");
	return 0;
}
