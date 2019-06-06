//
//  main.cpp
//  stronglyConnectedComponents
//
//  Created by Kris Rajendren on Jun/5/19.
//  Copyright © 2019 House Shout. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <set>

using namespace std;

struct node;

typedef node* Node;
typedef unsigned long Time;
typedef unsigned long ID;
typedef unsigned long Size;
typedef set<Node> Nodes;
typedef Nodes::iterator NodesIterator;
typedef pair<Time, ID> TimeNodePair;
typedef unordered_map<ID, Node> Graph;
typedef unordered_map<ID, Node>::const_iterator GraphIterator;
typedef set<ID> StronglyConnectedComponent;
typedef StronglyConnectedComponent SCC;

typedef unordered_map<ID, SCC> LeaderStronglyConnectedComponentMap;
typedef LeaderStronglyConnectedComponentMap LSCCmap;
typedef LSCCmap::const_iterator LSCCmapIterator;
typedef priority_queue<Size> StronglyConnectedComponentsSizeHeap;
typedef StronglyConnectedComponentsSizeHeap SCCsizeHeap;


class CompareTimeNodePair
{
public:
    bool operator()(TimeNodePair lhs, TimeNodePair rhs)
    {
        return lhs.first > rhs.first;
    }
};

typedef priority_queue<TimeNodePair, vector<TimeNodePair>, CompareTimeNodePair> TimeHeap;


bool GLOBAL_DEBUG_FLAG = false;

struct node
{
    ID id;
    bool visited = false;
    Time f = 0;        // Finishing time
    
    Nodes next; Nodes prev;
    
    ID leaderID = 0;
    
    node(ID num)
    {
        id = num; next = Nodes(); prev = Nodes();
    }
    
    ID getID()
    {
        return id;
    }
    
    bool isNotVisited()
    {
        return !visited;
    }
    
    bool isVisited()
    {
        return visited;
    }
    
    Time getFinishTime()
    {
        return f;
    }
    
    void markVisited()
    {
        visited = true;
    }
    
    void markNotVisited()
    {
        visited = false;
    }
    
    void setFinishTime(Time t)
    {
        f = t;
    }
    
    void addNext(Node n)
    {
        next.insert(n);
    }
    
    void addPrev(Node m)
    {
        prev.insert(m);
    }
    
    void setLeader(Node l)
    {
        if (l != NULL) leaderID = l->getID();
    }
    
    void printID()
    {
        cout << id << ":" ;
    }
    
    void printFinishTime()
    {
        cout << "Finish time of node: " << getID() << " = " << getFinishTime() << endl;
    }
    
    void printNodes(const Nodes& nodes, string name)
    {
        NodesIterator nIt;
        
        cout << name << " Nodes: [";
        
        for (nIt = nodes.begin(); nIt != nodes.end(); ++nIt)
        {
            cout << (*nIt)->id << ", ";
        }
        
        cout << "]" << endl;
    }
    
    void printNext()
    {
        printNodes(next, "Next");
    }
    
    void printPrev()
    {
        printNodes(prev, "Previous");
    }
};

Graph processGraphFromFile(const string& fileName)
{
    Graph G;
    ID u, v;
    ifstream file;
    
    file.open(fileName);
    
    while(file >> u >> v)
    {
        Node m, n;
        if (G.count(u) == 0)
        {
            m = new node(u); G[u] = m;
        }
        else
            m = G[u];
        if (G.count(v) == 0)
        {
            n = new node(v); G[v] = n;
        }
        else
            n = G[v];
        
        m->addNext(n); n->addPrev(m);
    }
    
    return G;
}

void DFSnode(Node node, Time& finish, TimeHeap& T, bool trackFinish = true, bool reverse = false, bool basedOnFinishTimes = false, Node leader = NULL, bool debug = GLOBAL_DEBUG_FLAG)
{
    if (!reverse)
    {
        if (node == NULL || node->isVisited())
        {
            return;
        }
        else
        {
            node->markVisited();
            
            NodesIterator nIt;

            if (node->next.size() != 0)
            {
                for (nIt = node->next.begin(); nIt != node->next.end(); ++nIt)
                {
                    Node next = *nIt;
                    if (next != NULL && next->isNotVisited())
                    {
                        DFSnode(next, finish, T, trackFinish, reverse, basedOnFinishTimes, leader, debug);
                    }
                }
            }
        }
    }
    else
    {
        if (node == NULL || node->isNotVisited())
        {
            return;
        }
        else
        {
            node->markNotVisited();
            
            NodesIterator nIt;
            
            if (node->prev.size() != 0)
            {
                for (nIt = node->prev.begin(); nIt != node->prev.end(); ++nIt)
                {
                    Node prev = *nIt;
                    if (prev != NULL && prev->isVisited())
                    {
                        DFSnode(prev, finish, T, trackFinish, reverse, basedOnFinishTimes, leader, debug);
                    }
                }
            }
        }
    }
        
    if (trackFinish)
    {
        finish++;
        node->setFinishTime(finish);
        TimeNodePair p = make_pair(node->getFinishTime(), node->getID());
        T.push(p);
        
        if (debug)
        {
            node->printFinishTime();
        }
    }
    
    if (basedOnFinishTimes)
    {
        node->setLeader(leader);
        
        if (debug)
        {
            cout << "Setting leader of node: " << node->getID() << " to " << leader->getID() << " " << node->leaderID << endl;
        }
    }
}


