#include "ArgumentManager.h"
#include "BTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    std::ifstream ifs(am.get("input"));
    std::ifstream cfs(am.get("command"));
    std::ofstream ofs(am.get("output"));

    std::string input;
    std::stringstream ss;
    int degree;
    std::getline(cfs, input);
    input = input.substr(input.find('=') + 1, input.size() - input.find('=') - 1);
    degree = std::stoi(input);
    BTree bt(degree);
    int key;
    while (std::getline(ifs, input)) {
        if (input.size() == 0) continue;
        ss.clear();
        ss << input;
        while (ss >> key) {
            if (bt.search(key) == nullptr)
                bt.insert(key);
        }
    }

    int height = bt.height();
    ofs << "Height=" << height << "\n";
    int level;
    std::string str;
    while (std::getline(cfs, input)) {
        if (input.size() == 0) continue;
        ss.clear();
        ss << input;
        ss >> str >> level;
        if (level > height) {
            ofs << "Empty\n";
            continue;
        }
        bt.traverse(level, ofs);
        ofs << "\n";
    }

    ofs.flush();
    ofs.close();
    cfs.close();
    ifs.close();
    return 0;
}