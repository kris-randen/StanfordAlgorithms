//
//  main.cpp
//  quickSort
//
//  Created by Kris Rajendren on Jun/1/19.
//  Copyright © 2019 House Shout. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

typedef vector<int> Ints;

void print(const Ints& V)
{
    for (int i = 0; i < V.size(); ++i)
    {
        cout << V[i] << " ";
    }
    
    cout << endl;
}

enum Partition
{
    Random = 0,
    First,
    Last,
    Median
};


void swap(int& a, int& b)
{
    int temp = a; a = b; b = temp;
}

int medianOf(const Ints& V, int s, int e)
{
    Ints v; Ints index;
    
    v.push_back(V[s]); index.push_back(s);
    v.push_back(V[(e + s) / 2]); index.push_back((e + s) / 2);
    v.push_back(e); index.push_back(e);
    
    int min, max; min = v[0]; max = v[2];
    
    for (int i=0; i < v.size(); ++i)
    {
        if (v[i] < min)
        {
            min = v[i];
            swap(v[i], v[0]); swap(index[i], index[0]);
        }
    }

    
    for (int i=0; i < v.size(); ++i)
    {
        if (v[i] > max)
        {
            max = v[i];
            swap(v[i], v[2]); swap(index[i], index[2]);
        }
    }
    
    print(v); print(index);
    
    return index[1];
}

int partition(const Ints& V, int s, int e, Partition PARTITION = First)
{
    switch (PARTITION)
    {
        case First:
            return s;
        case Last:
            return e;
        case Median:
            return medianOf(V, s, e);
        case Random:
            srand (time(NULL));
            return rand() % (e - s) + s;
    }
    return 0;
}

int sortForGivenPartition(Ints& V, int s, int e, int p)
{
    if (e <= s) return s;
    
    int P = V[p];
    swap(V[s], V[p]);
    
    int l = s + 1;
    
    for (int i = s + 1; i <= e; ++i)
    {
        if (V[i] < P)
        {
            swap(V[i], V[l]); l++;
        }
    }
    
    swap(V[s], V[--l]);
    return l;
}

void quickSort(Ints& V, int s, int e, Partition PARTITION = First)
{
    if (e <= s) return;
    
    int p = partition(V, s, e, PARTITION);
    p = sortForGivenPartition(V, s, e, p);
    
    quickSort(V, s, p-1);
    quickSort(V, p+1, e);
}

void quickSortAndCountComparisons(Ints& V, int s, int e, int& count, Partition PARTITION = First)
{
    if (e <= s) return;
    
    count += e - s;
    
    int p = partition(V, s, e, PARTITION);
    p = sortForGivenPartition(V, s, e, p);
    
    quickSortAndCountComparisons(V, s, p-1, count, PARTITION);
    quickSortAndCountComparisons(V, p+1, e, count, PARTITION);
}

int main(int argc, const char * argv[])
{
    // insert code here...
    
    int x;
    ifstream f;
    
    vector<int> v;
    
    f.open("QuickSort.txt");
    
    while(f >> x)
    {
        v.push_back(x);
    }
    
    int n = 0;
    
    quickSortAndCountComparisons(v, 0, v.size() - 1, n, Random);
    
    print(v); cout << n << endl;
    
    return 0;
}
