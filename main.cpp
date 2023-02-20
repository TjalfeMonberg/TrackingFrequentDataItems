#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>

using namespace std;

vector<string> tokenizeAOL() {
    // Store the contents into a vector of strings
    vector<string> outputs;

    // Create the file object (input)
    ifstream myfile(R"(dataset/AOLDataset.txt)");

    // Temporary buffer
    string temp;
    string token;
    stringstream iss;

    // Get the input from the input file until EOF
    while (getline(myfile, temp)) {

        iss << temp;
        while (getline (iss, token, ' ')) {
            // Add to the list of output strings
            outputs.push_back(token);
        }
        iss.clear();
    }

    // Use a range-based for loop to iterate through the output vector
    //for (const auto& i : outputs){
        //cout << i;
    //}

    return outputs;
}

vector<string> tokenizeCAIDA() {
    // Store the contents into a vector of strings
    vector<string> outputs;
    vector<uint32_t> test;

    // Create the file object (input)
    ifstream myfile(R"(dataset/CaidaDataset.txt)");
    ofstream CaidaProcessed(R"(dataset/CAIDAProcessed.txt)");

    // Temporary buffer
    string temp;

    // Get the input from the input file until EOF
    while (getline(myfile, temp)) {
        // regex expression for pattern to be searched
        regex rgx(R"(\b(?:[0-9]{1,3}\.){3}[0-9]{1,3}\b)");

        smatch match;

        // regex_search that searches pattern regexp in the string mystr
        if(regex_search(temp, match, rgx)){
            for (auto elem : match) {
                uint32_t IntegerAddress = 0;
                string token;
                string tempStr = elem.str();
                std::istringstream iss(tempStr);
                for (int i=0; i<4; ++i) {
                    uint32_t part;
                    iss >> part;
                    IntegerAddress |= part << (8*(3-i));
                    if ( i != 3 ) {
                        char delimiter;
                        iss >> delimiter;
                        if (iss.fail() || delimiter != '.') {
                            throw std::runtime_error("Invalid IP address - Expected '.' delimiter");
                        }
                    } else {
                        CaidaProcessed << IntegerAddress << endl;
                    }
                }
            }
        }

    }
    // Printing the elements of the vector (Commeted out most of the time)
    //for (const auto& x : outputs) {
        //cout << x << "\n";
    //}
    CaidaProcessed.close();
    cout << "done :)";
    return outputs;
}

void HashMap(const vector<string>& values) {

    unordered_map<string, int> m;

    // Inserting the values with a unique int
    for (const auto & value : values) {
        // Checking if the value exists, then adding one
        if(m.find(value) != m.end()) {
            m[value]++;
        } else {
            m[value] = 1;
        }
    }

    // For printing the content of the hashmap
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }

    unsigned currentMax = 0;
    string arg_max = " ";

    for(auto it = m.cbegin(); it != m.cend(); ++it ) {
        if (it ->second > currentMax) {
            arg_max = it -> first;
            currentMax = it -> second;
        }
    }
    cout << "Value " << arg_max << " occurs " << currentMax << " times " << endl;
}

int main() {
    //vector<string> AOL = tokenizeAOL();
    vector<string> CAIDA = tokenizeCAIDA();
    //HashMap(AOL);
    //HashMap(CAIDA);

    return 0;
}
