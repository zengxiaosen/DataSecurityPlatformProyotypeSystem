#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

void print(int v){
    cout << v << " ";
}

void test01(){
    vector<int> v1, v2;
    for(int i=0; i< 10; i++){
        v1.push_back(i);
    }

    //copy
    v2.resize(v1.size());
    copy(v1.begin(), v1.end(), v2.begin());
    for_each(v2.begin(), v2.end(), print);
}

