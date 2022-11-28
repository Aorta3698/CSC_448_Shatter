#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>

// global variables
static int LIMIT = int(1e7);
std::unordered_map<std::string, std::pair<std::string, int>> enzymes;

// print usage
void usage(){
    std::cout << "USAGE: digestFragment <genome-file> <enzyme> <output-file>\n"
              << "\n"
              << "<genome-file> must follow the following format (multiple separated headers are OK):\n"
              << "  >HEADER\n"
              << "  Sequence Starts Here\n"
              << "\n"
              << "Currently supported enzymes:\n";

    std::vector<std::string> all_enzyme;
    for (const auto& [enzyme, _] : enzymes){
        all_enzyme.push_back(enzyme);
    }
    std::ranges::sort(all_enzyme);
    for (const auto& enzyme : all_enzyme){
        std::cout << "- " << enzyme << '\n';
    }

    std::cout << "\n"
              << "To save the output as a csv file, enter something like output.csv for <output-file>\n\n";
}

/**
 * compute the prefix function for string s
 * input : the string whose prefix function is to be calculated
 * output: the prefix function for string s
 */
std::vector<int> computePrefix(std::string_view&& s){
    int n = int(s.size());
    std::vector<int> ans(n);
    for (int i = 1; i < n; ++i){
        int j = ans[i-1];
        while(j && s[j] != s[i]){
            j = ans[j-1];
        }
        ans[i] = j + (s[j] == s[i]);
    }
    return ans;
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
    // supported enzyme list
    enzymes["EcoRI"]  = {"GAATTC",   1};
    enzymes["BamHI"]  = {"GGATCC",   1};
    enzymes["HindII"] = {"AAGCTT",   1};
    enzymes["TaqI"]   = {"TCGA",     1};
    enzymes["NotI"]   = {"GCGGCCGC", 2};
    enzymes["HinFI"]  = {"GANTC",    1};
    enzymes["Sau3AI"] = {"GATC",     0};
    enzymes["PvuII"]  = {"CAGCTG",   3};
    enzymes["SamI"]   = {"CCCGGG",   3};
    enzymes["HaeIII"] = {"GGCC",     2};
    enzymes["AluI"]   = {"AGCT",     2};
    enzymes["EcoRV"]  = {"GATATC",   3};
    enzymes["KpnI"]   = {"GGTACC",   5};
    enzymes["PstI"]   = {"CTGCAG",   5};
    enzymes["SacI"]   = {"GAGCTC",   5};
    enzymes["SalI"]   = {"GTCGAC",   1};
    enzymes["ScaI"]   = {"AGTACT",   3};
    enzymes["SpeI"]   = {"ACTAGT",   1};
    enzymes["SphI"]   = {"GCATGC",   5};
    enzymes["StuI"]   = {"AGGCCT",   3};
    enzymes["XbaI"]   = {"TCTAGA",   1};

    // handle command line input
    if (argc != 4){
        usage();
        return -1;
    }
    std::string input_file = argv[1];
    std::string enzyme = argv[2];
    std::string output_file = argv[3];

    // verify enzyme validity
    if (enzymes.count(enzyme) == 0){
        usage();
        return -1;
    }

    // variables needed
    std::ofstream outfile{output_file};
    std::ifstream infile{input_file};
    if (!infile){
        std::cerr << "Failed to open input file " << input_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }
    if (!outfile){
        std::cerr << "Failed to create output file (file already eixsts?)" << output_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }
    std::string line;
    std::string now;
    auto [pat, cut] = enzymes[enzyme];
    int pat_len = int(pat.size());
    int index = 0;

    // process input file
    // there can be more than 1e11 characters,
    // we must process it chuck by chuck
    // I use KMP algo which runs in O(P+T) and it is deterministic
    while(infile){
        std::getline(infile, line);
        if (line[0] == '>' || int(now.size()) >= LIMIT || !infile){
            std::string dummy = pat + "&" + now;
            int len = int(dummy.size());
            int prev = 0;
            auto pi = computePrefix(dummy);
            for (int i = pat_len + 1; i < len; ++i){
                if (pi[i] == pat_len){
                    outfile << now.substr(prev, i - 2*pat_len+cut-prev) << '\n' << ++index << ",";
                    prev = i-2*pat_len+cut;
                    i += pat_len - 1;
                }
            }
            outfile << now.substr(prev);
            if (line[0] == '>'){ // header, a new segment, reset index
                index = 0;
                outfile << (now.empty()? "" : "\n") << ++index << ",";
            }
            now = "";
        }
        if (infile && line[0] != '>'){
            now += str_toupper(line);
        }
    }
    infile.close();
    outfile.close();
};




