#include <QCoreApplication>
#include <vector>
#include <QDebug>
#include <QVector>

using namespace std;
struct TagSt{
    int a, b, c;
};
#ifdef TEST_INITIALIZER_LIST
class ClassWithInitializerList{
public:
    ClassWithInitializerList(std::initializer_list<int> list){
        for(std::initializer_list<int>::const_iterator it = list.begin(); it != list.end(); it++){
            qDebug() << *it;
        }
    }
};
#endif


#ifdef TEST_STATIC_ASSERT
#define PI 3.14159
template<class T>
struct Check{
    static_assert(sizeof(int) <= sizeof(T), "T is not big enough!");
};
Check<long> testCheck;
#endif

#ifdef TEST_OVERRIDE_FINAL
class Base {
public:
  virtual ~Base(){

  }
  virtual int foo(int n){
        return n*n;
  }
  virtual void kiss(){

  }
};

class Derived : public Base{
public:
    virtual int foo (int n) override {
        return n*n*n;
    }
    virtual void kiss() override final {

    }

};

//class Derived2 : public Derived {
//public:
//    virtual void kiss(){

//    }
//};
#endif

#ifdef TEST_LAMBDA
class LambdaTester{
public:
    LambdaTester(const char *data) : m_data(data){

    }
    int dataSize() {
        return [this]()->int{ return this->m_data.size();}();
    }
    void addString(const char *str){
        //[=] means capture all wai bu bian liang as yi zhi de fang shi
        [=]{this->m_data.append(str);}();
    }
    void getDataSize(int &size){
        [&]{size = this->m_data.size();}();
    }
    void getDataSize2(int &size){
        [=, &size]{size = this->m_data.size();}();
    }


private:
    string m_data;
};
#endif






int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#ifdef TEST_LAMBDA
    LambdaTester t("hello World");
    qDebug() << "dataSize=" << t.dataSize();
    t.addString("WOW");
    qDebug() << "dataSize=" << t.dataSize();
    int size1 = 0;
    int size2 = 0;
    t.getDataSize(size1);
    t.getDataSize2(size2);
    qDebug() << "size1=" << size1 << " size2=" << size2;
#endif

#ifdef TEST_FOR
   int cArray[] = {3, 5, 2, 20, 9};
   qDebug() << "C style array:";
   int size = sizeof(cArray)/sizeof(cArray[0]);
   for(int i=0; i< size; ++i){
       qDebug() << cArray[i];
   }
   qDebug() << "Range-based for:";
   for(auto i : cArray){
       qDebug() << i;
   }
   qDebug() << "QVector:";
   QVector<int> numbers{2, 4, 6, 0, 1};
   for(auto i : numbers){
       qDebug() << i;
   }
   qDebug() << "map:";
   map<string, int> str2int{{"a", 1}, {"b", 2}, {"c", 3}};
   for(auto const &it : str2int){
       qDebug() << "key - " << it.first.c_str() << " value - " << it.second;
   }

#endif



#ifdef TEST_AUTO_DECLTYPE
    auto n = 3;
    auto bFlag = true;
    //QObject *pObj = new QWidget;
    map<string,int> str2int{{"a",1},{"b",2},{"c",3}};
    auto it = str2int.begin();
    for(; it != str2int.end(); it++){
        qDebug() << it->second;
    }
    //function point, lambda
    auto funcAdd = [](int a, int b){
        return a+b;
    };
    qDebug() << "add(12+20)=" << funcAdd(12, 20);

    decltype(n) m = 4; // typeof(m) = int
    decltype(it) end = str2int.end();
    decltype(str2int.begin()->first) strName = "zhangsanfeng";
    decltype(funcAdd) func2 = funcAdd;
    qDebug() << "func2(3+5)=" << func2(3, 5);


#endif



#ifdef TEST_STATIC_ASSERT
    static_assert((PI > 3.14 && PI < 3.15), "invalid PI");
#endif
    //nullptr
//    char *szName = nullptr;
//    int *p = nullptr;
    //bool bBig = nullptr;

#ifdef TEST_INITIALIZER_LIST
    qDebug() << "Custom class with initializer_list_constructor:";
    ClassWithInitializerList cwil = {1, 2, 3};

    qDebug() << "===";
    int arr[] = {1, 2, 3, 4};
    for(auto x: arr){
        qDebug() << x;
    }
    struct TagSt st = {2, 3, 4};
    vector<int> intArray = {1, 2, 3, 4, 5};
    qDebug() << "===";
    qDebug() << "elements int vector:";
    for(auto it = intArray.begin(); it != intArray.end(); ++it){
        qDebug() << *it;
    }
#endif
    return a.exec();
}
