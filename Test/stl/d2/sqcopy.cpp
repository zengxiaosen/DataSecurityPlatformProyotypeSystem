#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<vector>
using namespace std;

class Teacher{
public:
	Teacher(char* name,int age){
		
		int len = strlen(name) + 1;
		this->name = new char[len];  //在堆分配内存
		strcpy(this->name,name);

		this->age = age;
	}


	//拷贝构造
	Teacher(const Teacher& t){	
		int len = strlen(t.name) + 1;
		this->name = new char[len];
		strcpy(this->name, t.name);

		this->age = t.age;
	}

	//重载=
	Teacher& operator=(Teacher& t){

		int len = strlen(t.name) + 1;
		if (this->name != NULL){
			delete[] this->name;
		}
		this->name = new char[len];
		strcpy(this->name, t.name);

		this->age = t.age;

		return *this;
	}

	~Teacher(){
		if (this->name != NULL){
			delete[] this->name;
		}
		this->age = 0;
	}


	char* name;
	int age;
};


//测试函数  深拷贝和浅拷贝
void test01(){

	Teacher t1("aaa",20);
	vector<Teacher> v;
	v.push_back(t1);

}


int main(){


	test01();

	system("pause");
	return EXIT_SUCCESS;
}
