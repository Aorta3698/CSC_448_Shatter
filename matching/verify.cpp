#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstring>

constexpr static int maxn = 3e7 + 42; // Maximum amount of states
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

/**
 * convert a string to uppercase
 * input : string s
 * output: string s in uppercase
 */
std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c);});
    return s;
}

int main(int argc, char *argv[]){
    std::string ref_genome_file = "data_up.fna";

    // open all the files needed and verify whether they are successful
    std::ifstream ref{ref_genome_file};
    if (!ref){
        std::cerr << "Failed to open input file " << ref_genome_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }

    // build the suffix automaton
    std::string line, all;
    std::getline(ref, line); // skip header
    int i = 0;
    while(std::getline(ref, line)){
        all += line;
        ++i;
        if (long long s = all.find("AAAAAAGAAATGCGCAAGCCCGGAATCGAACCGGGGGCCCAACGATGGCAACGTTGGATTTTACCACTAAACCACTTGCGC"); s != std::string::npos){
            std::cout << "FOUND! at line " << i << ", pos = " << s;
        }
        if (all.size() > 1000){
            all = all.substr(500);
        }
    }
    /* if (long long s = all.find("AAAAAAGAAATGCGCAAGCCCGGAATCGAACCGGGGGCCCAACGATGGCAACGTTGGATTTTACCACTAAACCACTTGCGC"); s != std::string::npos){ */
    /*     std::cout << "FOUND! at " << s; */
    /* }else{ */
        std::cout << "NOT FOUND!";
    /* } */
    std::cout << '\n';
    ref.close();
};




