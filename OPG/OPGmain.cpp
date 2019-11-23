#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <memory.h>
#include <stack>
#include <assert.h>
#include <iostream>
#include <algorithm>

#define Gfile "/home/yc/Desktop/编译原理/OPG/G.txt"

class grammer{
public:
    int gid;
    char left;
    std::string right;
    grammer(const int, const char, const std::string&);
};

const char DS = '#';
const int mxv = 100;
const int mxg = 100;

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

std::set<char> FIRSTVT[mxv];
std::set<char> LASTVT[mxv];
// 依次用于记录FIRSTVT集，LASTVT集间的依赖关系，用于生成拓扑排序
bool dep[mxv][mxv];
std::map<char,int> in_cnt;

enum relationship{
    UNKNOWN, EQUAL, LESS, GREATER
};

relationship table[mxv][mxv];

void initEQUAL();

void initFIRSTVT();

void derivateFIRSTVT();

void initLASTVT();

void derivateLASTVT();

void initLESS();

void initGREATER();

void fillTable();

void printTable();

void initEQUAL(){
    // 查看产生式的右部，找A->...ab...或A->...aBb....
    memset(table,UNKNOWN,sizeof(table));
    for(auto & g: Gs){
        for(int i = 0;i<g.right.length();++i){
            if(VN.count(g.right[i])) continue;    // 是非终结符
            if(i+1<g.right.length()&&!VN.count(g.right[i+1])){    // A->...ab...
                table[charToId[g.right[i]]][charToId[g.right[i+1]]] = EQUAL;
                continue;
            }
            if(i+2<g.right.length()&&!VN.count(g.right[i+2])){    // A->...aBb....
                table[charToId[g.right[i]]][charToId[g.right[i+2]]] = EQUAL;
                continue;
            }
        }
    }
}

void initFIRSTVT(){
    memset(dep,false,sizeof(dep));
    for(auto c : VN){
        in_cnt[c] = 0;
    }
    // A->a...或A->Ba...，a belong to FIRSTVT(A)
    // A->B...，FIRSTVT(A) include FIRSTVT(B)
    for(auto & g: Gs){
        assert(g.right.length()>0);
        if(!VN.count(g.right[0])){    // 产生式右部第一个字符是终结符
            FIRSTVT[charToId[g.left]].insert(g.right[0]);
            continue;
        }
        if(charToId[g.right[0]]!=charToId[g.left]&&!dep[charToId[g.right[0]]][charToId[g.left]]){
            dep[charToId[g.right[0]]][charToId[g.left]] = true;    // 产生式右部第一个字符是非终结符
            ++in_cnt[g.left];
        }
        if(g.right.length()>1&&!VN.count(g.right[1])){    // 产生式右部第二个字符是终结符
            FIRSTVT[charToId[g.left]].insert(g.right[1]);
        }
    }
    derivateFIRSTVT();
}

void derivateFIRSTVT(){
    std::queue<char> q;
    for(auto &e:in_cnt){
        if(e.second==0) q.push(e.first);
    }
    char A;
    while(!q.empty()){
        A = q.front();
        q.pop();
        for(int to = 0;to<Vcnt;++to){
            if(dep[charToId[A]][to]){
                dep[charToId[A]][to] = false;
                --in_cnt[idToChar[to]];
                if(in_cnt[idToChar[to]]==0) q.push(idToChar[to]);

                for(auto & fc : FIRSTVT[charToId[A]]){
                    FIRSTVT[to].insert(fc);
                }
            }
        }
    }
}

void initLASTVT(){
    memset(dep,false,sizeof(dep));
    for(auto c : VN){
        in_cnt[c] = 0;
    }
    // B->...a或B->...aC，a belong to LASTVT(B)
    // B->...C，LASTVT(A) include LASTVT(B)
    for(auto & g: Gs){
        assert(g.right.length()>0);
        if(!VN.count(g.right[g.right.length()-1])){    // 产生式右部最后一个字符是终结符
            LASTVT[charToId[g.left]].insert(g.right[g.right.length()-1]);
            continue;
        }
        if(charToId[g.right[g.right.length()-1]]!=charToId[g.left]&&!dep[charToId[g.right[g.right.length()-1]]][charToId[g.left]]){
            dep[charToId[g.right[g.right.length()-1]]][charToId[g.left]] = true;    // 产生式右部最后一个字符是非终结符
            ++in_cnt[g.left];
        }
        if(g.right.length()>1&&!VN.count(g.right[g.right.length()-2])){    // 产生式右部倒数二个字符是终结符
            LASTVT[charToId[g.left]].insert(g.right[g.right.length()-2]);
        }
    }
    derivateLASTVT();
}

