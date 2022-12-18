#include "solution.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
extern int n;
extern vector <vector<double> >distances;

Solution::Solution(vector<int>points, double sum)
{
    this->points=points;
    this->sum=sum;
}
Solution::Solution(string path)// add already found path to the population. Use only for testing!!!
{
    vector<int>help;
    string v="";
    int num;
    for(int i=0; i<path.size(); i++)
    {
        if(path[i]=='-')
        {
            num=atoi(v.c_str());
            help.push_back(num);
            v="";
        }
        else
        {
            v+=path[i];
        }
    }
    this->points=help;
    this->sum=0;
}
void Solution::differentSolution(vector<int>points, double sum)
{
    this->points=points;
    this->sum=sum;
}
void Solution::mutate()
{
    int a=rand()%n;
    int b=rand()%n;
    if(b<a)swap(a,b);
    random_shuffle(this->points.begin()+a, this->points.begin()+b);
}
void Solution::display()
{
    for(int i=0; i<(int)points.size(); i++)
    {
        cout<<points[i]<<"-";
    }
    cout<<points[0]<<endl;
    cout<<sum<<endl;
}
vector<int> Solution::getPath()
{
    return this->points;
}
void Solution::CalcLength()
{
    this->sum=0;
    for(int i=0; i<n-1; i++)
    {
        this->sum+=distances[this->points[i]-1][this->points[i+1]-1];
    }
    this->sum+=distances[this->points[n-1]-1][this->points[0]-1];
}
bool Solution::operator<(const Solution& a) const
{
    return sum<a.sum;
}
bool Solution::operator==(const Solution& a) const
{
    if(sum!=a.sum)return false;
    else
    {
        for(int i=0; i<(int)this->points.size(); i++)
        {
            if(points[i]!=a.points[i])return false;
        }
    }
    return true;
}

