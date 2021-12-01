#include<iostream>
#include<thread>
#include<fstream>
#include"json.hpp"
using json = nlohmann::json;
using namespace std;
int miner = -1;
int answer = -1;
int var = 0;
class jar
{
public:
    int noofpills;
    int pillsweight;
    jar()
    {

    }
    void input(int a, int b)
    {
        noofpills = a;
        pillsweight = b;
    }
};
int randoms(int min, int max)
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL)); 
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}
void verify(jar * obj)
{
    if (obj[answer].pillsweight == 9)
    {
        var++;
    }
}
void p1(jar* obj, long noofjars, int a)
{
    long sum = 0;
    for (long i = 0; i < noofjars;i++)
    {
        sum = sum + (obj[i].pillsweight * (i + 1));
    }
    long actualsum = 0;
    for (long i = 0;i < noofjars;i++)
    {
        actualsum = actualsum + ((i + 1) * 10);
    }
    long index = actualsum - sum;
    if (miner == -1)
    {
        miner = a;
        answer = index;
    }
    else
    {
        verify(obj);
    }
}
jar* pill(long &noofjars)
{
    noofjars = randoms(100000, 150000);
    long seed = randoms(0, noofjars-1);
    jar* jars = new jar[noofjars];
    for (long i = 0; i < noofjars;i++)
    {
        if (i == seed)
        {
            jars[i].input(100, 9);
        }
        else
        {
            jars[i].input(100, 10);
        }
    }
    return jars;
}
void puzzle()
{
    long noofjars = 0;
    jar* obj = pill(noofjars);
    int x = randoms(1, 4);
    thread t1(p1, obj, noofjars,1);
    thread t2(p1, obj, noofjars,2);
    thread t3(p1, obj, noofjars,3);
    t1.join();
    t2.join();
    t3.join();

}
void datapool(json &obj)
{
    puzzle();
    fstream fileptr;
    if (var == 2)
    {
        switch (miner)
        {
            case 1:
                fileptr.open("miner1.txt", ios::in);
                fileptr << setw(4) << obj << endl;
            case 2:
                fileptr.open("miner2.txt", ios::in);
                fileptr << setw(4) << obj << endl;
            case 3:
                fileptr.open("miner3.txt", ios::in);
                fileptr << setw(4) << obj << endl;
    
        };
    }
    miner = -1;
    answer = -1;
    var = 0;
}
int main()
{
    json obj;
    cout << obj;
}