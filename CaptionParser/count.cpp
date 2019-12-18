#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;
#define srt "/home/luuukas/Desktop/CompilerTheory/CaptionParser/subtitle.srt"
bool vis[1<<8] = {0};
int main(){
    ifstream fin(srt,std::ios::in);
    vis[' '] = true;
    if(fin.is_open()){
        char c;
        while(fin>>c){
            assert(c<(1<<8));
            vis[c] = true;
        }
        for(int i=0;i<(1<<8);++i){
            if(vis[i])
                cout << char(i) << "|";
        }
        cout << endl;
    }else{
        cout << "open error..." << endl;
    }
}