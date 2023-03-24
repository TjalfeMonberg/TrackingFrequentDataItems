//
// Created by Tjalfe on 14-03-2023.
//

#include "CountMinSketch.cpp"
#include "CountSketch.cpp"
#include "math.h"
#include <stdint.h>
#include "iostream"
#include <chrono>

vector<int> vectorShit() {
    // Loading the dataset for testing
    ifstream dataStream(R"(TrackingFrequentDataItems/dataset/CAIDAProcessed.txt)");

    //Vector for the values in the file
    vector<int> processedValues;

    // Temporary buffer and string stream for converting string to int.
    string temp;
    uint32_t elem;
    stringstream ss;

    // Get the input from the input file until EOF
    while (getline(dataStream, temp)) {
        ss << temp;
        ss >> elem;
        // Add to the list of output strings
        processedValues.push_back(elem);
        ss.clear();
    }

    return processedValues;
}

void testCMS(double e, double d) {
    // Deprecated, use direct values instead and calculate backwards in report with math
    // int k = ceil(2 / e);
    // int t = ceil(log2(1/d));

    int k = e;
    int t = d;
    cout << "T: " << t << "\n";
    cout << "K: " << k << "\n";

    // Creating the CountMinSketch model
    CountMinSketch CMS = *new CountMinSketch(t, k);

    vector<int> processedValues = vectorShit();

    for (auto x : processedValues) {
        CMS.updateCounters(x, 1);
    }

    int res = CMS.findMinElem(3107889171);

    cout << "Res: " << res << endl;
}

// Er det sådan her det skal regnes ud?
// TODO: Spørg Kasper om det her
// TODO: Hvordan vælger vi delta ordenligt?
double l2() {
    vector<int> something = vectorShit();

    double accum = 0;
    for (double x : something) {
        accum += 1 * 1;
    }
    double norm = sqrt(accum);

    return norm;
}

void testCS(double e, double d) {
    int k = ceil(2 / e);
    int t = ceil(log2(1/d));

    cout << "T: " << t << "\n";
    cout << "K: " << k << "\n";

    CountSketch CS = *new CountSketch(t, k);

    vector<int> processedValues = vectorShit();

    for (auto x : processedValues) {
        CS.updateCounters(x, 1);
    }

    int res = CS.findMedianElem(3107889171);

    cout << "Res: " << res << endl;

}

int main(){
    double d = l2();
    cout << "d: " << d << endl;
    auto start = chrono::high_resolution_clock::now();
    // We are choosing epsilon e = 0.05 and delta d = 0.05 for initial testing
    //testCMS(0.05, 0.05);
    d *= 0.05;
    testCS(0.05, d);

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Duration: " << duration.count() << " milliseconds" << endl;
    return 0;
}