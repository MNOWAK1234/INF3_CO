#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>

using namespace std;

#define CROSSES 2500
#define POPULATION 5000
#define BEST 5000
#define STABLE 200
#define CLOSE 1600
#define NEIGHBOURS 3
#define ITERATIONS1 4001
#define ITERATIONS2 1001

int  n;

vector <vector<double> >distances;
vector <pair<double, double> > coordinates;
vector <int> path;
vector <int> visited1;
vector <int> visited2;
vector <int> child1;
vector <int> child2;
vector <int> mst;
vector <vector<int> >tree;

int cnt=0;

void show_matrix(vector<vector<double> >v)
{
    for(int i=0; i<(int)v.size(); i++)
    {
        cout<<i<<": "<<endl;
        for(int j=0; j<(int)v[i].size(); j++)
        {
            cout<<v[i][j]<<"\t";
        }
        cout<<endl;
    }
}


class Solution
{
    public:
        vector<int>points;
        double sum;
        Solution()
        {
            this->points=vector<int>();
            this->sum=0;
        }
        //default constructor is necessary for resize to work
        Solution(vector<int>points, double sum)
        {
            this->points=points;
            this->sum=sum;
        }
        void display()
        {
            cout<<sum<<endl;
            /*for(int i=0; i<n; i++)
            {
                cout<<this->points[i]<<"\t"<<coordinates[this->points[i]-1].first<<"\t"<<coordinates[this->points[i]-1].second<<endl;
            }
            cout<<endl;*/
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
    double distance;
    for(int i=0; i<(int)points.size(); i++)
    {
        one_vertex.clear();
        for(int j=0; j<(int)points.size(); j++)
        {
            distance=dist(points[i].first,points[i].second,points[j].first,points[j].second);
            one_vertex.push_back(distance);
        }
        distances.push_back(one_vertex);
    }
    return distances;
}
void random()
{
    vector<int>better;
    for(int i = 0; i < POPULATION; i++)
    {
        random_shuffle(path.begin(), path.end());//tasuje wierzcholki
        Solution randomized(path,0);
        randomized.CalcLength();
        solutions.push_back(randomized);
    }
}

void prepareCross()
{
    for(int j=0; j<=n; j++)
    {
        visited1.push_back(0);
        visited2.push_back(0);
    }
}
void mutation(int which)
{
    int option;
    int a,b;
    vector<int> change(0);
    option=rand()%100;
    if(option<25)
    {
        a=rand()%n;
        b=rand()%n;
        swap(solutions[which].points[a],solutions[which].points[b]);
    }
    else if(option<50)
    {
        a=rand()%n;
        b=rand()%n;
        if(b<a)swap(a,b);
        reverse(solutions[which].points.begin()+a, solutions[which].points.begin()+b);
    }
    else if(option<75)
    {
        a=rand()%n;
        b=rand()%n;
        change.push_back(solutions[which].points[a]);
        solutions[which].points.erase(solutions[which].points.begin()+a);
        solutions[which].points.insert(solutions[which].points.begin()+b, change[0]);
    }
    else if(option<100)
    {
        a=rand()%n;
        b=rand()%n;
        if(b<a)swap(a,b);
        for(int i=a; i<b; i++)change.push_back(solutions[which].points[i]);
        solutions[which].points.erase(solutions[which].points.begin()+a, solutions[which].points.begin()+b);
        a=rand()%solutions[which].points.size();
        solutions[which].points.insert(solutions[which].points.begin()+a, change.begin(), change.end());
    }
}

void orderCross()
{
    for(int i=0; i<CROSSES; i++)
    {
        int parent1=rand()%BEST;
        int parent2=rand()%POPULATION;
        child1.clear();
        child2.clear();
        for(int j=0; j<=n; j++)
        {
            visited1[j]=0;
            visited2[j]=0;
        }
        int a=rand()%n;
        int b=rand()%n;
        //int b=(a+rand()%20)%n;
        int index=a;
        while(index!=b)
        {
            visited1[solutions[parent1].points[index]]=1;
            visited2[solutions[parent2].points[index]]=1;
            index=(index+1)%n;
        }
        visited1[solutions[parent1].points[index]]=1;
        visited2[solutions[parent2].points[index]]=1;
        if(a<=b)
        {
            index=0;
            while(child1.size()<a)
            {
                if(visited1[solutions[parent2].points[index]]==0)
                {
                    child1.push_back(solutions[parent2].points[index]);
                }
                index++;
            }
            for(int j=a; j<=b; j++)
            {
                child1.push_back(solutions[parent1].points[j]);
            }
            for(int j=index; j<n; j++)
            {
                if(visited1[solutions[parent2].points[j]]==0)
                {
                    child1.push_back(solutions[parent2].points[j]);
                }
            }
            index=0;
            while(child2.size()<a)
            {
                if(visited2[solutions[parent1].points[index]]==0)
                {
                    child2.push_back(solutions[parent1].points[index]);
                }
                index++;
            }
            for(int j=a; j<=b; j++)
            {
                child2.push_back(solutions[parent2].points[j]);
            }
            for(int j=index; j<n; j++)
            {
                if(visited2[solutions[parent1].points[j]]==0)
                {
                    child2.push_back(solutions[parent1].points[j]);
                }
            }
        }
        else
        {
            for(int j=0; j<=b; j++)
            {
                child1.push_back(solutions[parent1].points[j]);
                child2.push_back(solutions[parent2].points[j]);
            }
            for(int j=0; j<n; j++)
            {
                if(visited1[solutions[parent2].points[j]]==0)
                {
                    child1.push_back(solutions[parent2].points[j]);
                }
                if(visited2[solutions[parent1].points[j]]==0)
                {
                    child2.push_back(solutions[parent1].points[j]);
                }
            }
            for(int j=a; j<n; j++)
            {
                child1.push_back(solutions[parent1].points[j]);
                child2.push_back(solutions[parent2].points[j]);
            }
        }
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
    if(best_of_generation.size()==STABLE)
    {
        if(abs(solutions[0].sum-best_of_generation.front().sum)<40)
        {
            extinct.push_back(solutions[0]);
            solutions.clear();
            random();
            best_of_generation=queue<Solution>();//clears the queue
        }
        else
        {
            best_of_generation.pop();
        }
    }
}

Solution findGreedy()
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
    solutions.push_back(res);
}

void DFS(int curr)
{
    for(int i=0; i<tree[curr].size(); i++)
    {
        if(visited1[tree[curr][i]]==0)
        {
            visited1[tree[curr][i]]=1;
            mst.push_back(tree[curr][i]+1);
            DFS(tree[curr][i]);
        }
    }
}

void MST()
{
    vector<bool>visited;
    vector<int>tmp;
    double sum=0;
    for(int i=0; i<n; i++)
    {
        visited.push_back(false);
        tree.push_back(tmp);
    }
    int current=0;
    priority_queue<pair<double,pair<int,int> >, vector<pair<double,pair<int,int> > >, greater<pair<double,pair<int,int> > > >pq;
    for(int i=1; i<n; i++)
    {
        visited[current]=true;
        for(int j=0; j<n; j++)
        {
            if(visited[j]==false)pq.push(make_pair(distances[current][j],make_pair(current,j)));
        }
        while(visited[pq.top().second.second]==true)pq.pop();
        current=pq.top().second.second;
        tree[pq.top().second.first].push_back(pq.top().second.second);
        tree[pq.top().second.second].push_back(pq.top().second.first);
        pq.pop();
    }
    for(int i=0; i<n; i++)visited1[i]=0;
    visited1[0]=1;
    mst.push_back(1);
    DFS(0);
    for(int i=1; i<n; i++)sum+=distances[mst[i]-1][mst[i-1]-1];
    sum+=distances[mst[0]][mst[n-1]];
    Solution res(mst,sum);
    solutions.push_back(res);
}

void LK()
{
    ;
}

struct triple{
    int vertex;
    int trial;
    double gain;
};
/*void shittyLinKernighan(Solution curr)
{
    int best_cost = curr.sum;
    while(true) {
        // find the longest edge in the current solution
        // reverse the order of the cities that it connects
        // repeat reversing sub-tour until no improvement
        int new_cost = calculateCost(current_solution, distances);
        if (new_cost >= best_cost) {
            break;
        }
        best_cost = new_cost;
    }
}*/


int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    srand(time(0));
    cin>>n;
    coordinates=readInput(n);
    distances=lss_points(coordinates); //adjacency matrix
    for(int i=1; i<=n; i++)
    {
        path.push_back(i);
    }
    random();
    prepareCross();
    //findGreedy();
    MST();
    sort(solutions.begin(),solutions.end());
    bool added=false;
    for(int k=0; k<ITERATIONS1; k++)
    {
        //if(k%50==0)
        {
            cout<<k<<endl;
            solutions[0].display();
            //solutions[100].display();
        }
        int cnt=0;
        for(int i=1; i<solutions.size(); i++)
        {
            if(solutions[i-1].sum==solutions[i].sum)
            {
                mutation(i);
                cnt++;
            }
        }
        cout<<"tyle mutuje "<<cnt<<endl;
        orderCross();
        if(solutions[0].sum>200000 && added==false)
        {
            //findGreedy();
            cout<<"added greedy"<<endl;
            added=true;
        }
        sort(solutions.begin(),solutions.end());
        solutions.erase(solutions.begin()+POPULATION,solutions.end());
        //solutions.resize(POPULATION);
        massExtinction();
    }
    cout<<"Extinct:"<<endl; //adding best results to population
    for(int i=0; i<(int)extinct.size(); i++)
    {
        extinct[i].display();
        for(int j=0; j<50; j++)solutions.push_back(extinct[i]);
    }
    sort(solutions.begin(),solutions.end());
    solutions.erase(solutions.begin()+POPULATION,solutions.end());

    for(int k=0; k<ITERATIONS2; k++)
    {
        if(k%50==0)
        {
            cout<<k<<endl;
            solutions[0].display();
            //solutions[100].display();
        }
        int cnt=0;
        for(int i=1; i<solutions.size(); i++)
        {
            if(solutions[i-1].sum==solutions[i].sum)
            {
                mutation(i);
                cnt++;
            }
        }
        //cout<<"tyle mutuje "<<cnt<<endl;
        orderCross();
        sort(solutions.begin(),solutions.end());
        solutions.erase(solutions.begin()+POPULATION,solutions.end());
        //solutions.resize(POPULATION);
    }
}
