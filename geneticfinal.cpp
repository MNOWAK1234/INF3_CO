#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

#define TOP_SOLUTIONS 5000 // How many top solutions to select
#define POPULATION 5000   // Number of solutions per generation
#define STABLE 500 // Number of iterations between extinctions
#define ALMOST_STABLE 40
#define ITERATIONS1 5000
#define ITERATIONS2 1000

int  n;

vector <vector<double> >distances;
vector <pair<double, double> > points;
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
        Solution(string path) //do wywalenia - tylko testy
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
        void mutate1()
        {
            int a=rand()%n;
            int b=rand()%n;
            swap(points[a],points[b]);
        }
        void mutate2()
        {
            int a=rand()%n;
            int b=rand()%n;
            if(b<a)swap(a,b);
            vector <int> tmp;
            for(int i=a; i<=b; i++) tmp.push_back(points[i]);
            int j=0;
            for(int i=b; i>=a; i--)
            {
                points[i]=tmp[j];
                j++;
            }
        }
        vector<int> getPath()
        {
            return this->points;
        }
        void display()
        {
            cout<<sum<<endl;
        }
        double getsum()
        {
            return this->sum;
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
queue<Solution> best_of_generation;
queue<Solution> eldest;
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
void mutation()
{
    for(int i=TOP_SOLUTIONS*0.7; i<TOP_SOLUTIONS; i++)
    {
        solutions[i].mutate1();
    }
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
    for(int i=0; i<TOP_SOLUTIONS; i++)
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
        points1=solutions[rand()%TOP_SOLUTIONS].getPath();
        points2=solutions[rand()%TOP_SOLUTIONS].getPath();
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
        crossed1.CalcLength();
        crossed2.CalcLength();
        solutions.push_back(crossed1);
        solutions.push_back(crossed2);
    }
}
void massExtinction()
{
    vector <int> tmp;
    Solution help(tmp,0);

    best_of_generation.push(solutions[0]);
    if(eldest.size()<ALMOST_STABLE)
    {
        help=best_of_generation.front();
        best_of_generation.pop();
        eldest.push(help);
    }
    if(best_of_generation.size()==STABLE)
    {
        if(abs(solutions[0].getsum()-best_of_generation.front().getsum())<1000)
        {
            mutation();
        }
        if(abs(solutions[0].getsum()-eldest.front().getsum())<40)
        {
            extinct.push_back(solutions[0]);
            solutions.clear();
            random();
            best_of_generation=queue<Solution>();//clears the queue
        }
        else
        {
            eldest.pop();
            help=best_of_generation.front();
            best_of_generation.pop();
            eldest.push(help);
        }
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
    
    for(int i=1; i<=n; i++)path.push_back(i);
    random();
    prepareCross();
    //solutions.push_back(findGreedy(points));
    sort(solutions.begin(),solutions.end());

    for(int k=0; k<=ITERATIONS1; k++)
    {
        if(k%200==0)
        {
            cout<<k<<endl;
            solutions[0].display();
        }
        orderCross();
        //mutation();
        sort(solutions.begin(),solutions.end());
        solutions.erase(solutions.begin()+POPULATION,solutions.end());
        massExtinction();
    }
    cout<<endl;
    cout<<"Extinct:"<<endl; //adding best results to population
    for(int i=0; i<(int)extinct.size(); i++)
    {
        extinct[i].display();
        solutions.push_back(extinct[i]);
    }
    
    //solutions.push_back(findGreedy(points));
    sort(solutions.begin(),solutions.end());
    solutions.erase(solutions.begin()+POPULATION,solutions.end());
    
    for(int k=0; k<ITERATIONS2; k++)
    {
        if(k%100==0)
        {
            cout<<k<<endl;
            solutions[0].display();
        }
        orderCross();
        sort(solutions.begin(),solutions.end());
        mutation();
        solutions.erase(solutions.begin()+POPULATION,solutions.end());
    }

    cout<<endl;
    cout<<"Final result"<<endl;
    solutions[0].display();
    vector <int> result = solutions[0].getPath();
    for (int i=0; i<result.size(); i++) cout<<result[i]<<" - ";
}