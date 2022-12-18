#include <iostream>
#include <vector>
#include <cmath>
#include "manageInput.hpp"

using namespace std;

vector<pair<double, double> >readInput(int n)
{
    vector<pair<double, double> >vertexes;
    double number,a,b;
    for(int i=0; i<n; i++) //reading the input
    {
        cin>>number>>a>>b;
        vertexes.push_back(make_pair(a,b));
    }
    return vertexes;
}

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
vector<vector<double> >lss_edges(vector<vector<double> >edges)//return adjacency matrix when given edges (vertex a, vertex b, distance)
{
    int vertexes=(1+sqrt(8*(int)edges.size()+1))/2;//solved quadratic equation, calculates the number of vertexes and build square matrix
    vector<double>help;
    vector<vector<double> >distances;
    for(int i=0; i<vertexes; i++)help.push_back(0);
    for(int i=0; i<vertexes; i++)distances.push_back(help);//needs improvement, I don't get it how to make 2-dimensional vector using resize XD
    for(int i=0; i<(int)edges.size(); i++)
    {
        distances[(int)edges[i][0]][(int)edges[i][1]]=edges[i][2];
        distances[(int)edges[i][1]][(int)edges[i][0]]=edges[i][2];
    }
    return distances;
}

vector<vector<double> >make_edges(vector<pair<double, double> > points)
{
    vector<vector<double> >edges;
    vector<double>one_edge;
    for(int i=0; i<(int)points.size()-1; i++)
    {
        for(int j=i+1; j<(int)points.size(); j++)
        {
            one_edge.clear();
            one_edge.push_back(double(i));
            one_edge.push_back(double(j));
            one_edge.push_back(dist(double(points[i].first),double(points[i].second),double(points[j].first),double(points[j].second)));
            edges.push_back(one_edge);
        }
    }
    return edges;
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
