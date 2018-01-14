#include <iostream>
#include <string.h>
using namespace std;
template <typename T>
class SqQueue{
    public:
        SqQueue(int capacity);
        ~SqQueue();

        void push(T &data);
        void pop();
        T front();
        T back();
        int length();
    private:
        T *m_array;
        int m_capacity;
        int m_rear;
};


template <typename T>
int SqQueue<T>::length(){
    return m_rear + 1;
}

template <typename T>
T SqQueue<T>::back(){
    return m_array[m_rear];
}

template <typename T>
T SqQueue<T>::front(){
    return m_array[0];
}

template <typename T>
void SqQueue<T>::pop(){
    if(m_rear == -1){
        return;
    }
    for(int i=1; i<= m_rear; ++i){
        m_array[i-1] = m_array[i];
    }
    m_rear--;
}

template <typename T>
void SqQueue<T>::push(T &data){
    if(m_rear == m_capacity - 1){
        return;
    }
    m_rear ++;
    m_array[m_rear] = data;
}

template <typename T>
SqQueue<T>::~SqQueue(){
    delete []m_array;
}

template <typename T>
SqQueue<T>::SqQueue(int capacity){
    m_array = new T[capacity];
    m_capacity = capacity;
    m_rear = -1;
}

struct Value{
    int val;
};

void QueueTest(){
    Value v[10];
    SqQueue<Value> q(30);
    for(int i=0; i< 10; i++){
        v[i].val = i+10;
        q.push(v[i]);
        cout << "push element : " << v[i].val << endl;
    }

    while(q.length() > 0){
        Value t = q.front();
        cout << "stack top element: " << t.val << endl;
        q.pop();
    }
}

int main(){
    QueueTest();
    return 0;
}

