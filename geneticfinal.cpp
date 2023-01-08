#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

#define TOP_SOLUTIONS 100 // How many top solutions to select
#define POPULATION 600   // Number of solutions per generation
#define LEAVE 5
#define STABLE 800
int  n;

vector <vector<double> >distances;
vector<pair<double, double> > points;
vector <int> path;
vector <int> parent1;
vector <int> parent2;
vector <int> visited1;
vector <int> visited2;
vector <int> child1;
vector <int> child2;
vector <int> points1;
vector <int> points2;

string found_earlier;

class Solution
{
    private:
        vector<int>points;
        double sum;
    public:
        Solution(vector<int>points, double sum)
        {
            this->points=points;
            this->sum=sum;
        }
        Solution(string path)
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
        void differentSolution(vector<int>points, double sum)
        {
            this->points=points;
            this->sum=sum;
        }
        void mutate()
        {
            int a=rand()%n;
            int b=rand()%n;
            if(b<a)swap(a,b);
            random_shuffle(this->points.begin()+a, this->points.begin()+b);
        }
        vector<int> getPath()
        {
            return this->points;
        }
        void display()
        {
            cout<<sum<<endl;
        }
        void CalcLength()
        {
            this->sum=0;
            for(int i=0; i<n-1; i++)
            {
                this->sum+=distances[this->points[i]-1][this->points[i+1]-1];
            }
            this->sum+=distances[this->points[n-1]-1][this->points[0]-1];
        }
        bool operator<(const Solution& a) const
        {
            return sum<a.sum;
        }
        bool operator==(const Solution& a) const
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
};

vector<Solution> solutions;
vector<Solution> copied_best;
queue<Solution> best_of_generation;
vector<Solution> extinct;

vector<pair<double, double> >readInput(int n)
{
    vector<pair<double, double> >vertices;
    double number,a,b;
    for(int i=0; i<n; i++) //reading the input
    {
        cin>>number>>a>>b;
        vertices.push_back(make_pair(a,b));
    }
    return vertices;
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
    for(int k=0; k<4000; k++)
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
    for(int k=0; k<4000; k++)
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