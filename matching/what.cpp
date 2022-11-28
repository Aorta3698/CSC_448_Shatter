#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;
mt19937 mt((unsigned int)chrono::steady_clock::now().time_since_epoch().count());

int random(int a, int b){
    return uniform_int_distribution<int>(a,b)(mt);
}

constexpr static int maxn = 2e5+10; // Maximum amount of states
std::map<char, int> to[maxn];   // Transitions
int link[maxn];            // Suffix links
int len[maxn];             // Lengthes of largest strings in states
int last = 0;              // State corresponding to the whole string
int sz = 1;                // Current amount of states

void addLetter (char c){   // Adding character to the end
    int p = last;          // State of string s
    last = sz++;           // Create state for string sc
    len[last] = len[p] + 1;
    for (; to[p][c] == 0; p = link[p]){
        to[p][c] = last;  // Jumps which add new suffixes
    }
    if(to[p][c] == last){ // This is the first occurrence of c
        link[last] = 0;
        return;
    }
    int q = to[p][c];
    if(len[q] == len[p] + 1){
        link[last] = q;
        return;
    }
    // We split off cl from q here
    int cl = sz++;
    to[cl] = to[q];
    link[cl] = link[q];
    len[cl] = len[p] + 1;
    link[last] = link[q] = cl;
    for (; to[p][c] == q; p = link[p]){
        to[p][c] = cl; // Redirect transitions where needed
    }
}

void MAIN(string a, string b){

    // build the suffix automaton
    for (char ch : a){
        addLetter(ch);
    }

    int cur = 0, l = 0, end = 0, best = 0;
    for (int i = 0; i < int(b.size()); ++i){
        while(cur && to[cur].count(b[i]) == 0){
            cur = link[cur];
            l = len[cur];
        }
        if (to[cur].count(b[i])){
            cur = to[cur][b[i]];
            if (++l > best){
                end = i;
                best = l;
            }
        }
    }
    string ans = b.substr(end - best + 1, best);
    if (a.find(ans) == string::npos || b.find(ans) == string::npos){
        cout << "ans = " << ans << '\n';
        cout << "a = " << a << '\n';
        cout << "b = " << b << '\n';
        exit(0);
    }

};

int main(){
    for (int t = 0; t < 1000; ++t){
        for (int i = 0; i < maxn; ++i){
            to[i].clear();
            link[i] = len[i] = 0;
        }
        last = 0;
        sz = 1;
        string all = "abcd";
        int a = random(1, 100000);
        int b = random(1, 100000);
        int n = int(all.size());
        string A, B;
        while(a--){
            A.push_back(all[random(0, n-1)]);
        }
        while(b--){
            B.push_back(all[random(0, n-1)]);
        }
        MAIN(A, B);
    }
    return 0;
}




