//
//  main.cpp
//  LR(0)
//
//  Created by yujin on 2019/11/19.
//  Copyright © 2019 yujin. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <assert.h>

#define Gfile "/home/yc/Desktop/DP/LR0/G.txt"

class grammer{
public:
    int gid;
    char left;
    std::string right;
    grammer(const int, const char, const std::string&);
};

const char DS = '#';
const int mxv = 100;

std::set<char> VN, VT, V;

// 字符与数字间的映射
std::map<char, int> charToId;
char idToChar[mxv];

int Vcnt = 0;
std::map<char, std::vector<int>> Gright;
std::vector<grammer> Gs;
char S = '\0';
int gcnt = 0;

void getRight(const std::string &);
void readGrammers();

struct program{
    int gid;
    int i = 0;    // 形象的说，句点在第i个字符前：i=3, xxx.x，i=0, .xxxx, i=4, xxxx.
    bool operator < (const program & oth) const{
        if(gid==oth.gid) return i<oth.i;
        return gid<oth.gid;
    }
};    // 项目及项目的状态

class state{
public:
    int sid;
    bool end = false;
    std::vector<program> Ps;    // 该状态下的所有项目
    std::set<char> right_VNs;
    bool findMore();
};

int derivateState(int,char);
void derivateAll(int);

enum motionType{
    UNKNOWN, ADDS, REDU, ACC, ERR, GOTO
};

struct cell{
    motionType mt = UNKNOWN;
    int nxtsid = -1;
    int gid = -1;
};

const int mxs = 100;
int scnt = 0;
std::vector<state> Ss;
std::map<std::vector<program>, int> PsToId;
cell table[mxs][mxv];

int generateI0();
void fillTable();
void printTable();

std::stack<int> stateS;
std::stack<char> charS;
std::stack<char> strS;
bool analyze(const std::string & str);

grammer::grammer(const int gid, const char left, const std::string& right):gid(gid),left(left),right(right){}

void getRight(const std::string &gram){
    int s;
    char left;
    for(s=0;s<gram.length()&&gram[s]==' ';++s);
    left = gram[s];
    if(S=='\0') S=left;    // 起始非终结符
    for(s=1;s<gram.length()&&gram[s]==' ';++s);
    assert(gram[s++]=='-');
    assert(gram[s++]=='>');
    for(int i=s;i<gram.length();++i){
        V.insert(gram[i]);    // 右部所有符号
    }
    if(!VN.count(left)){
        VN.insert(left);    // 非终结符
        charToId[left]=Vcnt;
        idToChar[Vcnt]=left;
        ++Vcnt;
    }
    Gs.emplace_back(gcnt,left,gram.substr(s,gram.length()-s));
    Gright[left].push_back(gcnt);
    ++gcnt;
}

bool state::findMore(){
    if(end) return false;
    bool found = false;
    for(auto & p : Ps){
        if(VN.count(Gs[p.gid].right[p.i])&&!right_VNs.count(Gs[p.gid].right[p.i])){    // 加入待归约项目
            right_VNs.insert(Gs[p.gid].right[p.i]);
            found = true;
            for(auto & gid : Gright[Gs[p.gid].right[p.i]]){
                Ps.push_back({gid, 0});
            }
        }
    }
    return found;
}

void printState(int sid){
    using std::cout;
    using std::endl;
    cout << "---------state---------" << endl;
    cout << "sid: " << sid << endl;
    cout << "isend: " << (Ss[sid].end?"true":"false") << endl << endl;
    for(auto & p : Ss[sid].Ps){
        cout << Gs[p.gid].left << "->";
        for(int i=0;i<p.i;++i){
            cout << Gs[p.gid].right[i];
        }
        cout << "`";
        for(int i=p.i;i<Gs[p.gid].right.length();++i){
            cout << Gs[p.gid].right[i];
        }
        cout << endl;
    }
    cout << "-----------------------" << endl;
}

int derivateState(int psid, char c){
    if(Ss[psid].end) return -1;
    
    state ts;
    bool isend = false;
    for(auto & p : Ss[psid].Ps){
        assert(p.i<Gs[p.gid].right.length());
        // 不应该出现归约-归约，移进-归约冲突
        assert(!isend||(isend&&Gs[p.gid].right[p.i]!=c));
        // ts.right_VNs.insert(Gs[p.gid].left);
        if(Gs[p.gid].right[p.i]==c){
            ts.Ps.push_back({p.gid,p.i+1});
            if(Gs[p.gid].right.length()==p.i+1){
                isend = ts.end = true;
            }
        }
    }
    if(ts.Ps.size()==0) return -1;
    ts.findMore();
    int sid;
    bool rec = false;
    if(PsToId.count(ts.Ps)){
        sid = PsToId[ts.Ps];
        rec = true;
    }else{
        PsToId[ts.Ps] = sid = scnt++;
        ts.sid = sid;
        Ss.push_back(ts);
        printState(sid);
    }
    
    if(!rec) derivateAll(sid);    // 这步递归调用有点危险
    
    return sid;
}

