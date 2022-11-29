#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <chrono>

// print usage
void usage(){
    std::cout << "USAGE: match <genome-file> <fragments-file> <output-file>\n"
              << "It finds the longest common substring for each fragment in <fragments-file> with all the chronosomes in <genome-file>\n"
              << "the input file should not have letters other than 'A', 'T', 'C', 'G', 'N', 'W'.\n"
              << "The maximum length for each chronosome (header) is 25,000,000. It will crash with an error message if it goes over that\n"
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
 * Error out if somehow there is a letter other than "ATCGNW"
 */
void error(char ch){
    std::cout << "got '" << ch << "', which is not supported (ATCGNW)" << '\n';
    std::cout << "Exiting" << '\n';
    exit(-1);
}

/**
 * global variables (for the sake of speed & simplicity)
 */
int idx[128];
std::vector<std::pair<int, std::string>> ans;

/**
 * Suffix Automaton Section
 * This runs in linear time O(N)
 */
constexpr static int maxn = 5e7; // Maximum amount of states
int to[maxn][6];           // Transitions
int link[maxn];            // Suffix links
int len[maxn];             // Lengthes of largest strings in states
int last = 0;              // State corresponding to the whole string
int sz = 1;                // Current amount of states

void addLetter (int c){   // Adding character to the end
    if (sz == maxn){
        std::cout << "Sorry - too many characters in one header leading to its crash.\n";
        exit(-1);
    }
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


/**
 * Use suffix automaton to find the best match for the current chronosome (header)
 * input : fragment file
 * output: the best match for each fragment
 */
void solve(std::ifstream& frag){
    std::string line;
    int count = 0;

    // find the longest match for each fragment
    while(std::getline(frag, line)){ // fragments should be all upper case now
        auto comma = line.find(",");
        int index = std::stoi(line.substr(0, comma));
        line = line.substr(comma + 1);

        int cur = 0, l = 0, end = 0, maxLen = 0;
        for (int i = 0; i < int(line.size()); ++i){
            int a = int(line[i]);
            if (idx[a] == -1){
                std::cout << "[fragment file]\n"
                          << "Line: " << count << " with an index of " << index << " with line\n"
                          << line << '\n';
                error(line[i]);
            }
            int k = idx[a];
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

        // add to answer if better
        const auto& [_, match] = ans[count];
        if (maxLen > int(match.size())){
            ans[count] = {index, line.substr(end - maxLen + 1, maxLen)};
        }
        ++count;
    }

    // rewind
    frag.clear();
    frag.seekg(0);
};

int main(int argc, char* argv[]){
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

    // give each letter its own index. The input files should not have letters other than ATCGNW
    memset(idx, -1, sizeof(idx));
    idx['A'] = 0;
    idx['T'] = 1;
    idx['C'] = 2;
    idx['G'] = 3;
    idx['N'] = 4;
    idx['W'] = 5;

    // count how many fragments are there
    std::string line;
    int count = 0;
    while(std::getline(frag, line)){
        ++count;
    }
    frag.clear();
    frag.seekg(0);
    ans = std::vector<std::pair<int, std::string>>(count);
    auto t1 = std::chrono::high_resolution_clock::now();

    // build the suffix automaton and solve for each header
    std::getline(ref, line); // skip the first header line
    while(std::getline(ref, line)){
        if (line[0] == '>'){ // header
            solve(frag);
            std::cout << "one lap finished... \n";
            for (int i = 0; i < sz; ++i){
                memset(to[i], 0, sizeof(to[i]));
            }
            memset(link, 0, sz * sizeof(int));
            memset(len,  0, sz * sizeof(int));
            last = 0;
            sz = 1;
            continue;
        }
        for (unsigned char ch : str_toupper(line)){
            if (idx[ch] == -1){
                std::cout << "[genome file]\n"
                          << "Line: \n"
                          << line << '\n';
                error(ch);
            }
            addLetter(idx[ch]);
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Done! now outputting the answer to " << output_file << '\n';
    std::cout << "Total time taken = " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << '\n';

    // output the answer
    for (const auto& each : ans){
        const auto& [index, match] = each;
        outfile << index << "," << match << '\n';
    }

    // close all the files
    ref.close();
    frag.close();
    outfile.close();
}




