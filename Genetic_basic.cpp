#include <iostream>
#include <math.h>
//#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

const int TOP_SOLUTIONS = 1000; // How many top solutions to select
const int POPULATION = 60000;   // Number of solutions per generation

vector <vector<double> >distances;
struct Solution
{
    vector<int>points;
    double sum;
    void CalcLength()
    {
        sum=0;
        for(int i=1; i<100; i++)
        {
            sum+=distances[i][i+1];
        }
        sum+=distances[100][0];
    }
    bool operator<(const Solution& a) const
    {
        return sum<a.sum;
    }
};

double dist(double x1, double y1, double x2, double y2)//return euclidean distance between 2 points
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

vector<vector<double> >lss_points(vector<pair<double, double> > points)//return adjacency matrix when given coordinates
{
    vector<vector<double> >distances;
    vector<double>one_vertex;
    for(int i=0; i<(int)points.size(); i++)
    {
        one_vertex.clear();
        for(int j=0; j<(int)points.size(); j++)
        {
            one_vertex.push_back(dist(points[i].first,points[i].second,points[j].first,points[j].second));
        }
        distances.push_back(one_vertex);
    }
    return distances;
}

void show_matrix(vector<vector<double> >v)
{
    for(int i=0; i<(int)v.size(); i++)
    {
        for(int j=0; j<(int)v[i].size(); j++)
        {
            cout<<v[i][j]<<"\t";
        }
        cout<<endl;
    }
}

int main()
{
    srand(time(0));
    //greedy solution
    vector<pair<double, double> > points;
    vector <bool> visited;

    int n;
    cin>>n;
    double a,b;

    for(int i=0; i<n; i++) //reading the input
    {
        cin>>a>>b;
        points.push_back(make_pair(a,b));
        visited.push_back(false);
    }

    distances=lss_points(points); //adjacency matrix

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
        //cout<<nextpoint<<"-";
    }
    sum+=distances[currentpoint][0];
    //cout<<sum<<endl;

    // now we have a greedy solution and want to improve

    vector<Solution> solutions;
    vector<Solution> copied_best;
    vector<int>path;
    for(int i=1; i<=100; i++)path.push_back(i);

    //generate random solutions
    for(int i = 0; i < POPULATION; i++)
    {
        random_shuffle(path.begin(), path.end());//tasuje wierzcholki
        solutions.push_back(Solution{path,0});
        solutions[solutions.size()-1].CalcLength();
    }
    for(int i=0; i<100; i++)
    {
        sort(solutions.begin(),solutions.end());
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

        // Mutate the top solutions
        for(int i=0; i<TOP_SOLUTIONS; i++)
        {
            int a=rand()%10+40;
            int b=rand()%10+50;
            random_shuffle(copied_best[i].points.begin()+a, copied_best[i].points.begin()+b);
        }
        // Cross over
        //
        // copy best function
    }

}
