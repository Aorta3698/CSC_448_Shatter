#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

// print usage
void usage(){
    std::cout << "USAGE: toupper <input_genome> <output_genome>\n"
              << "\n"
              << "all 2 files are required.\n"
              << "\n";
}

std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c);});
    return s;
}

int main(int argc, char *argv[]){
    // handle command line input
    if (argc != 3){
        usage();
        return -1;
    }
    std::string input_file = argv[1];
    std::string output_file= argv[2];

    // open all the files needed and verify whether they are successful
    std::ofstream outfile{output_file};
    std::ifstream infile{input_file};
    if (!infile){
        std::cerr << "Failed to open input file " << input_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }
    if (!outfile){
        std::cerr << "Failed to create output file " << output_file << '\n';
        std::cerr << "Exiting..." << '\n';
        return -1;
    }

    std::string line;
    while(std::getline(infile, line)){
        outfile << str_toupper(line) << '\n';
    }

    infile.close();
    outfile.close();
};




