#include <iostream>
#include <fstream>

int main() {
    std::ifstream file;
    file.open("test.csv");

    while(file.good()){
        std::string line;
        getline(file, line, ',');
        std::cout << line << std::endl;
    }
    file.close();
    return 0;
}
