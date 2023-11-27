// ---------------------------------------------------------------------
// UnlimitedInt
// ------------
// This is a pretty standard bignum arithmetic implementation.
// numbers are stored as an array of ints, but for convenience in
// string conversions each int stores only 9 decimal digits i.e. each
// int can be in the range 0 to 999999999.
// This is very close to the way Python does it as Python uses an array
// of int with up to 2^30 in each int.
// ---------------------------------------------------------------------

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include "ulimitedint.h"
using namespace std;

// Size of each int in the array
#define MAX_DIGITS 9
#define MAX_VALUE  1000000000

// ---------------------------------------------------------------------
// Vector helper functions
// -----------------------
// The vectors have the LSB first
// ---------------------------------------------------------------------

// This is just for debugging
// static void vec_print(string s, vector<int>& v) {
//     cout << s << " {";
//     for (int i = 0; i < v.size(); i++) {
//         if (i > 0)
//             cout << ", ";
//         cout << v[i];
//     }
//     cout << "}" << endl;
// }

// Compare two vectors treating both as unsigned numbers
// Return +1 if v1 > v2, 0 if v1 == v2 or -1 if v1 < v2
int vec_compare(vector<int>& v1, vector<int>& v2) {
    // Check the sizes
    if (v1.size() > v2.size())
        return 1;
    if (v1.size() < v2.size())
        return -1;

    // If the signs and sizes are equal we have to compare digit by digit
    for (int i = v1.size() - 1; i >= 0; i--) {
        if (v1[i] > v2[i])
            return 1;
        if (v1[i] < v2[i])
            return -1;
    }

    // If we got here the numbers are equal
    return 0;
}

// Subtract two vectors
// We assume v1 >= v2. It's up to the caller to ensure this.
void vec_subtract(vector<int>& v1, vector<int>& v2, vector<int>& diff) {
    diff.clear();

    // Do the subtraction
    for (int i = 0; i < v1.size(); i++)
        diff.push_back(v1[i] - (i < v2.size() ? v2[i] : 0));

    // Process any borrows
    for (int i = 0; i < diff.size(); i++) {
        if (diff[i] < 0) {
            diff[i+1]--;
            diff[i] += MAX_VALUE;
        }
    }

    // Trim leading zeros
    int size_diff = diff.size();
    for (int i = size_diff-1; i > 0; i--)
        if (diff[i] == 0)
            diff.pop_back();
}

// Multiply a vector by an int
void vec_multiplyint(vector<int>& v, int m, vector<int>& product) {
    // We need to use uint64_t for intermediate storage as the elements
    // can be up to 999999999^2.
    vector<uint64_t> p;
    p.resize(v.size());

    // Do the multiplication
    for (int i = 0; i < v.size(); i++)
        p[i] = v[i]*m;

    // Process any carries
    for (int i = 0; i < v.size(); i++) {
        if (p[i] >= MAX_VALUE) {
            // If necessary make room for the final carry
            if (i+1 >= p.size())
                p.push_back(0);
            v[i+1] += v[i]/MAX_VALUE;
            v[i] = v[i] % MAX_VALUE;
        }
    }

    // And copy the result into the product
    product.clear();
    for (int i = 0; i < p.size(); i++)
        product.push_back((int) p[i]);
}

// Divide two identically sized vector<int>
// The hard bit is the quotient estimation as the qotient can be in the
// range 0 to 999999999. So we find th quotient using a binary search.
int vec_divide(vector<int>& v1, vector<int>& v2, vector<int>& remainder) {
    // Initialise the output vectors
    remainder.clear();

    // Check for v1 < v2
    int compare = vec_compare(v1, v2);
    if (compare < 0) {
        remainder = v1;
        return 0;
    }
    // Check for v1 == v2
    if (compare == 0) {
        return 1;
    }

    // At this point we know v1 > v2 and we have to find an int n such
    // that v1 >= v2*n. We are going to do this with a binary search.
    int lower = 1;
    int upper = v1[v1.size()-1]/v2[v1.size()-1];
    if (upper == 1) {
        vec_subtract(v1, v2, remainder);
        return 1;
    }

    // Do the binary search
    upper += 1;
    int middle;
    vector<int> product;
    while (lower < upper-1) {
        middle = (upper+lower)/2;
        vec_multiplyint(v2, middle, product);

        compare = vec_compare(product, v1);
        // If we got lucky and hit the quotient we can return immediately
        // It's unlikely, but it could happen.
        if (compare == 0) {
            return middle;
        }

        if (compare < 0)
            lower = middle;
        else
            upper = middle;
    }

    // At this point product is the largest integer multiple of v2 less
    // than v1 and lower is that integer.
    vec_multiplyint(v2, lower, product);
    vec_subtract(v1, product, remainder);
    return lower;
}

