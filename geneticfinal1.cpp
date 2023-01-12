#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

#define CROSSES 1000
#define POPULATION 5000
#define BEST 5000
#define STABLE 200
#define ALMOST_STABLE 40
#define CLOSE 800
#define NEIGHBOURS 4

int  n;

vector <vector<double> >distances;
vector <pair<double, double> > coordinates;
vector <int> path;
vector <int> visited1;
vector <int> visited2;
vector <int> child1;
vector <int> child2;
vector <int> densepointsid;
vector <vector <int> > V;

string found_earlier;
int parent1,parent2;

class Solution
{
    public:
        vector<int>points;
        double sum;

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
        void mutate()
        {
            int a=rand()%n;
            int b=rand()%n;
            if(b<a)swap(a,b);
            random_shuffle(this->points.begin()+a, this->points.begin()+b);
        }
        void display()
        {
            cout<<sum<<endl;
            for(int i=0; i<n; i++)
            {
                if(i%10==0)cout<<this->points[i]<<"\tx: "<<coordinates[this->points[i]-1].first<<"\ty: "<<coordinates[this->points[i]-1].second<<endl;
            }
            //cout<<points[0]<<endl;
            cout<<endl;
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
    double distance;
    int cnt;
    for(int i=0; i<(int)points.size(); i++)
    {
        one_vertex.clear();
        cnt=0;
        for(int j=0; j<(int)points.size(); j++)
        {
            distance=dist(points[i].first,points[i].second,points[j].first,points[j].second);
            one_vertex.push_back(distance);
            if(distance<=CLOSE && i!=j)
            {
                V[i].push_back(j);
                cnt++;
            }
        }
        if(cnt>=NEIGHBOURS)
        {
            densepointsid.push_back(i);
        }
        distances.push_back(one_vertex);
    }
    return distances;
}
void random()
{
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
void mutation()
{
    for(int i=(0.7)*POPULATION; i<POPULATION; i++)
    {
        solutions[i].mutate();
    }
}

void orderCross()
{
    for(int i=0; i<CROSSES; i++)
    {
        parent1=rand()%BEST;
        parent2=rand()%POPULATION;
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
    if(eldest.size()<ALMOST_STABLE)
    {
        help=best_of_generation.front();
        best_of_generation.pop();
        eldest.push(help);
    }
    if(best_of_generation.size()==STABLE)
    {
        if(abs(solutions[0].getsum()-best_of_generation.front().getsum())<40)
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
    coordinates=readInput(n);

    distances=lss_points(coordinates); //adjacency matrix
    for(int i=1; i<=n; i++)path.push_back(i);
    random();
    prepareCross();
    sort(solutions.begin(),solutions.end());

    for(int k=0; k<4001; k++)
    {
        if(k%100==0)
        {
            cout<<k<<endl;
            solutions[0].display();
        }
        orderCross();
        sort(solutions.begin(),solutions.end());
        solutions.erase(solutions.begin()+POPULATION,solutions.end());
        massExtinction();
    }

    cout<<"Extinct:"<<endl; //adding best results to population
    for(int i=0; i<(int)extinct.size(); i++)
    {
        extinct[i].display();
        solutions.push_back(extinct[i]);
    }
    //solutions.push_back(findGreedy(points));
    sort(solutions.begin(),solutions.end());
    solutions.erase(solutions.begin()+POPULATION,solutions.end());

    for(int k=0; k<1001; k++)
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
}
