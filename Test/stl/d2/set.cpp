#include <iostream>
#include <functional>
#include <set>
#include <stdlib.h>
#include <time.h>

using namespace std;

void test01(){
	set<int> myset;
	set<int> myset2(myset);
}

void printSet(set<int>& myset){
	for(set<int>::iterator it = myset.begin(); it != myset.end(); it++){
		cout << *it << " ";
	}
	cout << endl;
}


void test02(){
	set<int> myset;
	myset.insert(4);
	myset.insert(2);
	myset.insert(1);
	myset.insert(5);
	myset.insert(2);

	printSet(myset);

	myset.erase(myset.begin());
	myset.erase(2);
	printSet(myset);
	myset.erase(myset.begin(), myset.end());
	cout << "size: " << myset.size() << endl;
}

template<class T>
class mycompare03{
public:
	bool operator()(T v1, T v2) const{
		return v1 > v2;
	}
};


void test03(){
	set<int, mycompare03<int>> myset;
	myset.insert(4);
	myset.insert(2);
	myset.insert(3);
	myset.insert(5);

	for(set<int, greater<int>>::iterator it = myset.begin(); it != myset.end(); it++){
		cout << *it << " ";
	}
	cout << endl;

	set<int>::iterator pos = myset.find(2);
	if(pos == myset.end()){
		cout << " mei you zhao dao ! " << endl;
	}else{
		cout << " find! " << *pos << endl;
	}
}


class Teacher{
public:
	Teacher(int id, int age) :id(id), age(age){

	}
	int id;
	int age;
};


class mycompare04{
public:
	bool operator()(Teacher t1, Teacher t2){
		return t1.id > t2.id;
	}
};

void test04(){
	set<Teacher, mycompare04> myset;
	Teacher t1(1, 2), t2(3, 4), t3(5, 6);

	myset.insert(t1);
	myset.insert(t2);
	myset.insert(t3);

	for(set<Teacher, mycompare04>::iterator it = myset.begin(); it != myset.end(); it++){
		cout << it->id << " " << it->age << endl;
	}
	cout << endl;

	set<Teacher, mycompare04>::iterator pos = myset.find(Teacher(3, 4));
	if(pos == myset.end()){
		cout << "mei you zhao dao ! " << endl;
	}else{
		cout << "cha zhao dao:" << pos->id << " : " << pos->age << endl;
	}
}

void test05(){
	set<int> myset;
	myset.insert(10);
	myset.insert(5);
	myset.insert(1);
	myset.insert(8);

	// fan hui da yu dengyu 5 de die dai qi
	set<int>::iterator pos = myset.lower_bound(5);
	if(pos == myset.end()){
		cout << "mei you zhao dao! " << endl;
	}else{
		cout << "zhao dao : " << *pos << endl;
	}
	pos = myset.upper_bound(5);
	if(pos == myset.end()){
		cout << "mei you zhao dao! " << endl;
	}else{
		cout << "zhao dao: " << *pos << endl;
	}

	cout << " ================================== " << endl;

	pair<set<int>::iterator, set<int>::iterator> pos2 = myset.equal_range(5);
	if(pos2.first == myset.end()){
		cout << "mei you zhao dao ! " << endl;
	}else{
		cout << "zhao dao ! " << *(pos2.first) << endl;
	}

	if(pos2.second == myset.end()){
		cout << "mei you zhao dao ! " << endl;
	}else{
		cout << "zhao dao ! " << *(pos2.second) << endl;
	}
}


int main(){
	test05();
	return 0;
}






