// Divide two vector<int> i.e. v1/v2.
// This uses traditional long division. The hard work is done by the
// vec_divide function defined above.
void vec_divide(vector<int>& v1, vector<int>& v2, vector<int>& quotient, vector<int>& remainder) {
    // Initialise the output vectors
    quotient.clear();
    remainder.clear();

    // Check for v1 < v2
    int compare = vec_compare(v1, v2);
    if (compare < 0) {
        quotient.push_back(0);
        remainder = v1;
        return;
    }
    // Check for v1 == v2
    if (compare == 0) {
        quotient.push_back(1);
        remainder.push_back(0);
        return;
    }

    // Long division
    int size1 = v1.size();
    int size2 = v2.size();

    // Start by getting the size2 most significant digits of v1
    vector<int> v;
    int cur_digit;
    for (int i = size1 - size2; i < size1; i++)
        v.push_back(v1[i]);
    cur_digit = size1 - size2;

    while (cur_digit >= 0) {
        // Divide the first size2 digits of v1 by v2
        int q = vec_divide(v, v2, remainder);
        // And append the quotient
        quotient.push_back(q);
        // Now get the next digits from v1
        cur_digit--;
        if (cur_digit >= 0) {
            v.clear();
            // Put the next digit at the start of v
            v.push_back(v1[cur_digit]);
            // And append the remainder
            v.insert(v.end(), remainder.begin(), remainder.end());
        }
    }

    // At this point the division is completed however the digits in
    // quotient are in the wrong order.
    reverse(quotient.begin(), quotient.end());
}

// ---------------------------------------------------------------------
// UnlimitedInt helper functions
// ----------------------------
// These have to be non-class functions since we can't modify the header
// ---------------------------------------------------------------------

// Check if an unlimited int is zero
static bool ui_iszero(UnlimitedInt* ui) {
    int* pui = ui->get_array();
    for (int i = 0; i < ui->get_size(); i++)
        if (pui[i] != 0)
            return false;
    return true;
}

// Compare two unlimited ints
// Return +1 if i1 > i2, 0 if i1 == i2 or -1 if i1 < i2
static int ui_compare(UnlimitedInt* i1, UnlimitedInt* i2) {
    // Check the signs first
    if (i1->get_sign() == 1 && i2->get_sign() == -1)
        return 1;
    if (i1->get_sign() == -1 && i2->get_sign() == 1)
        return -1;

    // If the signs are the same check the sizes
    // We are comparing the moduli, so if |i1| > |i2| we need to return
    // +1 if both are positive or -1 if both are negative.
    if (i1->get_size() > i2->get_size())
        return i1->get_sign();
    if (i1->get_size() < i2->get_size())
        return -(i1->get_sign());

    // If the signs and sizes are equal we have to compare digit by digit
    int* p1 = i1->get_array();
    int* p2 = i2->get_array();
    for (int i = i1->get_size() - 1; i >= 0; i--) {
        if (p1[i] > p2[i])
            return i1->get_sign();
        if (p1[i] < p2[i])
            return -(i1->get_sign());
    }

    // If we got here the numbers are equal
    return 0;
}

// ---------------------------------------------------------------------
// Class functions
// ---------------
// ---------------------------------------------------------------------

// Constructors
UnlimitedInt::UnlimitedInt() {
    size = 0;
    sign = 1;
    unlimited_int = NULL;
}

