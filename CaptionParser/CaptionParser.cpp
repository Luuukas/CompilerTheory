#include <fstream>
#include <iostream>
#include <memory.h>
#include <assert.h>
#include <queue>
#include <list>
#include <iomanip>
using namespace std;
#define srt "/home/luuukas/Desktop/CompilerTheory/CaptionParser/test.srt"
#define ROK 0
fstream fin(srt, std::ios::in);
int idx = 0;
string line = "";
bool iseof(){
    return fin.eof();
}
bool open()
{
    fstream fin(srt, std::ios::in);
    return fin.is_open();
}
int advancec()
{
    if (idx >= line.length())
    {
        if (getline(fin, line))
        {
            line.push_back('\n');
            idx = 0;
        }
        else
        { // EOF
            return EOF;
        }
    }
    else
    {
        ++idx;
    }
    return ROK;
}
char nowc()
{
    if(idx>=line.length()) advancec();
    return line[idx];
}
#define epsilon '\0'
const int mxs = 26;
const int mxc = (1 << 8);
int nxts[mxs][mxc];
bool cane[mxs];
void fillR(int s, int v)
{
    for (int i = 0; i < mxc; ++i)
    {
        if ((!(i >= '0' && i <= '9')) && i != '<' && i != '/' && i != 'i' && i != '>' && i != ':' && i != ',' && i != '-' && i != '\n' && i != ' ')
        {
            nxts[s][i] = v;
        }
    }
}
void fillN(int s, int v)
{
    for (int i = '0'; i <= '9'; ++i)
    {
        nxts[s][i] = v;
    }
}
void fillC0(int s, int v)
{
    fillR(s, v);
    nxts[s]['/'] = v;
    nxts[s]['i'] = v;
    nxts[s]['>'] = v;
}
void fillC1(int s, int v)
{
    fillR(s, v);
    nxts[s]['/'] = v;
    nxts[s]['i'] = v;
    nxts[s]['>'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s]['-'] = v;
    nxts[s][' '] = v;
}
void fillC2(int s, int v)
{
    fillR(s, v);
    fillN(s, v);
    nxts[s]['<'] = v;
    nxts[s]['>'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s]['-'] = v;
    nxts[s][' '] = v;
}
void fillC3(int s, int v)
{
    fillR(s, v);
    fillN(s, v);
    nxts[s]['/'] = v;
    nxts[s]['i'] = v;
    nxts[s]['>'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s]['-'] = v;
    nxts[s][' '] = v;
}
void fillC4(int s, int v)
{
    fillR(s, v);
    fillN(s, v);
    nxts[s]['<'] = v;
    nxts[s]['/'] = v;
    nxts[s]['>'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s]['-'] = v;
    nxts[s][' '] = v;
}
void fillC5(int s, int v)
{
    fillR(s, v);
    fillN(s, v);
    nxts[s]['<'] = v;
    nxts[s]['/'] = v;
    nxts[s]['i'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s]['-'] = v;
    nxts[s][' '] = v;
}
void fillC6(int s, int v)
{
    fillR(s, v);
    fillN(s, v);
    nxts[s]['<'] = v;
    nxts[s]['/'] = v;
    nxts[s]['i'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s]['-'] = v;
    nxts[s][' '] = v;
}
void fillC7(int s, int v){
    fillR(s,v);
    fillN(s,v);
    nxts[s]['<'] = v;
    nxts[s]['/'] = v;
    nxts[s]['i'] = v;
    nxts[s]['>'] = v;
    nxts[s][':'] = v;
    nxts[s][','] = v;
    nxts[s][' '] = v;
}
void initNxts()
{
    memset(nxts, -1, sizeof(nxts));
    memset(cane, false, sizeof(cane));
    // go from state 0
    nxts[0]['0'] = 1;
    for (int i = '1'; i <= '5'; ++i)
        nxts[0][i] = 2;
    for (int i = '6'; i <= '9'; ++i)
        nxts[0][i] = 3;
    nxts[0]['<'] = 4;
    nxts[0][':'] = 6;
    nxts[0][','] = 7;
    nxts[0]['-'] = 8;
    nxts[0][' '] = nxts[0]['\n'] = 9;
    fillC0(0, 5);
    // go from state 1
    cane[1] = true;
    fillN(1, 10);
    nxts[1]['<'] = 4;
    nxts[1]['\n'] = 11;
    fillC1(1, 5);
    // go from state 2
    cane[2] = true;
    fillN(2, 12);
    nxts[2]['<'] = 4;
    nxts[2]['\n'] = 11;
    fillC1(2, 5);
    // go from state 3
    cane[3] = true;
    fillN(3, 13);
    nxts[3]['<'] = 4;
    nxts[3]['\n'] = 11;
    fillC1(3, 5);
    // go from state 4
    nxts[4]['/'] = 14;
    nxts[4]['i'] = 15;
    nxts[4]['\n'] = 11;
    fillC2(4, 5);
    // go from state 5
    nxts[5]['<'] = 4;
    nxts[5]['\n'] = 11;
    fillC3(5, 5);
    // go from state 6
    cane[6] = true;
    // go from state 7
    cane[7] = true;
    // go from state 8
    nxts[8]['-'] = 16;
    nxts[8]['\n'] = 11;
    fillC7(8,5);
    // go from state 9
    cane[9] = true;
    // go from state 10
    cane[10] = true;
    nxts[10]['<'] = 4;
    nxts[10]['\n'] = 11;
    fillN(10, 17);
    // go from state 11
    nxts[11]['<'] = 4;
    nxts[11]['\n'] = 18;
    fillC3(11, 5);
    // go from state 12
    cane[12] = true;
    fillN(12, 19);
    nxts[12]['<'] = 4;
    nxts[12]['\n'] = 11;
    fillC1(12, 5);
    // go from state 13
    cane[13] = true;
    fillN(13, 19);
    nxts[13]['<'] = 4;
    nxts[13]['\n'] = 11;
    fillC1(13, 5);
    // go from state 14
    nxts[14]['i'] = 20;
    nxts[14]['\n'] = 11;
    fillC4(14, 5);
    // go from state 15
    nxts[15]['>'] = 21;
    nxts[15]['\n'] = 11;
    fillC5(15, 5);
    // go from state 16
    nxts[16]['>'] = 22;
    nxts[16]['\n'] = 11;
    fillC5(16,5);
    // go from staet 17
    cane[17] = true;
    nxts[17]['<'] = 4;
    nxts[17]['\n'] = 11;
    fillC3(17, 5);
    // go from state 18
    cane[18] = true;
    // go from state 19
    cane[19] = true;
    nxts[19]['<'] = 4;
    nxts[19]['\n'] = 11;
    fillN(19, 23);
    fillC1(19, 5);
    // go from state 20
    nxts[20]['>'] = 24;
    nxts[20]['\n'] = 11;
    fillC6(20, 5);
    // go from state 21
    cane[21] = true;
    // go from state 22
    cane[22] = true;
    // go from state 23
    cane[23] = true;
    fillN(23, 23);
    nxts[23]['<'] = 4;
    nxts[23]['\n'] = 11;
    fillC1(23, 5);
    // go from state 24
    cane[24] = true;
}
void printNxts(){
    cout << setw(5) << "";
    cout << setw(5) << "\\n";
    for(int i=32;i<128;++i){
        cout << setw(5) << char(i);
    }
    cout << endl;
    for(int j=0;j<=24;++j){
        cout << setw(1) << cane[j];
        cout << setw(4) << j;
        cout << setw(5) << nxts[j]['\n'];
        for(int i=32;i<128;++i){
            cout << setw(5) << nxts[j][i];
        }
        cout << endl;
    }
}
enum EXPECTION
{
    AW2F0N = (1 << 10) | (1 << 12) | (1 << 13),
    AW3F0N = (1 << 17) | (1 << 19),
    ASPAN = (1 << 22),
    ASPLIT = (1 << 9),
    ACOLON = (1 << 6),
    ACOMMA = (1 << 7),
    ASTRING = (1 << 18),
    AHIGHTS = (1 << 21),
    AHIGHTE = (1 << 24),
    AINT = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 12) | (1 << 13) | (1 << 19) | (1 << 23)
};
// int exp = AINT|ASPLIT;
queue<unsigned int> EXPS;
enum TYPE
{
    INT,
    STRING,
    CHAR,
    ERROR
};
struct node
{
    TYPE t;
    void *p;
    unsigned int e;    // 终结的结点编号
};
TYPE gett[mxs] = {ERROR,INT,INT,INT,ERROR,
ERROR,CHAR,CHAR,ERROR,CHAR,
INT,ERROR,INT,INT,ERROR,
ERROR,ERROR,INT,STRING,INT,
ERROR,STRING,STRING,INT,STRING,
ERROR};
void printNode(node tnode){
    switch(tnode.t){
        case INT:
            cout << *(int*)tnode.p << endl;
            break;
        case CHAR:
            cout << *(char*)tnode.p << endl;
            break;
        case STRING:
            cout << *(string*)tnode.p << endl;
            break;
        case ERROR:
            cout << "ERROR" << endl;
            break;
    }

}
node DFA(unsigned int exp)
{
    if(iseof()) return {CHAR, new char('#'), 0};
    TYPE t;
    void *p = nullptr;
    int state = 0;
    bool matched = false;
    list<char> chars;
    while (true)
    {
        if (nxts[state][nowc()] != -1 && ((1<<nxts[state][nowc()]) & exp))
        {
            matched = true;
        }
        else if (matched)
        {
            break;
        }
        if(nxts[state][nowc()]==-1) return {ERROR, nullptr, 0};
        chars.push_back(nowc());
        state = nxts[state][nowc()];
        if (advancec() == EOF){
            if(matched) break;
            else return {ERROR, nullptr, 0};
        }
    }
    t = gett[state];
    switch (t)
    {
    case INT:
        p = new int(0);
        for(auto c : chars){
            *(int*)p = (*(int*)p)*10+(c-'0');
        }
        break;    
    case CHAR:
        p = new char(*chars.begin());
        break;
    case STRING:
        p = new string("");
        for(auto c : chars){
            ((string*)p)->push_back(c);
        }
        break;
    }
    // printNode({t, p, (1 << state)});
    return {t, p, (1 << state)};
}
queue<node> q;
node advancew()
{
    if (q.empty())
    {
        if(EXPS.empty()) return {ERROR, nullptr, 0};
        node tnode = DFA(EXPS.front());
        switch (tnode.t)
        {
        case CHAR:
            if(tnode.e&ASPLIT) return advancew();    // 自动去除界符，不进入语法分析
        case ERROR:
        case INT:
            q.push(tnode);
            break;
        case STRING:
            if (tnode.e & AHIGHTS)
            {
                q.push({STRING,
                        new string(((string *)tnode.p)->substr(0, ((string *)tnode.p)->length() - 3)),
                        ASTRING});
                q.push({STRING,
                        new string(((string *)tnode.p)->substr(((string *)tnode.p)->length() - 3, 3)),
                        AHIGHTS});
            }
            else if (tnode.e & AHIGHTE)
            {
                q.push({STRING,
                        new string(((string *)tnode.p)->substr(0, ((string *)tnode.p)->length() - 4)),
                        ASTRING});
                q.push({STRING,
                        new string(((string *)tnode.p)->substr(((string *)tnode.p)->length() - 4, 4)),
                        AHIGHTE});
            }
            else
            {
                q.push(tnode);
            }
            break;
        }
        EXPS.pop();
    }
    node tnode = q.front();
    q.pop();
    return tnode;
}
class subttime{
    public:
    int hour, minute, second, msecond;
};
class content{
    public:
    string text;
    bool ishightlight;
};
class subt{
    public:
    int sid;
    subttime begin;
    subttime end;
    list<content> cons;
};
list<subt> subtlist;
void pushexp(unsigned int exp){
    if(!q.empty()) return;    // 如果取词器中还有词，应该直接从取词器中取，而不是去指导词法分析器去分析新的词
    EXPS.push(exp);
}
// RDP
node word;
void Procedure_S();
subt Procedure_R();
pair<subttime,subttime> Procedure_B();
subttime Procedure_M();
list<content> Procedure_C();
list<content> Procedure_D();
void Procedure_S(){
    // S->RS|epsilon
    // FIRST(S)={i,epsilon}, FOLLOW(S)={#}
    if(word.e&AINT){
        subt tsubt = Procedure_R();
        subtlist.push_back(tsubt);
        Procedure_S();
    }else{
        if(!(word.t==CHAR&&*(char*)word.p=='#')){
            cout << "error occur in procedure S." << endl;
        }
    }
}
subt Procedure_R(){
    // R->iBC
    // FIRST(R)={i}, FOLLOW(R)={i,#}
    int sid;
    pair<subttime,subttime> ptt;
    subttime begin, end;
    list<content> cons;
    if(word.e&AINT){
        sid = *(int*)word.p;
        pushexp(AW2F0N|ASPLIT);
        word = advancew();
        ptt = Procedure_B();
        begin = ptt.first;
        end = ptt.second;
        cons = Procedure_C();
        return {sid, begin, end, cons};
    }
}
pair<subttime,subttime> Procedure_B(){
    // B->MpM
    // FIRST(B)={f}, FOLLOW(B)={t,b}
    subttime begin, end;
    if(word.e&AW2F0N){
        begin = Procedure_M();
        if(word.e&ASPAN){
            pushexp(AW2F0N|ASPLIT);
            word=advancew();
            end = Procedure_M();
            return {begin, end};
        }
    }
}
subttime Procedure_M(){
    // M->f:f:f,e
    // FIRST(M)={f}, FOLLOW(M)={p,t,b}
    subttime t;
    if(word.e&AW2F0N){
        t.hour = *(int*)word.p;
        pushexp(ACOLON|ASPLIT);
        word = advancew();
        if(word.e&ACOLON){
            pushexp(AW2F0N|ASPLIT);
            word = advancew();
            if(word.e&AW2F0N){
                t.minute = *(int*)word.p;
                pushexp(ACOLON|ASPLIT);
                word = advancew();
                if(word.e&ACOLON){
                    pushexp(AW2F0N|ASPLIT);
                    word = advancew();
                    if(word.e&AW2F0N){
                        t.second = *(int*)word.p;
                        pushexp(ACOMMA|ASPLIT);
                        word = advancew();
                        if(word.e&ACOMMA){
                            pushexp(AW3F0N|ASPLIT);
                            word = advancew();
                            if(word.e&AW3F0N){
                                t.msecond = *(int*)word.p;
                                pushexp(ASPAN|ASTRING|AHIGHTS|ASPLIT);
                                word = advancew();
                                return t;
                            }
                        }
                    }
                }
            }
        }
    }
}
list<content> Procedure_C(){
    // C->tD|btdD
    // FIRST(S)={t,b}, FOLLOW(S)={i,#}
    list<content> tcl;
    if(word.e&ASTRING){
        tcl.push_back({*(string*)word.p, false});
        pushexp(AINT|ASTRING|AHIGHTS|AHIGHTE|ASPLIT);
        word = advancew();
        list<content> Dtcl = Procedure_D();
        for(auto c : Dtcl){
            tcl.push_back(c);
        }
        return tcl;
    }else if(word.e&AHIGHTS){
        pushexp(ASTRING|AHIGHTE|ASPLIT);
        word = advancew();
        if(word.e&ASTRING){
            tcl.push_back({*(string*)word.p, true});
            pushexp(AHIGHTE|ASPLIT);
            word = advancew();
            if(word.e&AHIGHTE){
                pushexp(ASTRING|AHIGHTS|AHIGHTE|AINT|ASPLIT);
                word = advancew();
                list<content> Dtcl = Procedure_D();
                for(auto c : Dtcl){
                    tcl.push_back(c);
                }
                return tcl;
            }
        }
    }
}
list<content> Procedure_D(){
    // D->CD|epsilon
    // FIRST(D)={t,b,epsilon}, FOLLOW(D)={i,#}
    list<content> tcl;
    if(word.e&(ASTRING|AHIGHTS)){
        list<content> Ctcl = Procedure_C();
        list<content> Dtcl = Procedure_D();
        for(auto c : Ctcl){
            tcl.push_back(c);
        }
        for(auto c : Dtcl){
            tcl.push_back(c);
        }
        return tcl;
    }else{
        if(!((word.t==CHAR&&*(char*)word.p=='#')||(word.e&AINT))){
            cout << "error occur in procedure D." << endl;
        }
        return tcl;
    }
}
void printSubt(subt tsubt){
    cout << "sid: " << tsubt.sid << endl;
    cout << "begin: " << tsubt.begin.hour << ":" << tsubt.begin.minute << ":" << tsubt.begin.second << "," << tsubt.begin.msecond << endl;
    cout << "end: " << tsubt.end.hour << ":" << tsubt.end.minute << ":" << tsubt.end.second << "," << tsubt.end.msecond << endl;
    cout << "text: " << endl;
    for(auto text : tsubt.cons){
        cout << text.ishightlight << "   ";
        cout << text.text << endl;
    }
    cout << endl;
}
int main()
{
    initNxts();
    // printNxts();    // ./CaptionParser > Nxtstable.txt
    if (!open()) return 1;
    if (advancec() == EOF) return 2;    // 激活字幕字符读取器
    pushexp(AINT|ASPLIT);    // 进入S前
    word = advancew();    // 激活字幕词读取器
    Procedure_S();
    for(auto s : subtlist){
        printSubt(s);
    }
    return 0;
}