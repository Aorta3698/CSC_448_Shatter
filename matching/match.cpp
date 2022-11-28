#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>

// print usage
void usage(){
    std::cout << "USAGE: match <genome-file> <fragments-file> <output-file>\n"
              << "\n"
              << "All 3 files are required.\n"
              << "\n"
              << "<genome-file> must follow the following format (multiple separated headers are OK):\n"
              << "  >HEADER\n"
              << "  Sequence Starts Here\n"
              << "\n"
              << "<fragments-file> must follows the following format for each line: \n"
              << "  NUM,FRAGMENT\n"
              << "\n"
              << "To save the output as a csv file, enter something like output.csv for <output-file>\n\n";
}

/**
 * Suffix Automaton Section
 * This runs in linear time O(N)
 */
constexpr static int maxn = 3e7; // Maximum amount of states
int to[maxn][5];           // Transitions
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
    // handle command line input
    if (argc != 4){
        usage();
        return -1;
    }
    std::string ref_genome_file = argv[1];
    std::string fragments_file  = argv[2];
    std::string output_file     = argv[3];

    // open all the files needed and verify whether they are successful
    std::ofstream outfile{output_file};
    std::ifstream ref{ref_genome_file};
    std::ifstream frag{fragments_file};
    if (!ref){
        std::cerr << "Failed to open input file " << ref_genome_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }
    if (!frag){
        std::cerr << "Failed to open input file " << fragments_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }
    if (!outfile){
        std::cerr << "Failed to create output file (maybe it already exists) " << output_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }

    // build the suffix automaton
    int idx[128];
    idx['A'] = 0;
    idx['T'] = 1;
    idx['C'] = 2;
    idx['G'] = 3;
    idx['>'] = 4;
    std::string line;
    while(std::getline(ref, line)){
        if (line[0] == '>'){ // header
            line = ">";
        }
        for (unsigned char ch : str_toupper(line)){
            addLetter(idx[ch]);
        }
    }
    ref.close();

    // find the longest match for each fragment
    while(std::getline(frag, line)){ // fragments should be all upper case now
        auto comma = line.find(",");
        int index = std::stoi(line.substr(0, comma));
        line = line.substr(comma + 1);

        int cur = 0, l = 0, end = 0, maxLen = 0;
        for (int i = 0; i < int(line.size()); ++i){
            int k = idx[int(line[i])];
            while(cur && to[cur][k] == 0){
                cur = link[cur];
                l = len[cur];
            }
            if (to[cur][k]){
                cur = to[cur][k];
                if (++l > maxLen){
                    end = i;
                    maxLen = l;
                }
            }
        }

        // print out the longest common substring
        outfile << index << "," << line.substr(end-maxLen+1, maxLen) << '\n';
    }

    frag.close();
    outfile.close();
};