// Create from string
UnlimitedInt::UnlimitedInt(string s) {
    size = 0;
    sign = 1;

    // We'll use a vector for intermediate processing
    vector<int> v;
    int v_off = -1;

    // Ignore leading spaces
    int firstdigit = 0;
    while (firstdigit < s.size() && s[firstdigit] == ' ')
        firstdigit++;
    if (firstdigit == s.size())
        return;

    // Check for a sign
    if (s[firstdigit] == '-') {
        sign = -1;
        firstdigit++;
    }
    else if (s[firstdigit] == '+') {
        sign = 1;
        firstdigit++;
    }
    // Check we have at least one digit
    if (firstdigit == s.size())
        return;
    if (s[firstdigit] < '0' || s[firstdigit] > '9')
        return;

    // Now find the last digit
    int lastdigit;
    for (lastdigit = firstdigit+1; lastdigit < s.size(); lastdigit++)
        if (s[lastdigit] < '0' || s[lastdigit] > '9')
            break;
    lastdigit--;

    // Go through the string from least to most significant digit
    int num_digits = 0, multiplier = 1;
    for (int digit = lastdigit; digit >= firstdigit; digit--) {
        // If we've put nine digits in the current int we need to add a
        // new int.
        if (num_digits % 9 == 0) {
            v.push_back(0);
            v_off++;
            multiplier = 1;
        }
        // Add the next digit
        v[v_off] += ((int) (s[digit] - '0'))*multiplier;
        multiplier *= 10;
        num_digits++;
    }

    // Copy the data from the vector to the buffer
    size = v.size();
    unlimited_int = new int[size];
    for (int i = 0; i < size; i++)
        unlimited_int[i] = v[i];
}

// Create from int
UnlimitedInt::UnlimitedInt(int i) {
    size = 1;
    sign = i < 0 ? -1 : 1;
    unlimited_int = new int[1];
    unlimited_int[0] = i < 0 ? -i : i;
}
// 
UnlimitedInt::UnlimitedInt(int* ulimited_int, int cap, int sgn, int sz) {
    unlimited_int = new int;
    *unlimited_int = *ulimited_int;
    size = sz;
    sign = sgn;
}
// The destructor needs to free the buffer if it has been allocated
UnlimitedInt::~UnlimitedInt() {
    if (unlimited_int)
        delete unlimited_int;
}

// Return the size of the unlimited int
int UnlimitedInt::get_size() {
    return size;
}

// Return the unlimited_int array
int* UnlimitedInt::get_array() {
    return unlimited_int;
}

// Get sign
int UnlimitedInt::get_sign() {
    return sign;
}
// Get capacity
int UnlimitedInt::get_capacity() {
    return 1;
}
// Add i1 to i2
UnlimitedInt* UnlimitedInt::add(UnlimitedInt* i1, UnlimitedInt* i2) {
    // If the signs differ we do a subtraction instead
    if (i1->sign == 1 && i2->sign == -1) {
        i2->sign = +1; // Change i2's sign for the subtraction
        UnlimitedInt* ui = sub(i1, i2);
        i2->sign = -1; // Change i2's sign back to what it was
        return ui;
    }
    else if (i1->sign == -1 && i2->sign == +1) {
        i1->sign = +1; // Change i1's sign for the subtraction
        UnlimitedInt* ui = sub(i2, i1);
        i1->sign = -1; // Change i1's sign back to what it was
        return ui;
    }

    // It will be convenient to make i1 the number with most digits
    if (i1->size < i2->size) {
        UnlimitedInt* u = i1;
        i1 = i2;
        i2 = u;
    }

    // Copy the numbers from i1 into a vector
    vector<int> v;
    for (int i = 0; i < i1->size; i++)
        v.push_back(i1->unlimited_int[i]);
    // And add the numbers from i2
    for (int i = 0; i < i2->size; i++)
        v[i] += i2->unlimited_int[i];

    // And now go through the vector processing any carries
    for (int i = 0; i < v.size(); i++) {
        // If the current int is > MAX_VALUE there's a carry
        if (v[i] >= MAX_VALUE) {
            // We might need to add a new int for the carry
            if (i == v.size() -1)
                v.push_back(0);
            // Add the carry
            v[i+1]++;
            // And remove it from the current int
            v[i] -= MAX_VALUE;
        }
    }

    // And copy the vector into a new UnlimitedInt
    UnlimitedInt* ui = new UnlimitedInt();
    ui->size = v.size();
    // If the two numbers are negative the sum is negative
    if (i1->sign == -1)
        ui->sign = -1;
    // Copy the digits
    ui->unlimited_int = new int[v.size()];
    for (int i = 0; i < v.size(); i++)
        ui->unlimited_int[i] = v[i];

    // And return the new number
    return ui;
}