void derivateLASTVT(){
    std::queue<char> q;
    for(auto &e:in_cnt){
        if(e.second==0) q.push(e.first);
    }
    char A;
    while(!q.empty()){
        A = q.front();
        q.pop();
        for(int to = 0;to<Vcnt;++to){
            if(dep[charToId[A]][to]){
                dep[charToId[A]][to] = false;
                --in_cnt[idToChar[to]];
                if(in_cnt[idToChar[to]]==0) q.push(idToChar[to]);

                for(auto & fc : LASTVT[charToId[A]]){
                    LASTVT[to].insert(fc);
                }
            }
        }
    }
}

void initLESS(){
    initFIRSTVT();
    // 产生式中终结符在前非终结符在后的相邻符号对
    for(auto & g: Gs){
        for(int i=0;i<g.right.length()-1;++i){
            if(!VN.count(g.right[i])&&VN.count(g.right[i+1])){
                for(auto fc : FIRSTVT[charToId[g.right[i+1]]]){
                    table[charToId[g.right[i]]][charToId[fc]] = LESS;
                }
            }
        }
    }
}

void initGREATER(){
    initLASTVT();
    // 产生式中非终结符在前终结符在后的相邻符号对
    for(auto & g: Gs){
        for(int i=0;i<g.right.length()-1;++i){
            if(VN.count(g.right[i])&&!VN.count(g.right[i+1])){
                for(auto fc : LASTVT[charToId[g.right[i]]]){
                    table[charToId[fc]][charToId[g.right[i+1]]] = GREATER;
                }
            }
        }
    }
}

void fillTable(){
    initEQUAL();
    initLESS();
    initGREATER();
}

void printTable(){
    using std::cout;
    using std::endl;
    cout << "     ";
    for(auto c : VT){
        cout << c << "    ";
    }
    cout << endl;
    for(auto c1: VT){
        cout << c1 << "    ";
        for(auto c2 : VT){
            cout << table[charToId[c1]][charToId[c2]] << "    ";
        }
        cout << endl;
    }
}

// 算符优先文法中，并不在乎归约时的非终结符是什么。
// 归约时只要与某条产生式右部“终结符位置对应且值相等，非终结符位置对应“，即可用该产生式进行归约。
char reduce(const std::string & right){
    for(auto & g : Gs){
        if(right.length()==g.right.length()){
            bool matched = true;
            int cond;
            for(int i=0;i<right.length();++i){
                cond = VN.count(right[i])+VN.count(g.right[i]);
                if(cond==1){
                    matched = false;
                    break;
                }else if(cond==0&&right[i]!=g.right[i]){
                    matched = false;
                    break;
                }
            }
            if(matched) return g.left;
        }
    }
    return '/0';
}

std::stack<char> charS;
std::stack<char> tS;
std::stack<relationship> relaS;
std::stack<char> strS;

bool analyze(const std::string & str){
    charS.push(DS);
    for(int i=str.length()-1;i>=0;--i){
        strS.push(str[i]);
    }
    char top_c, top_vt;
    std::string tr;
    while(!strS.empty()){
        top_c = strS.top();
        strS.pop();
        while(VN.count(charS.top())){
            tS.push(charS.top());
            charS.pop();
        }
        top_vt = charS.top();
        while(!tS.empty()){
            charS.push(tS.top());
            tS.pop();
        }
        switch (table[charToId[top_vt]][charToId[top_c]])
        {
        case EQUAL:
            charS.push(top_c);
            relaS.push(EQUAL);
            break;
        case LESS:
            charS.push(top_c);
            relaS.push(LESS);
            break;
        case GREATER:
            tr = "";
            strS.push(top_c);
            while(VN.count(charS.top())||(!relaS.empty()&&relaS.top()==EQUAL)){
                tr += charS.top();
                if(!VN.count(charS.top())) relaS.pop();
                charS.pop();
            }
            assert(!relaS.empty()&&relaS.top()==LESS);
            tr += charS.top();
            charS.pop();
            relaS.pop();
            while(VN.count(charS.top())){
                tr += charS.top();
                charS.pop();
            }
            std::reverse(tr.begin(),tr.end());
            charS.push(reduce(tr));
            break;
        default:
            return false;
            break;
        }
    }
    tr = "";
    while(!charS.empty()){
        tr += charS.top();
        charS.pop();
    }
    std::reverse(tr.begin(),tr.end());
    return reduce(tr)==S;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "begin to analyze...\n";
    
    readGrammers();
    fillTable();
    printTable();
    std::cout << analyze("(i)*(i)#") << std::endl;
    return 0;
}