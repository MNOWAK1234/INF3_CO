#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

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
    vector<pair<double, double> > points;
    vector <bool> visited;

    int n;
    cin>>n;
    double number,a,b;

    for(int i=0; i<n; i++) //reading the input
    {
        cin>>number>>a>>b;
        points.push_back(make_pair(a,b));
        visited.push_back(false);
    }

    vector <vector<double> > distances=lss_points(points); //adjacency matrix

    double sum=0;
    double minimum=INT_MAX;
    int currentpoint=0;
    int nextpoint=0;

    for(int i=0; i<n-1; i++)
    {
        minimum=INT_MAX;
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
    }
    sum+=distances[currentpoint][0];
    cout<<sum;
}