// Subtract i2 from i1
UnlimitedInt* UnlimitedInt::sub(UnlimitedInt* i1, UnlimitedInt* i2) {
    // If the signs differ we do an addition instead
    if (i1->sign == 1 && i2->sign == -1) {
        i2->sign = +1; // Change i2's sign for the subtraction
        UnlimitedInt* ui = add(i1, i2);
        i2->sign = -1; // Change i2's sign back to what it was
        return ui;
    }
    else if (i1->sign == -1 && i2->sign == +1) {
        i1->sign = +1; // Change i1's sign for the subtraction
        UnlimitedInt* ui = add(i1, i2);
        i1->sign = -1; // Change i1's sign back to what it was
        ui->sign = -1; // -ve minus +ve = -ve
        return ui;
    }

    // If the numbers are equal we can immediately return 0
    int compared = ui_compare(i1, i2);
    if (compared == 0)
        return new UnlimitedInt(0);

    // It will be convenient to make i1 the larger number
    if (compared < 0) {
        UnlimitedInt* u = i1;
        i1 = i2;
        i2 = u;
    }

    // Copy the numbers from i1 into a vector
    vector<int> v;
    for (int i = 0; i < i1->size; i++)
        v.push_back(i1->unlimited_int[i]);
    // And add the numbers from i2
    for (int i = 0; i < i2->size; i++)
        v[i] -= i2->unlimited_int[i];

    // And now go through the vector processing any borrows
    for (int i = 0; i < v.size(); i++) {
        // If the current int is < 0 there's a borrow
        if (v[i] < 0) {
            // Subtract the borrow
            v[i+1]--;
            // And add it to the current int
            v[i] += MAX_VALUE;
        }
    }

    // And copy the vector into a new UnlimitedInt
    UnlimitedInt* ui = new UnlimitedInt();
    ui->size = v.size();
    if (compared < 0)
        ui->sign = -1;

    // Remove any leading zeros
    for (int i = v.size()-1; v[i] == 0; i--)
        ui->size--;

    // Copy in the remaining digits
    if (ui->size == 0) {
        ui->unlimited_int = new int[1];
        ui->unlimited_int = 0;
    }
    else {
        ui->unlimited_int = new int[ui->size];
        for (int i = 0; i < ui->size; i++)
            ui->unlimited_int[i] = v[i];
    }

    // And return the new number
    return ui;
}

// Multiply i1 by i2
UnlimitedInt* UnlimitedInt::mul(UnlimitedInt* i1, UnlimitedInt* i2) {
    // If either number is zero we can immediately return 0
    if ((i1->size == 1 && i1->unlimited_int[0] == 0) || (i2->size == 1 && i2->unlimited_int[0] == 0))
        return new UnlimitedInt(0);

    // It will be convenient to make i1 the number with most digits
    if (i1->size < i2->size) {
        UnlimitedInt* u = i1;
        i1 = i2;
        i2 = u;
    }

    // Create a vector<uint64_t> to store the product. The max size we
    // could need is the sum of the two sizes.
    // Note that we need uint64_t as the max value of an element could
    // be 999999999^2.
    vector<uint64_t> v;
    v.resize(i1->size + i2->size, 0);

    // Now we have to loop over the digits of i2 multiplying i1 by each
    // digit in turn i.e. just doing long multiplication.
    for (int d2 = 0; d2 < i2->size; d2++) {
        // Multiply i1 by i2[d2] and add to the new number
        for (int d1 = 0; d1 < i1->size; d1++) {
            v[d1+d2] += (uint64_t)i1->unlimited_int[d1]*(uint64_t)i2->unlimited_int[d2];
        }
        // Process any carries
        for (int d1 = 0; d1 < i1->size; d1++) {
            if (v[d1+d2] >= MAX_VALUE) {
                v[d1+d2+1] += v[d1+d2]/MAX_VALUE;
                v[d1+d2] = v[d1+d2] % MAX_VALUE;
            }
        }
    }

    // And copy the vector into a new UnlimitedInt
    UnlimitedInt* ui = new UnlimitedInt();
    ui->size = v.size();
    ui->sign = i1->sign*i2->sign;

    // Remove any leading zeros
    for (int i = v.size()-1; v[i] == 0; i--)
        ui->size--;

    // Copy the digits
    ui->unlimited_int = new int[ui->size];
    for (int i = 0; i < ui->size; i++)
        ui->unlimited_int[i] = (int) v[i];

    // And return the new number
    return ui;
}

