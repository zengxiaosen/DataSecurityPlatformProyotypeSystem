#include <numeric>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//accumulate 将容器中的元素累加
void test01(){
    vector<int> v;
    for(int i=0; i<= 100; i++){
        v.push_back(i);
    }
    int n = accumulate(v.begin(), v.end(), 0);
    cout << "n: " << n << endl;

}

//fill algorithm
class print{
public:
    void operator()(int v){
        cout << v << " ";
    }
};

void test02(){
    vector<int> v;
    v.resize(10);
    fill(v.begin(), v.end(), 100);
    cout << "size:" << v.size() << endl;
    for_each(v.begin(), v.end(), print());
}

int main(){
    test02();
    return 0;
    
}