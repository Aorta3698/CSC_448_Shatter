#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>

constexpr static int maxn = 5e5+2; // Maximum amount of states
int to[maxn][26];          // Transitions
int link[maxn];            // Suffix links
int len[maxn];             // Lengthes of largest strings in states
int last = 0;              // State corresponding to the whole string
int sz = 1;                // Current amount of states

void addLetter (int c){   // Adding character to the end
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
    memcpy(to[cl], to[q], sizeof(to[cl]));
    link[cl] = link[q];
    len[cl] = len[p] + 1;
    link[last] = link[q] = cl;
    for (; to[p][c] == q; p = link[p]){
        to[p][c] = cl; // Redirect transitions where needed
    }
}

int main(){
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    // build the suffix automaton
    std::string a, b;
    std::cin >> a >> b;
    for (char ch : a){
        addLetter(ch-'a');
    }

    int cur = 0, l = 0, best = 0;
    for (int i : b){
        int k = i - 'a';
        while(cur && to[cur][k] == 0){
            cur = link[cur];
            l = len[cur];
        }
        if (to[cur][k]){
            cur = to[cur][k];
            best = std::max(best, ++l);
        }
    }
    std::cout << best << '\n';
};




