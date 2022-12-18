#ifndef SOLUTION_HPP_INCLUDED
#define SOLUTION_HPP_INCLUDED

#include <iostream>
#include <vector>
using namespace std;

class Solution
{
    private:
        vector<int>points;
        double sum;
    public:
        Solution(vector<int>points, double sum);
        Solution(string path);
        void differentSolution(vector<int>points, double sum);
        void mutate();
        vector<int> getPath();
        void display();
        void CalcLength();
        bool operator<(const Solution& a) const;
        bool operator==(const Solution& a) const;
};

#endif // SOLUTION_HPP_INCLUDED
