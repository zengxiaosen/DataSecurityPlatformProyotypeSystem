#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define SALE_DEPARTMENT 1
#define DEVELOP_DEPARTMENT 2
#define FINACIAL_DEPARTMENT 3

class Yuangong{
public:
	string name;
	int age;
	string tele;
	double salary;
};

//create staff
void Create_Yuangong(vector<Yuangong>& v){
	string nameseed = "ABCDE";
	for(int i=0; i< 5; i++){
		Yuangong yg;
		yg.name = "Yuangong";
		yg.name += nameseed[i];
		yg.age = rand() % 30;
		yg.salary = rand() % 10000 + 10000;
		yg.tele = "+86-88888888";
		v.push_back(yg);
	}
}

void Set_YG_Group(vector<Yuangong>& v, multimap<int, Yuangong>& group){
	for(vector<Yuangong>::iterator it = v.begin(); it != v.end(); it++){
		cout << "dang qian yuan gong xin xi: " << endl;
		cout << "mingzi: " << it->name << " nianling: " << it->age << " gongzi: " << it->salary << " dianhua: " << it->tele << endl;
		int departmentID = -1;
		while(true){
			cout << "qing shu ru bu men(1 xiaoshoubu 2 kaifabu 3 caiwubu) : " << endl;
			scanf("%d", &departmentID);
			if(departmentID == SALE_DEPARTMENT){
				group.insert(make_pair(SALE_DEPARTMENT, *it));
				break;
			}
			else if(departmentID == DEVELOP_DEPARTMENT){
				group.insert(make_pair(DEVELOP_DEPARTMENT, *it));
				break;
			}else if(departmentID == FINACIAL_DEPARTMENT){
				group.insert(make_pair(FINACIAL_DEPARTMENT, *it));
				break;
			}else{
				cout << "shu ru cuo wu, qing chong xin shu ru:" << endl;
			}
		}
	}
}


//
void Show_YG_Info(multimap<int, Yuangong>& group){
	int departmentID = -1;
	while(true){
		cout << "qing shu ru cha kan de bu men(1 xiaoshoubu, 2 kaifabu, 3 caiwubu): " << endl;
		scanf("%d", &departmentID);

		if(departmentID < 1 || departmentID > 3){
			continue;
		}

		multimap<int, Yuangong>::iterator pos = group.find(departmentID);
		int ygcount = group.count(departmentID);
		int num = 0;
		while(pos != group.end() && num < ygcount){
			cout << "xingming: " << pos->second.name << " nianling: " << pos->second.age << " gongzi: " << pos->second.salary << " dianhua: " << pos->second.tele << endl;
			num ++;
			pos ++;
		}
	}
}

int main(){
	vector<Yuangong> v;
	multimap<int, Yuangong> Ygroup;
	Create_Yuangong(v);
	Set_YG_Group(v, Ygroup);
	Show_YG_Info(Ygroup);
	return 0;
}


