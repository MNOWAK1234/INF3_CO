#ifndef MANAGEINPUT_HPP_INCLUDED
#define MANAGEINPUT_HPP_INCLUDED

#include <vector>
using namespace std;

vector<pair<double, double> >readInput(int n);
double dist(double x1, double y1, double x2, double y2);//return euclidean distance between 2 points
vector<vector<double> >lss_points(vector<pair<double, double> > points);//return adjacency matrix when given coordinates
vector<vector<double> >lss_edges(vector<vector<double> >edges);//return adjacency matrix when given edges (vertex a, vertex b, distance)
vector<vector<double> >make_edges(vector<pair<double, double> > points);
void show_matrix(vector<vector<double> >v);

#endif // MANAGEINPUT_HPP_INCLUDED