// Multiply i1 by i2
UnlimitedInt* UnlimitedInt::div(UnlimitedInt* i1, UnlimitedInt* i2) {
    // Division by zero!
    if (ui_iszero(i2))
        throw runtime_error("Division by zero");

    // if i1 < i2 the result is zero
     int compare = ui_compare(i1, i2);
    // if (compare < 0)
    //     return new UnlimitedInt(0);

    // If i1 == i2 the result is 1
    if (compare == 0)
        return new UnlimitedInt(1);

    // If both sizes are 1 this is easy
    // if (i1->size == 1 && i2->size == 1)
    //     return new UnlimitedInt(i1->unlimited_int[0]/i2->unlimited_int[0]);

    // We need to copy the data to vectors, and use the the vector
    // helper functions.
    vector<int> v1, v2, quotient, remainder;
    for (int i = 0; i < i1->size; i++)
        v1.push_back(i1->unlimited_int[i]);
    for (int i = 0; i < i2->size; i++)
        v2.push_back(i2->unlimited_int[i]);
    vec_divide(v1, v2, quotient, remainder);

    // Return the quotient in a new ui
    UnlimitedInt* ui = new UnlimitedInt();
    ui->sign = i1->sign*i2->sign;
    ui->size = quotient.size();
    ui->unlimited_int = new int [ui->size];
    for (int i = 0; i < ui->size; i++)
        ui->unlimited_int[i] = quotient[i];
    if(ui->sign<0)
        ui=UnlimitedInt::sub(ui,new UnlimitedInt(1));
    // And return the new number
    return ui;
}

// i1 modulus i2
// TODO: This only works for positive numbers so far
UnlimitedInt* UnlimitedInt::mod(UnlimitedInt* i1, UnlimitedInt* i2) {
    // Division by zero!
    if (ui_iszero(i2))
        throw runtime_error("Division by zero");

    // If i1 == i2 the result is 0
    int compare = ui_compare(i1, i2);
    if (compare == 0)
        return new UnlimitedInt(0);

    // if i1 < i2 the result is i1
    if (compare < 0) {
        UnlimitedInt* ui = new UnlimitedInt();
        ui->size = i1->size;
        ui->sign = i1->sign;
        ui->unlimited_int = new int[ui->size];
        for (int i = 0; i < i1->size; i++)
            ui->unlimited_int[i] = i1->unlimited_int[i];
        return ui;
    }

    // If both sizes are 1 this is easy
    if (i1->size == 1 && i2->size == 1)
        return new UnlimitedInt(i1->unlimited_int[0] % i2->unlimited_int[0]);

    // We need to copy the data to vectors, and use the the vector
    // helper functions.
    vector<int> v1, v2, quotient, remainder;
    for (int i = 0; i < i1->size; i++)
        v1.push_back(i1->unlimited_int[i]);
    for (int i = 0; i < i2->size; i++)
        v2.push_back(i2->unlimited_int[i]);
    vec_divide(v1, v2, quotient, remainder);

    // Return the remainder in a new ui
    // TODO: I'll have to think about the signs here
    UnlimitedInt* ui = new UnlimitedInt();
    ui->size = remainder.size();
    ui->unlimited_int = new int [ui->size];
    for (int i = 0; i < ui->size; i++)
        ui->unlimited_int[i] = remainder[i];

    // And return the new number
    return ui;
}

// Convert to string
string UnlimitedInt::to_string() {
    string s = "";
    if (sign == -1)
        s.push_back('-');

    // Work through the numbers from most to least significant
    for (int i = size-1; i >= 0; i--) {
        string si = std::to_string(unlimited_int[i]);
        // if this isn't the first digit pad the string to 9 digits
        if (i != size-1) {
            if (si.size() < MAX_DIGITS) {
                si = string(MAX_DIGITS-si.size(), '0') + si;
            }
        }
        s += si;
    }

    // Return the string
    return s;
}