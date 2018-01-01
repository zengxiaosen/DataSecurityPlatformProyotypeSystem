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

//replace replace_if
class mycompare02{
public:
    bool operator()(int v){
        return v > 5;
    }
};

void test02(){
    vector<int> v1;
    for(int i=0; i< 10; i++){
        v1.push_back(i);
    }

    for_each(v1.begin(), v1.end(), print);
    cout << endl;
    replace(v1.begin(), v1.end(), 6, 100);
    for_each(v1.begin(), v1.end(), print);
    cout << endl;

    //replace_if
    replace_if(v1.begin(), v1.end(), mycompare02(), 50);
    for_each(v1.begin(), v1.end(), print);
    cout << endl;

    //swap
    vector<int> v2;
    for(int i=0; i< 10; i++) {
        v2.push_back(i);
    }


    cout << "jiao huan zhi qian : " << endl;
    for_each(v1.begin(), v1.end(), print);
    cout << endl;

    for_each(v2.begin(), v2.end(), print);
    cout << endl;

    swap(v1, v2);

    cout << "jiao huan zhi hou : " << endl;
    for_each(v1.begin(), v1.end(), print);
    cout << endl;

    for_each(v2.begin(), v2.end(), print);
    cout << endl;
}


int main(){
    test02();
    return 0;
}