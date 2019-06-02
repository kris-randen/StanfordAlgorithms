//
//  main.cpp
//  mergeSort
//
//  Created by Kris Rajendren on Jun/1/19.
//  Copyright © 2019 House Shout. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void print(const vector<int>& v)
{
    for (int i=0; i < v.size(); ++i)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

vector<int> merge(const vector<int>& u, const vector<int>& v)
{
    if (u.size() == 0) return v;
    if (v.size() == 0) return u;
    
    vector<int> w;
    
    int l, r; l = r = 0;
    
    while(true)
    {
        while(l < u.size() && r < v.size() && u[l] <= v[r]) w.push_back(u[l++]);
        while(l < u.size() && r < v.size() && u[l] >= v[r]) w.push_back(v[r++]);
        
        if (l == u.size() || r == v.size()) break;
    }
    
    if (l == u.size())
    {
        while(r < v.size()) w.push_back(v[r++]);
    }
    else if (r == v.size())
    {
        while(l < u.size()) w.push_back(u[l++]);
    }
    
    return w;
}


vector<int> mergeAndCountInversions(const vector<int>& u, const vector<int>& v, unsigned long& n)
{
    if (u.size() == 0) return v;
    if (v.size() == 0) return u;
    
    vector<int> w;
    
    int l, r; l = r = 0;
    
    while(true)
    {
        while(l < u.size() && r < v.size() && u[l] <= v[r]) w.push_back(u[l++]);
        while(l < u.size() && r < v.size() && u[l] >= v[r])
        {
            w.push_back(v[r++]);
            n += u.size() - l;
        }
        
        if (l == u.size() || r == v.size()) break;
    }
    
    if (l == u.size())
    {
        while(r < v.size()) w.push_back(v[r++]);
    }
    else if (r == v.size())
    {
        while(l < u.size()) w.push_back(u[l++]);
    }
    
    return w;
}

vector<int> mergeSort(const vector<int>& v, int start, int end)
{
    if (end < start) return vector<int>();
    if (start == end) return {v[start]};
    
    return (merge(mergeSort(v, start, (start + end) / 2), mergeSort(v, (start + end + 2) / 2, end)));
}

vector<int> mergeSortAndCountInversions(const vector<int>& v, int start, int end, unsigned long& n)
{
    if (end < start) return vector<int>();
    if (start == end) return {v[start]};
    
    return (mergeAndCountInversions(mergeSortAndCountInversions(v, start, (start + end) / 2, n), mergeSortAndCountInversions(v, (start + end + 2) / 2, end, n), n));
}



int main()
{
    // insert code here...
    
    int x;
    ifstream f;
    
    vector<int> v;
    
    f.open("IntegerArray.txt");
    
    while(f >> x)
    {
        v.push_back(x);
    }
    
    unsigned long n = 0;
    
//    print(mergeSortAndCountInversions(v, 0, v.size() - 1, n));
    
    vector<int> u =  mergeSortAndCountInversions(v, 0, v.size() - 1, n);
    
    cout << n << endl;
    return 0;
}
