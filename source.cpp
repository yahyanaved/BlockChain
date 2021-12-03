#include<iostream>
#include<thread>
#include<string>
#include<fstream>
#include<ctime>
#include<chrono>
#include"json.hpp"
#pragma warning(disable:4996)
using json = nlohmann::json;
using namespace std;
class miscellaneous
{
public:
    string NowToString()
    {
        chrono::system_clock::time_point p = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(p);
        char str[26];
        ctime_s(str, sizeof str, &t);
        return str;
    }
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
}misc;
class puzzle
{
public:
    friend class miscellaneous;
    friend class mining;
    int miner;
    int answer;
    int var;
    puzzle()
    {
        miner = -1;
        answer = -1;
        var = 0;
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
    void p1(jar*obj, long noofjars, int a)
    {
        long sum = 0;
        for (long i = 0; i < noofjars;i++)
        {
            if(miner != -1)
               {
                   break;
                }
            sum = sum + (obj[i].pillsweight * (i + 1));
        }
        long actualsum = 0;
        for (long i = 0;i < noofjars;i++)
        {
            if(miner!=-1)
                break;
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
    jar* pill(long& noofjars)
    {
        noofjars = misc.randoms(100000, 150000);
        long nonce = misc.randoms(0, noofjars - 1);
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
    void verify(jar* obj)
    {
        if (obj[answer - 1].pillsweight == 9)
        {
            var++;
        }
    }
    void puzzlesolver(int noofminers)
    {
        long noofjars = 0;
        jar* obj = pill(noofjars);
        int x = misc.randoms(1, 4);
        thread* temp = new thread[noofminers];
        for (int i = 0;i < noofminers;i++)
        {
            temp[i] = std::thread(&puzzle::p1, this,obj, noofjars, i + 1);
        }
        for (int i = 0;i < noofminers; i++)
        {
            temp[i].join();
        }
    }
}puzz;
class mining
{
public:
    friend class puzzle;
    friend class miscellaneous;
    int noofminers;
    mining(int x)
    {
        noofminers = x;
    }
    void takeinput(json& obj)
    {
        fstream fileptr;int count = 0;
        fileptr.open("input.json", ios::in);
        string time;
        if (fileptr.is_open())
        {
            while (!fileptr.eof())
            {
                time = misc.NowToString();
                fileptr >> obj;
                obj["Time"] = time;
                datapool(obj);
                if (fileptr.peek() == EOF)
                {
                    break;
                }
            }
        }
        fileptr.close();
    }
    void datapool(json& obj)
    {
        puzz.puzzlesolver(noofminers);
        fstream fileptr;
        fstream filereward;
        string filename = "miner";
        string temp = to_string(puzz.miner);
        string temp1 = ".txt";
        filename += temp + temp1;
        if (puzz.var == noofminers - 1)
        {
            fileptr.open(filename, ios::app);
            fileptr << obj << endl;
            fileptr.close();
            filereward.open("rewards.txt", ios::in);

            if (filereward.peek()==EOF)
            {
                filereward.close();
                filereward.open("rewards.txt", ios::app);
                filereward  << puzz.miner;
                filereward.close();
            }
            else
            {
                filereward.close();
                filereward.open("rewards.txt", ios::app);
                filereward <<endl<< puzz.miner;
                filereward.close();
            }
            
        }
        puzz.miner = -1;
        puzz.answer = -1;
        puzz.var = 0;
    }
    void getrewards(int x)
    {
        int temp, count = 0;
        ifstream rewards("rewards.txt");
        if (rewards.is_open())
        {
            while (!rewards.eof())
            {
                rewards >> temp;
                if (temp == x)
                {
                    count++;
                }
            }
        }
        else
        {
            return;
        }
        cout << "REWARDS OF MINER" << x << " : " << count;
    }
};

int main()
{
    json obj;
    mining m(3);
    m.takeinput(obj);
    m.getrewards(1);
}
