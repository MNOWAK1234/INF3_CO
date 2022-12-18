#include "reproduction.hpp"
#include "solution.hpp"
#include "parameters.cpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>

using namespace std;

extern int n;
extern vector<Solution> solutions;
extern vector<Solution> copied_best;
extern vector<int>path;

extern vector <int> parent1;
extern vector <int> parent2;
extern vector <int> visited1;
extern vector <int> visited2;
extern vector <int> child1;
extern vector <int> child2;
extern vector<int> points1;
extern vector<int> points2;
extern queue<Solution> best_of_generation;
extern vector<Solution> extinct;

void random()
{
    Solution randomized(path,0);
    for(int i = 0; i < POPULATION; i++)
    {
        random_shuffle(path.begin(), path.end());//tasuje wierzcholki
        randomized.differentSolution(path,0);
        randomized.CalcLength();
        solutions.push_back(randomized);
    }
}
void prepareCross()
{
    for(int j=0; j<=n; j++)
    {
        parent1.push_back(0);
        parent2.push_back(0);
        visited1.push_back(0);
        visited2.push_back(0);
    }
}
void eugenics()
{
    copied_best.clear();
    for(int i=0; i<TOP_SOLUTIONS; i++)
    {
        copied_best.push_back(solutions[i]);
    }
    for(int i=0; i<TOP_SOLUTIONS; i++)
    {
        copied_best.push_back(solutions[i]);
    }
    solutions.clear();
}
void mutation()
{
    for(int i=0; i<TOP_SOLUTIONS; i++)
    {
        copied_best[i].mutate();
    }
}
void cycleCross()
{
    for(int i=TOP_SOLUTIONS; i<2*TOP_SOLUTIONS; i+=2)
    {
        points1.clear();
        points2.clear();
        for(int j=0; j<=n; j++)
        {
            parent1[j]=0;
            parent2[j]=0;
            visited1[j]=0;
            visited2[j]=0;
            child1.clear();
            child2.clear();
        }
        points1=copied_best[rand()%LEAVE+TOP_SOLUTIONS].getPath();
        points2=copied_best[rand()%TOP_SOLUTIONS+TOP_SOLUTIONS].getPath();
        for(int j=0; j<n; j++) //position table
        {
            parent1[points1[j]]=j;
            parent2[points2[j]]=j;
        }
        int cycleNumber=0;
        int current;
        for(int j=0; j<n; j++)
        {
            current=j;
            if(visited1[current]==0)cycleNumber++;
            while(visited1[current]==0) //find cycle
            {
                visited1[current]=cycleNumber;
                current=parent2[points1[current]];
            }
        }
        for(int j=0; j<n; j++)
        {
            if(visited1[j]%2==1)
            {
                child1.push_back(points1[j]);
                child2.push_back(points2[j]);
            }
            else
            {
                child1.push_back(points2[j]);
                child2.push_back(points1[j]);
            }
        }
        Solution crossed1(child1,0);
        Solution crossed2(child2,0);
        solutions.push_back(crossed1);
        solutions.push_back(crossed2);
    }
}
void partiallyMappedCross()
{
    ;
}
void insertSegment(int startVertex, int endVertex)//helps with orderCross
{
    if((int)child1.size()==startVertex)
    {
        int copyIndex=startVertex;
        while(copyIndex<=endVertex)
        {
            child1.push_back(points1[copyIndex]);
            copyIndex++;
        }
    }
    if((int)child2.size()==startVertex)
    {
        int copyIndex=startVertex;
        while(copyIndex<=endVertex)
        {
            child2.push_back(points2[copyIndex]);
            copyIndex++;
        }
    }
}
void orderCross()
{
    for(int i=TOP_SOLUTIONS; i<2*TOP_SOLUTIONS; i+=2)
    {
        points1.clear();
        points2.clear();
        for(int j=0; j<=n; j++)
        {
            parent1[j]=0;
            parent2[j]=0;
            visited1[j]=0;
            visited2[j]=0;
            child1.clear();
            child2.clear();
        }
        points1=copied_best[rand()%LEAVE+TOP_SOLUTIONS].getPath();
        points2=copied_best[rand()%TOP_SOLUTIONS+TOP_SOLUTIONS].getPath();
        for(int j=0; j<n; j++) //position table
        {
            parent1[points1[j]]=j;
            parent2[points2[j]]=j;
        }
        int a=rand()%n;
        int b=rand()%n;
        if(b<a)swap(a,b);
        for(int j=0; j<n; j++)
        {
            insertSegment(a,b);
            if(parent1[points2[j]]<a || parent1[points2[j]]>b)child1.push_back(points2[j]);
            if(parent2[points1[j]]<a || parent2[points1[j]]>b)child2.push_back(points1[j]);
        }
        insertSegment(a,b);
        Solution crossed1(child1,0);
        Solution crossed2(child2,0);
        solutions.push_back(crossed1);
        solutions.push_back(crossed2);
    }
}
void endBreed()
{
    for(int i=0; i<(int)copied_best.size(); i++)
    {
        solutions.push_back(copied_best[i]);
    }
    for(int i=0; i<(int)solutions.size(); i++)
    {
        solutions[i].CalcLength();
    }
    sort(solutions.begin(),solutions.end());
}
void massExtinction()
{
    best_of_generation.push(solutions[0]);
    if(best_of_generation.size()==STABLE)
    {
        if(solutions[0]==best_of_generation.front())
        {
            extinct.push_back(solutions[0]);
            solutions.clear();
            best_of_generation=queue<Solution>();//clears the queue
        }
        else best_of_generation.pop();
    }
}
