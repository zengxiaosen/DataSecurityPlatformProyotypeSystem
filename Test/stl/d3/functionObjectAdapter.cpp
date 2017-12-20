#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class print : public binary_function<int, int, void>{
public:
	void operator()(int v, int v2) const{
		cout << "v:" << v << " v2:" << v2 << endl;
		if(v > v2){
			cout << v << " ";
		}
	}
};
//qu fan shi pei qi
struct mycompare02 : public unary_function<int, bool>{
	bool operator()(int v) const{
		return v>5;
	}
};

void test01(){
	vector<int> v;
	for(int i=0; i< 10; i++){
		v.push_back(i);
	}
	print p;
	for_each(v.begin(), v.end(), bind2nd(p, 5));
	cout << endl;

}

void test02(){
	vector<int> v;
	for(int i=0; i< 10; i++){
		v.push_back(i);
	}
	mycompare02 myp02;
	vector<int>::iterator pos = find_if(v.begin(), v.end(), not1(myp02));
	if(pos != v.end()){
		cout << "find:" << *pos << endl;
	}else{
		cout << "not found!" << endl;
	}
	greater<int> mygreater;
	//sort
	sort(v.begin(), v.end(), mygreater);

	for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
		cout << *it << " ";
	}

	cout << endl;

	sort(v.begin(), v.end(), not2(mygreater));

}

//ptr_fun: pu tong han shu shi pei qi
// struct print03{
// 	void operator()(int v){
// 		cout << v << " ";
// 	}
// };
void print03(int v, int v2){
	//cout << "v:" << v << "v2:" << v2 << endl;
	if(v > v2){
		cout << v << " ";
	}
}
void test03(){
	vector<int> v;
	for(int i=0; i< 10; i++){
		v.push_back(i);
	}
	//ptr_fun(print03);
	for_each(v.begin(), v.end(), bind2nd(ptr_fun(print03), 5));
}
class Teacher{
public:
	Teacher(int id, int age) : id(id), age(age){

	}
	int id;
	int age;
	void print(){
		cout << this->id << " " << this->age << endl;
	}
};
//mem_fun mem_fun_ref
class print04{
public:
	void operator()(){
		cout << t.id << " " << t.age << endl;
	}
};
void test04(){
	vector<Teacher> v;
	Teacher t1(1, 2), t2(3, 4), t3(5, 6);
	v.push_back(t1);
	v.push_back(t2);
	v.push_back(t3);
	//han shu dui xiang fang shi

	//for_each(v.begin(), v.end(), print04());
	for_each(v.begin(),v.end(), mem_fun_ref(&Teacher::print));

	vector<Teacher*> v2;
	Teacher* tt1 = new Teacher(1, 2);
	Teacher* tt2 = new Teacher(3, 4);
	Teacher* tt3 = new Teacher(5, 6);
	v2.push_back(t1);
	v2.push_back(t2);
	v2.push_back(t3);
	for_each(v2.begin(), v2.end(), mem_fun(&Teacher::print));

}