#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
using namespace std;

//merge
void print(int v){
    cout << v << " ";
}

void test02(){
    vector<int> v1, v2, v3;
    v1.push_back(6);
    v1.push_back(2);
    v1.push_back(8);
    v1.push_back(4);

    v2.push_back(1);
    v2.push_back(2);
    v2.push_back(6);
    v2.push_back(4);

    sort(v1.begin(), v1.end(), greater<int>());
    sort(v1.begin(), v2.end(), greater<int>());

    v3.resize(v1.size() + v2.size());
    for_each(v3.begin(), v3.end(), print);
    merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin(), greater<int>());
    for_each(v3.begin(), v3.end(), print);
}

void test03(){
    vector<int> v;
    v.push_back(6);
    v.push_back(2);
    v.push_back(8);
    v.push_back(4);

    sort(v.begin(), v.end(), greater<int>()); // 从大到小排序

}

//random_shuffle洗牌，将容器中的元素 顺序打乱
void test04(){
    vector<int> v;
    for(int i=0; i< 10; i++){
        v.push_back(i);
    }

    for_each(v.begin(), v.end(), print);
    cout << endl;
    random_shuffle(v.begin(), v.end());
    for_each(v.begin(), v.end(), print);
    cout << endl;
    random_shuffle(v.begin(), v.end());
    for_each(v.begin(), v.end(), print);
    cout << endl;
}

//反转容器中的元素
void test05(){
    vector<int> v;
    for(int i=0; i< 10; i++){
        v.push_back(i);
    }
    for_each(v.begin(), v.end(), print);
    cout << endl;
    reverse(v.begin(), v.end());
    for_each(v.begin(), v.end(), print);
    cout << endl;
}

int main(){
    test02();
    return 0;
}