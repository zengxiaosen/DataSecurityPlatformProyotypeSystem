#include<iostream>
#include<stack>
#include<string>
using namespace std;

void test01(){
	stack<int> mystack;
	mystack.push(8);
	mystack.push(2);
	mystack.push(7);
	mystack.push(0);

	stack<int> mystack2(mystack);

	if(mystack.empty()){
		cout << "mystack empty" << endl;
	}else{
		cout << "mystack is not empty" << endl;
	}

	while(mystack.size() > 0){
		int val = mystack.top();
		cout << val << " ";
		mystack.pop();
	}

	cout << endl;


}

class Teacher{
	public:
		Teacher(string name, int age) :name(name), age(age){

		}
		string name;
		int age;
};

void test02(){
	stack<Teacher> mystack;
	Teacher t1("aaa", 10), t2("bbb", 20), t3("ccc", 30);

	mystack.push(t1);
	mystack.push(t2);
	mystack.push(t3);

	while(mystack.size() > 0){
		Teacher t = mystack.top();
		cout << "name: " << t.name << " age: " << t.age << endl;
		mystack.pop();
	}
}

void test03(){
	stack<Teacher*> mystack;
	Teacher t1("aaa", 10), t2("bbb", 20), t3("ccc", 30);

	Teacher* p1 = &t1;
	Teacher* p2 = &t2;
	Teacher* p3 = &t3;

	mystack.push(p1);
	mystack.push(p2);
	mystack.push(p3);

	while(mystack.size > 0){
		Teacher* p = mystack.top();
		cout << "name: " << p->name << " age: " << p->age << endl;
		mystack.pop();
	}
	cout << endl;
}

int main(){
	test03();
	return 0;
	
}