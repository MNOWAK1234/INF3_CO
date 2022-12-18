#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <queue>
#include <algorithm>
#include "manageInput.hpp"
#include "solution.hpp"
#include "initialPopulation.hpp"
#include "parameters.cpp"
#include "reproduction.hpp"

using namespace std;
int  n;
vector <vector<double> >distances;
vector<pair<double, double> > points;
vector<Solution> solutions;
vector<Solution> copied_best;
queue<Solution> best_of_generation;
vector<Solution> extinct;
vector<int>path;
vector <int> parent1;
vector <int> parent2;
vector <int> visited1;
vector <int> visited2;
vector <int> child1;
vector <int> child2;
vector<int> points1;
vector<int> points2;

string found_earlier;

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    srand(time(0));
    cin>>n;
    points=readInput(n);
    distances=lss_points(points); //adjacency matrix
    //solutions.push_back(findGreedy(points));
    //cout<<"greedy:"<<endl;
    //solutions[0].display();
    //cin>>found_earlier;
    //solutions.push_back(Solution(found_earlier));
    for(int i=1; i<=n; i++)path.push_back(i);
    random();
    prepareCross();
    sort(solutions.begin(),solutions.end());
    vector<int>help;
    for(int k=0; k<40000; k++)
    {
        if(k%500==0)
        {
            cout<<k<<endl;
            solutions[0].display();
        }
        copied_best.clear();
        eugenics();
        mutation();
        cycleCross();
        //partiallyMappedCross();
        orderCross();
        endBreed();
        massExtinction();
        random();
    }
    solutions.clear();
    cout<<"Extinct:"<<endl;
    for(int i=0; i<(int)extinct.size(); i++)
    {
        extinct[i].display();
        solutions.push_back(extinct[i]);
    }
    random();
    for(int k=0; k<1000; k++)
    {
        if(k%100==0)
        {
            cout<<k<<endl;
            solutions[0].display();
        }
        copied_best.clear();
        eugenics();
        mutation();
        cycleCross();
        //partiallyMappedCross();
        orderCross();
        endBreed();
        random();
    }
    cout<<"New:"<<endl;
    solutions[0].display();
    solutions.clear();
    solutions.push_back(findGreedy(points));
    cout<<"greedy:"<<endl;
    solutions[0].display();
    for(int k=0; k<1000; k++)
    {
        if(k%100==0)
        {
            cout<<k<<endl;
            solutions[0].display();
        }
        copied_best.clear();
        eugenics();
        mutation();
        cycleCross();
        //partiallyMappedCross();
        orderCross();
        endBreed();
        random();
    }
    cout<<"After greedy:"<<endl;
    solutions[0].display();

}
