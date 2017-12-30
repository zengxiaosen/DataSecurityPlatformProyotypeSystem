#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

//求两个集合的交集
class print{
public:
    void operator()(int v){
        cout << v << " ";
    }
};

void test01(){
    vector<int> v1, v2, v3;
    for(int i=0; i< 10; i++){
        v1.push_back(i);
    }
    for(int i=5; i< 15; i++){
        v2.push_back(i);
    }

    v3.resize(min(v1.size(), v2.size()));
    vector<int>::iterator myEnd = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    for_each(v3.begin(), myEnd, print());
}

//求两个容器的并集
void test02(){
    vector<int> v1, v2, v3;
    for(int i=0; i< 10; i++){
        v1.push_back(i);
    }
    for(int i=5; i< 15; i++){
        v2.push_back(i);
    }
    v3.resize(v1.size()+v2.size());
    vector<int>::iterator myEnd = set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    for_each(v3.begin(), myEnd, print());

    
}



//求差集
void print(int v){
    cout << v << " ";
}
void test03(){
    vector<int> v1, v2, v3;
    for(int i=0; i< 10; i++){
        v1.push_back(i);
    }
    for(int i=5; i< 15; i++){
        v2.push_back(i);
    }

    v3.resize(v1.size());
    vector<int>::iterator myEnd = set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    for_each(v3.begin(), myEnd, print);
}


int main(){
    test03();
    return 0;
}