void derivateAll(int sid){
    if(Ss[sid].end) return;
    std::set<char> input_c;
    for(auto & p: Ss[sid].Ps){
        input_c.insert(Gs[p.gid].right[p.i]);
    }
    for(auto & c: input_c){
        int nxtsid = derivateState(sid,c);
        if(nxtsid==-1) continue;
        // assert(table[sid][charToId[c]].mt == UNKNOWN);
        if(VN.count(c)){    // 是非终结符
            table[sid][charToId[c]].mt = GOTO;
            table[sid][charToId[c]].nxtsid = nxtsid;
        }else{    // 是终结符
            table[sid][charToId[c]].mt = ADDS;
            table[sid][charToId[c]].nxtsid = nxtsid;
        }
    }
}

int generateI0(){
    Ss.emplace_back();
    
    Ss[0].right_VNs.insert(S);
    for(auto gid : Gright[S]){    // 理论上用拓展文法拓展后，仅有一条入口规则
        Ss[0].Ps.push_back({gid,0});
    }
    
    while (Ss[0].findMore());

    printState(0);
    
    PsToId[Ss[0].Ps] = scnt++;
    
    return 0;
}

void fillTable(){
    int ssid = generateI0();
    derivateAll(ssid);
    
    for(int sid=0;sid<PsToId.size();++sid){
        if(Ss[sid].end){    // 是终态
            assert(Ss[sid].Ps.size()==1);    // 终态有且仅有一个归约式
            if(Gs[Ss[sid].Ps[0].gid].right[Ss[sid].Ps[0].i-1]==DS){
                table[sid][charToId[DS]].mt = ACC;
            }else{
                // '#'
                table[sid][0].mt = REDU;
                table[sid][0].gid = Ss[sid].Ps[0].gid;
                for(int i=VN.size()+1;i<Vcnt;++i){
                    table[sid][i].mt = REDU;
                    table[sid][i].gid = Ss[sid].Ps[0].gid;
                }
            }
        }
    }
}

void printTable(){
    using std::cout;
    using std::endl;
    for(int j=0;j<Vcnt;++j){
            cout << j << ":" << idToChar[j] << " ";
        }
        cout << endl;
    for(int i=0;i<PsToId.size();++i){
        cout << i << "   ";
        for(int j=0;j<Vcnt;++j){
            cout << table[i][j].mt << "|" << table[i][j].nxtsid << "|" << table[i][j].gid << "   ";
        }
        cout << endl;
    }
}

void readGrammers(){
    VT.insert(DS);    // 终结符'#'
    charToId[DS]=Vcnt;
    idToChar[Vcnt]=DS;
    ++Vcnt;
    
    std::ifstream fin(Gfile,std::ios::in);
    std::string gram;
    if(fin.is_open()){
        while(getline(fin,gram)){
            getRight(gram);
        }
        for(auto c : V){
            if(!VN.count(c)&&!VT.count(c)){
                VT.insert(c);    // 终结符
                charToId[c]=Vcnt;
                idToChar[Vcnt]=c;
                ++Vcnt;
            }
        }
    }
}

bool analyze(const std::string & str){
    for(int i=(int)str.length()-1;i>=0;--i){
        strS.push(str[i]);
    }
    stateS.push(0);
    int top_s = 0;
    char top_c;
    while(!str.empty()){
        if(strS.size()==2&&strS.top()==S) return true;
        top_s = stateS.top();
        stateS.pop();
        top_c = strS.top();
        switch (table[top_s][charToId[top_c]].mt) {
            case ADDS:
                strS.pop();
                charS.push(top_c);
                stateS.push(top_s);
                stateS.push(table[top_s][charToId[top_c]].nxtsid);
                break;
            case REDU:
                for(int j=0;j<Gs[table[top_s][charToId[top_c]].gid].right.length();--j){
                    charS.pop();
                }
                strS.push(Gs[table[top_s][charToId[top_c]].gid].left);
                break;
            case GOTO:
                strS.pop();
                stateS.push(table[top_s][charToId[top_c]].nxtsid);
                break;
            default:
                return false;
                break;
        }
    }
    return true;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "begin to analyze...\n";
    
    readGrammers();
    fillTable();
    printTable();
    std::cout << analyze("acccd#") << std::endl;
    
    return 0;
}
