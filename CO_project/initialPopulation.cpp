#include "initialPopulation.hpp"
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

extern int n;
extern vector <vector<double> >distances;

Solution findGreedy(vector<pair<double, double> > vertexes)
{
    vector<int>greedyPoints;
    vector <bool> visited;
    for(int i=0; i<n; i++)visited.push_back(false);
    double sum=0;
    double minimum=double(INT_MAX);
    int currentpoint=0;
    int nextpoint=0;
    for(int i=0; i<n-1; i++)
    {
        minimum=double(INT_MAX);
        visited[currentpoint]=true;
        for(int j=0; j<n; j++)
        {
            if(visited[j]==false && distances[currentpoint][j]<minimum)
            {
                minimum=distances[currentpoint][j];
                nextpoint=j;
            }
        }
        sum+=minimum;
        currentpoint=nextpoint;
        greedyPoints.push_back(nextpoint+1);
    }
    greedyPoints.push_back(1);
    sum+=distances[currentpoint][0];
    Solution res(greedyPoints,sum);
    return res;
}

