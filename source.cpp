#include<iostream>
#include<thread>
#include<string>
#include<fstream>
#include<ctime>
#include<chrono>
#include"json.hpp"
using json = nlohmann::json;
using namespace std;
int miner = -1;
int answer = -1;
int var = 0;
string NowToString()
{
    chrono::system_clock::time_point p = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(p);
    char str[26];
    ctime_s(str, sizeof str, &t);
    return str;
}
class jar
{
public:
    long noofpills;
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
    if (obj[answer-1].pillsweight == 9)
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
    long nonce = randoms(0, noofjars-1);
    jar* jars = new jar[noofjars];
    for (long i = 0; i < noofjars;i++)
    {
        if (i == nonce)
        {
            jars[i].input(150000, 9);
        }
        else
        {
            jars[i].input(150000, 10);
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
    int noofminers = 3;
    puzzle();
    fstream fileptr;
    fstream filereward;
    string filename = "miner";
    string temp = to_string(miner);
    string temp1 = ".txt";
    filename += temp + temp1;
    if (var == noofminers-1)
    {
        fileptr.open(filename, ios::app);
        fileptr << obj << endl;
        fileptr.close();
        filereward.open("rewards.txt", ios::app);
        filereward << miner << endl;
        filereward.close();
    }
    miner = -1;
    answer = -1;
    var = 0;
}
void takeinput(json &obj)
{
    fstream fileptr;int count=0;
    fileptr.open("input.json", ios::in);
    string time;
    if (fileptr.is_open())
    {
        while (!fileptr.eof())
        {
            time = NowToString();
            fileptr >> obj;
            obj["Time"] = time;
            datapool(obj);
            if (fileptr.peek()==EOF)
            {
                break;
            }
        }
    }
    fileptr.close();

}
int main()
{
    json obj;
    takeinput(obj);
}