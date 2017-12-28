#include <QCoreApplication>
#include <vector>
#include <QDebug>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    vector<int> intArray = {1, 2, 3, 4, 5};
    qDebug() << "elements int vector:";
    for(auto it = intArray.begin(); it != intArray.end(); ++it){
        qDebug() << *it;
    }
    return a.exec();
}
