#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <set>

using namespace std;

vector<string> tokenizeAOL() {
    // Store the contents into a vector of strings
    vector<string> outputs;

    // Create the file object (input)
    ifstream myfile(R"(C:\Users\ruben\Desktop\Uni\6. Sjette semester\BachelorProjekt\Coding\dataset2.txt)");

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

    // Create the file object (input)
    ifstream myfile(R"(C:\Users\ruben\Desktop\Uni\6. Sjette semester\BachelorProjekt\Coding\dataset3.txt)");

    // Temporary buffer
    string temp;

    // Get the input from the input file until EOF
    while (getline(myfile, temp)) {
        // regex expression for pattern to be searched
        regex rgx(R"(([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+))");

        smatch match;

        // regex_search that searches pattern regexp in the string mystr
        if(regex_search(temp, match, rgx)){
            for (auto elem : match) {
                outputs.push_back(elem);
            }
        }

    }

    // Removing duplicates, since I can't seem to figure out, when it gets duplicated
    sort( outputs.begin(), outputs.end() );
    outputs.erase( unique( outputs.begin(), outputs.end() ), outputs.end() );

    return outputs;
}

void HashMap(const vector<string>& values) {

    map<string, int> m;

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
    //for (auto const &pair: m) {
        //std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    //}
}

int main() {
    vector<string> AOL = tokenizeAOL();
    vector<string> CAIDA = tokenizeCAIDA();
    HashMap(AOL);

    return 0;
}