void DFSgraph(const Graph& G, TimeHeap& T, bool trackFinish = true, bool reverse = false, bool basedOnFinishTimes = false)
{
    if (G.size() == 0) return;
    
    if (!basedOnFinishTimes)
    {
        GraphIterator gIt = G.begin();
        Time finish = 0;
        
        while (gIt != G.end())
        {
            Node node = gIt->second; DFSnode(node, finish, T, trackFinish, reverse, basedOnFinishTimes); gIt++;
        }
    }
    else
    {
        Time finish = 0;
        
        while(!T.empty())
        {
            TimeNodePair p = T.top(); T.pop();
            ID id = p.second; Node leader = G.at(id);
            DFSnode(leader, finish, T, trackFinish, reverse, basedOnFinishTimes, leader);
        }
    }
}

LSCCmap AggregateStronglyConnectedComponents(const Graph& G, bool debug = GLOBAL_DEBUG_FLAG)
{
    LSCCmap lsccMap;
    GraphIterator gIt;
    ID id, leaderID;
    
    for (gIt = G.begin(); gIt != G.end(); ++gIt)
    {
        id = gIt->first;
        Node node = gIt->second;
        leaderID = node->leaderID;
        
        if (lsccMap.count(leaderID) == 0)
        {
            SCC scc; scc.insert(id);
            lsccMap[leaderID] = scc;
        }
        else
        {
            lsccMap[leaderID].insert(id);
        }
        
        if (debug)
        {
            cout << "Inserting node: " << id << " into the SCC of leader " << leaderID << endl;
        }
    }
    
    return lsccMap;
}

LSCCmap calculateStronglyConnectedComponents(const Graph& G, TimeHeap& T)
{
    bool trackFinish, reverse, basedOnFinishTimes;
    
    trackFinish = true; reverse = false; basedOnFinishTimes = false;
    
    DFSgraph(G, T, trackFinish, reverse, basedOnFinishTimes);
    
    trackFinish = false; reverse = true; basedOnFinishTimes = true;
    
    DFSgraph(G, T, trackFinish, reverse, basedOnFinishTimes);
    
    return AggregateStronglyConnectedComponents(G);
}

void printGraph(const Graph& G)
{
    GraphIterator gIt;
    
    for (gIt = G.begin(); gIt != G.end(); ++gIt)
    {
        cout << "Graph ID: " << gIt->first << " Node ID: " << gIt->second->id << endl;
        
        gIt->second->printNext(); gIt->second->printPrev();
    }
}

void printStronglyConnectedComponentSizes(const LSCCmap& lsccMap)
{
    SCCsizeHeap sccSizeHeap;
    LSCCmapIterator lIt;
    
    for (lIt = lsccMap.begin(); lIt != lsccMap.end(); ++lIt)
    {
        SCC scc = lIt->second;
        sccSizeHeap.push(scc.size());
    }
    
    int count = 1;
    while(!sccSizeHeap.empty())
    {
        Size size = sccSizeHeap.top(); sccSizeHeap.pop();
        cout << "Strongly connected component size " << count << ": " << size << endl;
        count++;
    }
}

int main(int argc, const char * argv[])
{
    // insert code here...
    
    string fileName = "scc.txt";
//    fileName = "input_mostlyCycles_68_320000.txt";
//    fileName = "input_mostlyCycles_51_20000.txt";
    
    Graph G = processGraphFromFile(fileName);
    
    TimeHeap T;
    
    LSCCmap lsccMap = calculateStronglyConnectedComponents(G, T);

    printStronglyConnectedComponentSizes(lsccMap);
    
//    DFSgraph(G, T);
    
    return 0;
}
