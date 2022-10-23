//Generates 10 random points - only for testing

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    srand(time(0));
    int n=10;
    for(int i=0; i<n; i++)
    {
        cout<<rand()%100+0<<" ";
        cout<<rand()%100+0<<endl;
    }
}

