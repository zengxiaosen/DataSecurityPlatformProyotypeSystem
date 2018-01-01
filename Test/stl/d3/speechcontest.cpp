#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <deque>
#include <numeric>
#include <functional>
using namespace std;

class Speaker{
public:
    string name;
    //三轮比赛，每一轮得分
    int score[3];
};

//创建选手
void Create_Speaker(map<int, Speaker>& mapSpeaker, vector<int>& v1){
    string nameseed = "ABCEDFGHIJKLMNOPQRSTUVWXYZ";
    for(int i=0; i< 24; i++){
        //创建选手对象
        Speaker speaker;
        speaker.name = "选手";
        speaker.name += nameseed[i];

        speaker.score[0] = 0;
        speaker.score[1] = 0;
        speaker.score[2] = 0;

        //创建选手唯一的参赛编号
        int SpeakerID = 100 + i;
        mapSpeaker.insert(make_pair(SpeakerID, speaker)); // 保存选手信息
        v1.push_back(SpeakerID);
    }
}


//选手抽签
void Random_SpeakerID(vector<int>& v1){
    random_shuffle(v1.begin(), v1.end());
}

//选手比赛
void Match_Speaker(int index, vector<int>& v1, map<int, Speaker>& mapSpeaker, vector<int>& v2){
    multimap<int, int, greater<int>> mapGroup;
    int indexTemp = 0;
    int indexGroup = 0;
    for(vector<int>::iterator it = v1.begin(); it != v1.end(); it++){
        indexTemp ++;
        deque<int> dscore; //保存评委10次得分
        for(int i=0; i< 10; i++){
            int score = 50 + rand() % 50;
            dscore.push_back(score);
        }
        //排序
        sort(dscore.begin(), dscore.end()); //从小到大排序
        //去除最高分，去除最低分
        dscore.pop_back();//去除最高分
        dscore.pop_front();//去除最低分

        int totalscore = accumulate(dscore.begin(), dscore.end(), 0);
        int scoreavg = totalscore / dscore.size();

        mapSpeaker[*it].score[index - 1] = scoreavg; //保存当前参赛选手的得分
        mapGroup.insert(map_pair(scoreavg, *it)); //保存分组信息

        if(indexTemp % 6 == 0){
            indexGroup ++;
            int highIndex = 0;
            printf("第%d轮比赛第%d组比赛成绩：\n", index, indexGroup);
            for(multimap<int, int, greater<int>>::iterator it = mapGroup.begin(); it != mapGroup.end(); it++){
                highIndex ++;
                if(highIndex <= 3){
                    v2.push_back(it->second);
                }
                cout << "姓名:" << mapSpeaker[it->second].name << "分数:" << mapSpeaker[it->second].score[index-1] << endl;
            }
            mapGroup.clear();
        }
    }


}

//查看比赛结果
void Show_Match_Result(int index, vector<int>& v2, map<int, Speaker>& mapSpeaker){
    printf("第%d轮比赛晋级名单：\n", index);
    for(vector<int>::iterator it = v2.begin(); it != v2.end(); it++){
        cout << "姓名：" << mapSpeaker[*it].name << "  分数:" << mapSpeaker[*it].score[index-1] << endl;
    }
}

int main(){
    map<int, Speaker> mapSpeaker; //保存选手的信息，根据编号
    vector<int> v1; // 保存第一轮参赛编号
    vector<int> v2; // 第一轮比赛的晋级编号
    vector<int> v3; // 保存第二轮比赛晋级名单
    vector<int> v4; // 保存最终的前三名的编号

    //创建选手， 24名
    Create_Speaker(mapSpeaker, v1);

    //第一轮比赛
    Random_SpeakerID(v1); // 选手抽签
    Match_Speaker(1, v1, mapSpeaker, v2); //选手比赛
    Show_Match_Result(1, v2, mapSpeaker); //查看比赛结果

    //第二轮比赛
    Random_SpeakerID(v2); //选手抽签
    Match_Speaker(2, v2, mapSpeaker, v3); //选手比赛
    Show_Match_Result(2, v3, mapSpeaker); //查看比赛结果

    //第三轮比赛
    Random_SpeakerID(v3); // 选手抽签
    Match_Speaker(3, v3, mapSpeaker, v4); //选手比赛
    Show_Match_Result(3, v4, mapSpeaker); //查看比赛结果

    return 0;


}