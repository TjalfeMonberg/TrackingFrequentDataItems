//==================================================== file = genzipf.c =====
//=  Program to generate Zipf (power law) distributed random variables      =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of values                    =
//=         3) Run times is same as an empirical distribution generator     =
//=         4) Implements p(i) = C/i^alpha for i = 1 to N where C is the    =
//=            normalization constant (i.e., sum of p(i) = 1).              =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ---------------------------------------- genzipf.c -----              =
//=   -     Program to generate Zipf random variables        -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Alpha vlaue ========================================> 1.0             =
//=   N value ============================================> 1000            =
//=   Number of values to generate =======================> 5               =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   1                                                                     =
//=   1                                                                     =
//=   161                                                                   =
//=   17                                                                    =
//=   30                                                                    =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genzipf.c                                                 =
//=-------------------------------------------------------------------------=
//=  Execute: genzipf                                                       =
//=-------------------------------------------------------------------------=
//=  Author: Kenneth J. Christensen                                         =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (11/16/03) - Genesis (from genexp.c)                      =
//===========================================================================
// Rewritten version of the original genzipf.c file to c++
// By Ruben Mortensen

//----- Include files -------------------------------------------------------
#include <iostream>            // For getting terminal info
#include <fstream>             // For writing the outputfile
#include <cassert>             // Needed for assert() macro
#include <cstdio>              // Needed for printf()
#include <cstdlib>             // Needed for exit() and ato*()
#include <cmath>               // Needed for pow()

//----- Namespace -----------------------------------------------------------
using namespace std;

//----- Constants -----------------------------------------------------------
#define  FALSE          0       // Boolean false
#define  TRUE           1       // Boolean true

//----- Function prototypes -------------------------------------------------
int      zipf(double alpha, int n);  // Returns a Zipf random variable
double   rand_val(int seed);         // Jain's RNG

//===== Main program ========================================================
int main() {
    ofstream fp;                   // File pointer to output file
    char   file_name[256];          // Output file name string
    char   temp_string[256];        // Temporary string variable
    double alpha, n;                // Alpha parameter and N parameter
    int    num_values, zipf_rv;  // Number of values, Zipf random variable

    // Output banner
    printf("---------------------------------------- genzipf.cpp --- \n");
    printf("-     Program to generate Zipf random variables        - \n");
    printf("-------------------------------------------------------- \n");

    // Prompt for output filename and then create/open the file
    cout << "Output file name ===================================> ";
    cin >> file_name;
    fp.open(file_name, ios::out);
    if (!fp.is_open()) {
        cout << "ERROR in creating output file (" << file_name << ") \n";
        exit(1);
    }

    // Prompt for random number seed and then use it
    cout << "Random number seed (greater than 0) ================> ";
    cin >> temp_string;
    rand_val(atoi(temp_string));

    // Prompt for alpha value
    cout << "Alpha value ========================================> ";
    cin >> temp_string;
    alpha = atof(temp_string);

    // Prompt for N value
    cout << "N value ============================================> ";
    cin >> temp_string;
    n = atoi(temp_string);

    // Prompt for number of values to generate
    cout << "Number of values to generate =======================> ";
    cin >> temp_string;
    num_values = atoi(temp_string);

    // Output "generating" message
    cout << "-------------------------------------------------------- \n";
    cout << "-  Generating samples to file                          - \n";
    cout << "-------------------------------------------------------- \n";

    // Generate and output zipf random variables
    for (int i = 0; i < num_values; i++)
    {
        zipf_rv = zipf(alpha, n);
        fp << zipf_rv << "\n";
    }

    // Output "done" message and close the output file
    cout << "-------------------------------------------------------- \n";
    cout << "-  Done! \n";
    cout << "-------------------------------------------------------- \n";
    fp.close();

    return 0; // Keep the compiler happy
}

//===========================================================================
//=  Function to generate Zipf (power law) distributed random variables     =
//=    - Input: alpha and N                                                 =
//=    - Output: Returns with Zipf distributed random variable              =
//===========================================================================
int zipf(double alpha, int n)
{
    static int first = TRUE;      // Static first time flag
    static double c = 0;          // Normalization constant
    double z;                     // Uniform random number (0 < z < 1)
    double sum_prob;              // Sum of probabilities
    double zipf_value;            // Computed exponential value to be returned

    // Compute normalization constant on first call only
    if (first == TRUE) {
        for (int i = 1; i <= n; ++i)
            c = c + (1.0 / pow((double) i, alpha));
        c = 1.0 / c;
        first = FALSE;
    }

    // Pull a uniform random number (0 < z < 1)
    do {
        z = rand_val(0);
    } while ((z == 0) || (z == 1));

    // Map z to the value
    sum_prob = 0;
    zipf_value = 0;
    for (int i=1; i <=n ; ++i) {
        sum_prob = sum_prob + c / pow((double) i, alpha);
        if (sum_prob >= z) {
            zipf_value = i;
            break;
        }
    }

    // Assert that zipf_value is between 1 and N
    assert((zipf_value >= 1) && (zipf_value <= n));

    return ((int) zipf_value);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(int seed)
{
    const long  a =      16807;  // Multiplier
    const long  m = 2147483647;  // Modulus
    const long  q =     127773;  // m div a
    const long  r =       2836;  // m mod a
    static long x;               // Random int value

    // Set the seed if argument is non-zero and then return zero
    if (seed > 0) {
        x = seed;
        return(0.0);
    }

    // RNG using integer arithmetic
    long x_div_q = x / q;
    long x_mod_q = x % q;
    long x_new = (a * x_mod_q) - (r * x_div_q);
    if (x_new > 0) {
        x = x_new;
    } else {
        x = x_new + m;
    }

    // Return a random value between 0.0 and 1.0
    return((double) x / m);
}