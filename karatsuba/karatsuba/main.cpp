//
//  main.cpp
//  karatsuba
//
//  Created by Kris Rajendren on May/30/19.
//  Copyright © 2019 House Shout. All rights reserved.
//

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

const unsigned long BASE = 10;

string reverse(const string& s)
{
    string r;
    
    for (int i=0; i < s.size(); ++i)
    {
        r.push_back(s[s.size() - 1 - i]);
    }
    
    return r;
}

void printVector(const vector<int>& A)
{
    for (int i=0; i < A.size(); ++i)
    {
        cout << A[i];
    }
    cout << endl;
}

void printVectorInReverse(const vector<int>& A)
{
    for (int i=0; i < A.size(); ++i)
    {
        cout << A[A.size() - 1 - i];
    }
    cout << endl;
}

vector<int> digitize(int a)
{
    vector<int> A;
    
    while (a > 0)
    {
        A.push_back(a % BASE);
        a = (a - (a % BASE)) / BASE;
    }
    
    return A;
}

vector<int> digitize(const string& s)
{
    vector<int> S;
    
    for (int i=0; i < s.size(); ++i)
    {
        int a = s[i] - '0'; S.push_back(a);
    }
    
    return S;
}

vector<int> exp(int n)
{
    vector<int> A(n + 1, 0); A[n] = 1;
    return A;
}

vector<int> exp(const vector<int>& A, int n)
{
    vector<int> B(A.size() + n, 0);
    
    for (int i=n; i < B.size(); ++i)
    {
        B[i] = A[i - n];
    }
    
    return B;
}

vector<int> operator+(const vector<int>& A, const vector<int>& B)
{
    unsigned long sa, sb; sa = A.size(); sb = B.size();
    unsigned long m, n;
    m = min(sa, sb); n = max(sa, sb);
    
    vector<int> S;
    
    int c, s; c = s = 0;
    
    for (unsigned long i=0; i < m; ++i)
    {
        s = A[i] + B[i] + c;
        if (s >= 0)
        {
            S.push_back(s % BASE); c = s / BASE;
        }
        else
        {
            S.push_back((s + BASE) % BASE); c = -1;
        }
    }
    
    for (unsigned long i=m; i < n; ++i)
    {
        if (A.size() > B.size())
        {
            s = A[i] + c;

            if (i != n - 1)
            {
                S.push_back(s % BASE); c = s / BASE;
            }
            else
            {
                if (s != 0) S.push_back(s % BASE); c = s / BASE;
            }
        }
        else
        {
            s = B[i] + c;
            if (i != n - 1)
            {
                S.push_back(s % BASE); c = s / BASE;
            }
            else
            {
                if (s != 0) S.push_back(s % BASE); c = s / BASE;
            }
        }
    }
    
    if (c != 0) S.push_back(c);
    
    return S;
}

vector<int> operator-(const vector<int>& A)
{
    vector<int> B(A.size(), 0);
    
    for (int i=0; i < A.size(); ++i)
    {
        B[i] = -A[i];
    }
    
    return B;
}

vector<int> operator-(const vector<int>& A, const vector<int>& B)
{
    return A + (-B);
}

vector<int> operator*(int a, const vector<int>& B)
{
    vector<int> p;
    
    int c = 0;
    
    for (int i=0; i < B.size(); ++i)
    {
        int s = a * B[i] + c;
        p.push_back(s % BASE);
        c = s / BASE;
    }
    
    if (c != 0) p.push_back(c);
    
    return p;
}

vector<int> operator*(const vector<int>& B, int a)
{
    return a*B;
}

vector<int> subVector(const vector<int>& A, unsigned long start, unsigned long end)
{
    vector<int> sub;
    
    if (end < start) return sub;
    
    for (unsigned long i=start; i <= end; ++i)
    {
        sub.push_back(A[i]);
    }
    
    return sub;
}

vector<int> karatsuba(const vector<int>& A, const vector<int>& B)
{
    cout << "inside karatsuba." << endl;

    if (A.size() == 1) return A[0] * B;
    if (B.size() == 1) return B[0] * A;
    
    vector<int> P;
    
    unsigned long na = A.size(); unsigned long nb = B.size();
    
    cout << "na = " << na << " and nb = " << nb << endl;
    
    vector<int> C = subVector(A, na/2, na - 1);
    vector<int> D = subVector(A, 0, na/2 - 1);
    vector<int> E = subVector(B, nb/2, nb - 1);
    vector<int> F = subVector(B, 0, nb/2 - 1);
    
    vector<int> CE = karatsuba(C, E);
    vector<int> DF = karatsuba(D, F);
    vector<int> CF = karatsuba(C, F);
    vector<int> DE = karatsuba(D, E);

    return (exp(CE, (na + nb)/2) + exp(CF, na / 2) + exp(DE, nb / 2) + DF);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    string a, b;
    
    cout << "Enter the first number: ";
    cin >> a;
    
    cout << "Enter the second number: ";
    cin >> b;

    vector<int> A, B;

    A = digitize(reverse(a)); B = digitize(reverse(b));

    printVectorInReverse(karatsuba(A, B));

    return 0;
}
