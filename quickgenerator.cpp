//Generates 10 random points - only for testing

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

int main()
{
    srand(time(0));

    ofstream zapis;
    zapis.open("test.txt");

    int n=100;
    zapis<<n<<endl;
    for(int i=1; i<=n; i++)
    {
        zapis<<i<<" ";
        zapis<<rand()%100000+0<<" ";
        zapis<<rand()%100000+0<<endl;
    }
}

