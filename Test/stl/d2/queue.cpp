#include <iostream>
#include <queue>
#include <string>
using namespace std;

void test01(){
	queue<int> q1;
	q1.push(7);
	q1.push(3);
	q1.push(2);
	q1.push(9);

	if(q1.empty()){
		cout << "kong! " << endl;
	}
	else{
		cout << "not kong! " << endl;
	}

	queue<int> q2(q1);

	while(q1.size() > 0){
		int val = q1.front();
		cout << val << " ";
		q1.pop();
	}
}

class Teacher{
public:
	Teacher(string name, int age) :name(name), age(age){

	}
	string name;
	int age;
};

void test02(){
	queue<Teacher> q;
	Teacher t1("aaa", 10), t2("bbb", 20), t3("ccc", 30);

	q.push(t1);
	q.push(t2);
	q.push(t3);

	while(q.size() > 0){
		Teacher t = q.front();
		cout << "name: " << t.name << " age: " << t.age << endl;
		q.pop();
	}
}

void test03(){
	queue<Teacher*> q;
	Teacher t1("aaa", 10), t2("bbb", 20), t3("ccc", 30);
	Teacher* p1 = &t1;
	Teacher* p2 = &t2;
	Teacher* p3 = &t3;

	q.push(p1);
	q.push(p2);
	q.push(p3);

	while(q.size() > 0){
		Teacher* t = q.front();
		cout << "name: " << t->name << " age: " << t->age << endl;
		q.pop();
	}
}


int main(){
	test03();
	return 0;
}






