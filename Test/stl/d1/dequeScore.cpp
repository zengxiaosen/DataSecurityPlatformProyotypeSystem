#include<iostream>
#include<string>
#include<vector>
#include<deque>
#include<algorithm>
using namespace std;

class Player{
public:
	Player(string name, int score) :name(name), score(score){

	}
	string name;
	int score;
};

void Create_Player(vector<Player>& v){
	string nameseed = "ABCDE";
	for(int i=0; i< 5; i++){
		string name = "xuan shou ";
		name += nameseed[i];
		Player p(name, 0);
		v.push_back(p);
	}
}

bool mycompare(int v1, int v2){
	if(v1 > v2){
		return true;
	}
	else{
		return false;
	}
}

void Set_Player_Score(vector<Player>& plist){
	for(vector<Player>::iterator it = plist.begin(); it!= plist.end(); it++){
		deque<int> dscore;
		for(int i=0; i< 10; i++){
			int score = 50 + rand() % 50;
			dscore.push_back(score);
		}

		sort(dscore.begin(), dscore.end(), mycompare);

		dscore.pop_front();
		dscore.pop_back();

		int totalscore = 0;
		for(deque<int>::iterator dit = dscore.begin(); dit != dscore.end(); dit++){
			totalscore += *dit;
		}

		int scoreavg = totalscore / dscore.size();
		(*it).score = scoreavg;

	}
}

// show the 5 xuanshou score
void Show_Player_Score(vector<Player>& plist){
	for(vector<Player>::iterator it = plist.begin(); it != plist.end(); it++){
		cout << "name: " <<  it->name << " score: " << it->score << endl;
	}
	cout << endl;
}

int main(){
	vector<Player> vPlayer;
	Create_Player(vPlayer);
	Set_Player_Score(vPlayer);
	Show_Player_Score(vPlayer);
	return 0;
}




