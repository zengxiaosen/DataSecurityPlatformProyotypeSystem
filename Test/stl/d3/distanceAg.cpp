#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
//distance
void test01(){
    vector<int> v;
    for(int i=10; i< 20; i++){
        v.push_back(i);
    }

    for(vector<int>::iterator it = v.begin(); it != v.end(); it++){
        int index = distance(v.begin(), it);
        cout << v[index] << " ";
    }
    cout << endl;
}

// for_each change zhi
void print(int& v){
    v = v+100;
}

void print2(const int& v){
    cout << v << "  ";
}

void test02(){
    vector<int> v;
    for(int i=10; i< 20; i++){
        v.push_back(i);
    }

    for_each(v.begin(), v.end(), print2);
    cout << endl;
    for_each(v.begin(), v.end(), print);
    for_each(v.begin(), v.end(), print2);
    cout << endl;
}

//for_each reverse bianli
void test03(){
    vector<int> v;
    for(int i=10; i< 20; i++){
        v.push_back(i);
    }
    for_each(v.begin(), v.end(), print2);
    cout << endl;
    for_each(v.rbegin(), v.rend(), print2);
}

int main(){
    test03();
    return 0;
}

