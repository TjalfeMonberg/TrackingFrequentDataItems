#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <random>

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

void misraGries(int k, vector<int>& incomming, int n) {
    // Initialize the new map
    unordered_map<int, int> freqMap;

    // Iterate the incomming stream
    for (int i : incomming) {
        // If the element already exists in the map, add one to its frequency
        if(freqMap.find(i) != freqMap.end()) {
            freqMap[i]++;
        }
        // Else if the size of D is less than k-1, insert a in D with value 1
        else if (freqMap.size() < k - 1) {
            freqMap[i] = 1;
        }
        // Else decrement all counters in D by 1, and remove all elements with count 0
        else {
            for (auto& it : freqMap) {
                it.second--;
                if (it.second == 0) {
                    freqMap.erase(it.first);
                }
            }
        }
    }
}

bool isPrime(int n) {
    // Since 0 and 1 is not
    // prime return false.
    if(n == 1 || n == 0) return false;

    // Run a loop from 2 to n-1
    for(int i = 2; i < n; i++)
    {
        // if the number is divisible by i,
        // then n is not a prime number.
        if(n  % i == 0) return false;
    }
    // Otherwise n is a prime number.
    return true;
}

/*
// Utility function to do modular exponentiation.
// It returns (x^y) % p
int power(int x, unsigned int y, int p)
{
    int res = 1;      // Initialize result
    x = x % p;  // Update x if it is more than or
    // equal to p
    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res*x) % p;

        // y must be even now
        y = y>>1; // y = y/2
        x = (x*x) % p;
    }
    return res;
}

bool millerTest(int d, int prime) {
    // Pick a random number in [2..n-2]
    // Corner cases make sure that n > 4
    default_random_engine generator;
    uniform_int_distribution<int> distribution(2,prime-4);
    int a = distribution(generator);

    // Compute a^d % n
    int x = power(a, d, prime);

    if (x == 1  || x == prime-1)
        return true;

    // Keep squaring x while one of the following doesn't
    // happen
    // (i)   d does not reach n-1
    // (ii)  (x^2) % n is not 1
    // (iii) (x^2) % n is not n-1
    while (d != prime-1)
    {
        x = (x * x) % prime;
        d *= 2;

        if (x == 1)      return false;
        if (x == prime-1)    return true;
    }

    // Return composite
    return false;
}

int generateRandomPrine(int lowerBound, int upperBound) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(lowerBound,upperBound);

    // We start by generating the random number, between the bounds
    int temp = distribution(generator);

    if (isPrime(temp)) {

        // Find r, such that n = 2^d * r + 1 for some r >= 1
        int d = temp - 1;
        while (d % 2 == 0) {
            d /= 2;
        }


        for (int i = 0; i < 100; ++i) {
            if (!millerTest(d, temp)) {
                return false;
            }
        }
        return temp;
    }
    return 0;
}
*/

vector<int> generateRandomPrime(int lowerBound, int upperBound ) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(lowerBound,upperBound);

    int primeCandidate = distribution(generator);
    vector<int> candidates;

    for (int i = 0; i < upperBound; ++i) {
        if (isPrime(primeCandidate)) {
            candidates.push_back(primeCandidate);
        }
        primeCandidate++;
    }

    return candidates;
}

/*
int customHashFunction(int lowerBound, int upperBound, vector<int> dataStream) {
    // Getting a random prime number
    int prime = generateRandomPrime(lowerBound, upperBound);

    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,prime);
    uniform_int_distribution<int> distribution2(0,prime^2);

    // Selecting random a and b
    int a = distribution2(generator);
    int b = distribution2(generator);
    int m = distribution(generator);

    vector<int> hashedVector;

    for (auto elem : dataStream) {
        int hashFunction = ((a * elem + b) % prime) % m;
    }
}
*/

int main() {
    //vector<string> AOL = tokenizeAOL();

    // We do not need to use this method call anymore, since we have generated our new data.
    //vector<string> CAIDA = tokenizeCAIDA();

    //HashMap(AOL);
    //HashMap(CAIDA);

    vector<int> primes = generateRandomPrime(100,100000);
    for (auto elem : primes) {
        cout << elem << "\n";
    }
    cout << "\n";
    cout << primes.size() << "\n";

    return 0;
}
