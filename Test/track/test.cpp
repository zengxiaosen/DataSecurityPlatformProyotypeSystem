#include <iostream>
#include <string.h>
using namespace std;
template <typename T>
struct ListNode{
    T data;
    ListNode *next;
};

template <typename T>
class LinkList{
public:
    LinkList();
    ~LinkList();

    void insert(int pos, T &data);
    T del(int pos);
    T get(int pos);
    void clear();
    int length();

private:
    ListNode<T> *m_head;
    int m_length;

};

struct Student{
    int age;
    string name;
};


template <typename T>
LinkList<T>::LinkList(){
    m_head = new ListNode<T>;
    m_head->next = NULL;
    m_length = 0;
    cout << "LinkList 构造完成！" << endl;
}

template <typename T>
LinkList<T>::~LinkList(){
    clear();
    //释放头节点
    delete m_head;
    cout << "LinkList 析构完毕！" << endl;
}

template <typename T>
void LinkList<T>::insert(int pos, T &data){
    if(pos < 0 || pos > m_length){
        return;
    }
    ListNode<T>* pCur = m_head;
    for(int i=0; i< pos; ++i){
        pCur = pCur->next;
    }
    ListNode<T>* pnew = new ListNode<T>;
    pnew->data = data;
    pnew->next = pCur->next;
    pCur->next = pnew;
    m_length++;
}

template <typename T>
T LinkList<T>::del(int pos){
    //辅助指针
    ListNode<T>* pCur = m_head;
    for(int i=0; i< pos; ++i){
        pCur = pCur->next;
    }
    ListNode<T>* pDel = pCur->next;
    T t = pDel->data;
    pCur->next = pDel->next;
    //释放内存
    delete pDel;
    m_length--;
    return t;
}

template <typename T>
T LinkList<T>::get(int pos){
    ListNode<T>* pCur = m_head;
    for(int i=0; i<= pos; ++i){
        pCur= pCur->next;
    }
    return pCur->data;
}

template <typename T>
void LinkList<T>::clear(){
    while(m_length){
        del(0);
    }
}

template <typename T>
int LinkList<T>::length(){
    return m_length;
}






void LinkListTest(){
    //创建对象
    LinkList<Student> list;
    Student stu[10];
    //初始化
    for(int i=0; i< 10; i++){
        stu[i].age = i + 20;
        stu[i].name = "Student";
        list.insert(i, stu[i]);
    }

    for(int i=0; i< list.length(); i++){
        Student s = list.get(i);
        cout << "student age : " << s.age << endl;
    }

    //删除所有节点
    while(list.length() > 0){
        Student s = list.del(0);
        cout << "delete --- Student age: " << s.age << endl;
    }

}

int main(){

    LinkListTest();
    cout << "Keyboard not found , pree F1 to continue..." << endl;
    return 0;
}
