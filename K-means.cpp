#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int k;
double points[100][3];
int temp;
double tempdist;
double X1,X2,X3,Y1,Y2,Y3;
double dist0,dist1,dist2;
double centerx0, centery0, counter0, centerx1, centery1, counter1, centerx2, centery2, counter2;
double tab[50][50];
double distance(double X1, double Y1, double X2, double Y2)
{
    return sqrt((X1-X2)*(X1-X2)+(Y1-Y2)*(Y1-Y2));
}
void showtable()
{
    for(int i=0; i<100; i++)
    {
        tab[(int)points[i][0]] [(int)points[i][1]]=points[i][2];
    }
    for(int i=0; i<50; i++)
    {
        for(int j=0; j<50; j++)
        {
            if(tab[i][j]==(-1))cout<<"_";
            else cout<<tab[i][j];
        }
        cout<<endl;
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    srand(time(0));
    for(int i=0; i<100; i++)
    {
        temp=rand()%50;
        points[i][0]=(double)temp;
        temp=rand()%50;
        points[i][1]=(double)temp;
        points[i][2]=-1;
    }
    for(int i=0; i<50; i++)
    {
        for(int j=0; j<50; j++)
        {
            tab[i][j]=-1;
        }
    }
    X1=points[0][0];
    Y1=points[0][1];
    points[0][2]=0;
    X2=points[1][0];
    Y2=points[1][1];
    points[1][2]=1;
    X3=points[2][0];
    Y3=points[2][1];
    points[2][2]=2;
    for(int i=3; i<100; i++)
    {
        tempdist=distance(X1, Y1, points[i][0], points[i][1]);
        dist0=tempdist;
        tempdist=distance(X2, Y2, points[i][0], points[i][1]);
        dist1=tempdist;
        tempdist=distance(X3, Y3, points[i][0], points[i][1]);
        dist2=tempdist;
        if(min(dist0,min(dist1,dist2))==dist0)points[i][2]=0;
        if(min(dist0,min(dist1,dist2))==dist1)points[i][2]=1;
        if(min(dist0,min(dist1,dist2))==dist2)points[i][2]=2;
    }
    centerx0=0;
    centery0=0;
    counter0=0;
    centerx1=0;
    centery1=0;
    counter1=0;
    centerx2=0;
    centery2=0;
    counter2=0;
    //showtable();
    for(int i=0; i<100; i++)
    {
        if(points[i][2]==0)
        {
            centerx0+=points[i][0];
            centery0+=points[i][1];
            counter0++;
        }
        if(points[i][2]==1)
        {
            centerx1+=points[i][0];
            centery1+=points[i][1];
            counter1++;
        }
        if(points[i][2]==2)
        {
            centerx2+=points[i][0];
            centery2+=points[i][1];
            counter2++;
        }
    }
    centerx0/=counter0;
    centery0/=counter0;
    centerx1/=counter0;
    centery1/=counter0;
    centerx2/=counter0;
    centery2/=counter0;
    X1=centerx0;
    Y1=centery0;
    X2=centerx1;
    Y2=centery1;
    X3=centerx2;
    Y3=centery2;

    for(int j=0; j<1000; j++)//powinienem tu dac while(tablica sie zmienia), ale dla malych tablic 1000 powtorzen wystarczy
    {
    for(int i=0; i<100; i++)
    {
        tempdist=distance(X1, Y1, points[i][0], points[i][1]);
        dist0=tempdist;
        tempdist=distance(X2, Y2, points[i][0], points[i][1]);
        dist1=tempdist;
        tempdist=distance(X3, Y3, points[i][0], points[i][1]);
        dist2=tempdist;
        if(min(dist0,min(dist1,dist2))==dist0)points[i][2]=0;
        if(min(dist0,min(dist1,dist2))==dist1)points[i][2]=1;
        if(min(dist0,min(dist1,dist2))==dist2)points[i][2]=2;
    }
    centerx0=0;
    centery0=0;
    counter0=0;
    centerx1=0;
    centery1=0;
    counter1=0;
    centerx2=0;
    centery2=0;
    counter2=0;
    for(int i=0; i<100; i++)
    {
        if(points[i][2]==0)
        {
            centerx0+=points[i][0];
            centery0+=points[i][1];
            counter0++;
        }
        if(points[i][2]==1)
        {
            centerx1+=points[i][0];
            centery1+=points[i][1];
            counter1++;
        }
        if(points[i][2]==2)
        {
            centerx2+=points[i][0];
            centery2+=points[i][1];
            counter2++;
        }
    }
    centerx0/=counter0;
    centery0/=counter0;
    centerx1/=counter0;
    centery1/=counter0;
    centerx2/=counter0;
    centery2/=counter0;
    X1=centerx0;
    Y1=centery0;
    X2=centerx1;
    Y2=centery1;
    X3=centerx2;
    Y3=centery2;
    }
    showtable();
}

