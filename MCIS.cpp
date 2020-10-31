#include<iostream>
#include<set>
#include<algorithm>
#include<map>
using namespace std;
set<pair<int,int>> maximumcomsub={};
map<int,set<int>> NG;//map with key=vertex in G and key = adj to key
map<int,set<int>> NH;
set<int> VG;
set<int> VH;


bool checkMCIS(set<pair<int,int>> M,int k){
if(M.size()<k)return false;
map<int,int> Map;
for(auto x:M) {Map[x.first]=x.second;Map[x.second]=x.first;}
for(auto x:M){
    int v=x.first;
    int w=x.second;
    for(auto y:NG[v]){
        if(Map.find(y)!=Map.end()){
            if((NH[w]).find(Map[y])==NH[w].end()){
                    return false;
            }
        }
    }
    for(auto y:NH[w]){
        if(Map.find(y)!=Map.end()){
            if((NG[v]).find(Map[y])==NG[w].end()){
                    return false;
            }
        }
    }
}
return true;

}

int sigma(set<pair<set<int>,set<int>>> future){//ยังไม่ได้เช็ค
    int c=0;
    for(auto x:future){
        if(x.first.size()>=x.second.size()){
            c+=x.second.size();
        }else{
            c+=x.first.size();
        }
    }
    return c;
}
int selectVertex(set<int> S){
    int m=0;int k;
    for(auto x:S){
        if(NG[x].size() > m){
            m=NG[x].size();
            k=x;
        }
    }
    return k;
}
pair<set<int>,set<int>>  selectlabelclass( set<pair<set<int>,set<int>>> future){

    int min=9999999999;
    pair<set<int>,set<int>> R;
    for(auto x :future){
        int a =x.first.size();
        int b =x.second.size();
        int m=max(a,b);
        if(m<min){
            R=x;
            min=m;
        }
    }
    return R;
}

set<int> adjacent(int v , char type){
    if(type =='H'){
        return NH[v];
    }
    if(type == 'G'){
        return NG[v];
    }
    if(type == 'h'){
        set<int> S1;
        for(auto x:VH){
            if(NH[v].find(x)== NH[v].end() && x!=v){
                S1.insert(x);
            }
        }
        return S1;
    }
    if(type == 'g'){
        set<int> S1;
        for(auto x:VG){
            if(NG[v].find(x)== NG[v].end() && x!=v){
                S1.insert(x);
            }
        }
        return S1;
    }
}

set<int> intersection(set<int> s1, set<int> s2){
    set<int> ret;
    for(auto x:s1)for(auto y :s2)if(x==y)ret.insert(x);
    return ret;
}
void MCIS(set<pair<set<int>,set<int>>> future, set<pair<int,int>> M){
    if(M.size()>maximumcomsub.size()) maximumcomsub=M;
    int bound =M.size()+sigma(future);
    if(bound<= maximumcomsub.size())return;
    pair<set<int>,set<int>> samelabel= selectlabelclass(future);
    int v = selectVertex(samelabel.first);
    for(auto w:samelabel.second){
        set<pair<set<int>,set<int>>> future_1;
        set<int> G_1;//contain vertex in G with label L
        set<int> H_1;//contain vertex in H with label L
        set<int> G_2;//contain adjacent v
        set<int> G_3;//contain not adjacent v
        set<int> H_2;//contain adjacent w
        set<int> H_3;//contain not adjacent w
        for(auto x : future){
            G_1 =x.first;
            H_1 =x.second;
            G_2= intersection(G_1,adjacent(v,'G'));
            H_2= intersection(H_1,adjacent(w,'H'));
            G_3= intersection(G_1,adjacent(v,'g'));
            H_3= intersection(H_1,adjacent(w,'h'));
            if(G_2.size()>0 && H_2.size()>0){
                future_1.insert({G_2,H_2});
            }
            if(G_3.size()>0 && H_3.size()>0){
                future_1.insert({G_3,H_3});
            }
        }
            set<pair<int,int>> M_tmp=M;
            M_tmp.insert({v,w});
            cout<<v<<","<<w<<" ";
            MCIS(future_1,M_tmp);
    }

    set<int> g1={};
    for(auto x:samelabel.first){
        if(x!=v)g1.insert(x);
    }
   set<pair<set<int>,set<int>>>  futureUnmatch={};
    for(auto x:future){
        if(x!=samelabel)futureUnmatch.insert(x);
    }
    if(g1.size()!=0){
    futureUnmatch.insert({g1,samelabel.second});
    MCIS(futureUnmatch,M);
    }

}
int main(){
int k,nG,nH;cin>> k >> nG >> nH;

set<pair<int,int>> M={};
for(int i=0;i<nG;i++){
    int v; cin>> v;
    VG.insert(v);
}
for(int i=0;i<nH;i++){
    int v; cin>> v;
    VH.insert(v);
}
int edgeG,edgeH; cin >>edgeG>> edgeH;

for(int i=0;i<edgeG;i++){//edge in graph G
    int a,b;cin >> a >> b;
    NG[a].insert(b);
    NG[b].insert(a);
}
for(int i=0;i<edgeH;i++){//edge in graph H
    int a,b;cin >> a >> b;
    NH[b].insert(a);
    NH[a].insert(b);
}

MCIS({{VG,VH}},M);


if(maximumcomsub.size()>=k)cout<<"YES\n";
else cout<<"NO\n";
for(auto x:maximumcomsub)cout<<x.first<<" :" <<x.second<<"\n";

if(checkMCIS(maximumcomsub,k))cout<<"check pass\n";
else cout<<"check failed\n";
}
