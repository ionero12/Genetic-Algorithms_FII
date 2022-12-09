#include <iostream>
#include<random>
#include<vector>
#include<math.h>
#include <algorithm>
#include <stdio.h>
#include<ctime>
#include <climits>
using namespace std;
#define PI 3.14159265358979323846
vector <bool> GenerateRandomCandidate(size_t ld){
    vector <bool> x_bit(ld);
    for(int i=0; i<ld; i++)
        x_bit[i]=rand() %2;
    return x_bit;
}
float random_01(int resolution = 10000){
    return rand() % resolution / float(resolution);
}
vector <double> DecodeBitRepr(const vector<bool> x_b, size_t D, size_t ld, double a, double b, double p){
    vector<double> x;
    for(int i = 0; i < D; i++)
    {
        double x_int= 0;
        for(int j = 0; j < ld; j++)
        {
            x_int*=2;
            x_int+=x_b[i+j];
        }
        i += ld - 1;
        x_int=  a + x_int * (b - a) / (pow(2,ld)-1);
        x_int= round(x_int*p) / p;
        x.push_back(x_int);
    }
    return x;
}
double Evaluate(int function, const vector<bool> x_b, size_t D, size_t ld, double a, double b, double p){
    double fun;
    fun=0;
    auto x= DecodeBitRepr(x_b,D,ld,a,b,p);
    switch(function){
        case 1:
            for(double i :x)
                fun=fun+i*i;
            break;
        case 2:
            for(double i : x)
                fun=fun+(-i)*sin(sqrt(abs(i)));
            break;
        case 3:
            fun=fun+10*x.size();
            for(double i : x)

                fun=fun+i*i-10*cos(2*PI*i);
            break;
        case 4:
            for(int i=0;i<x.size(); i++)
                fun =fun+ sin(x[i]) * pow((sin((i+1)*x[i]*x[i] / PI)),20);
            fun*=-1;
            break;
    }
    fun= (round(p*fun)/p);
    return fun;
}

void selection(vector<vector<bool> >& Pop, vector<double> E, int k, int pressure, int popSize)
{
    vector<double> pc;
    double minimum=INT_MAX, maximum=INT_MIN;
    for (auto i = E.begin(); i != E.end(); i++){
        if (*i > maximum)
            maximum = *i;
        if (*i < minimum)
            minimum = *i;
    }
    vector<double> Fitness;
    double fsum = 0;
    for (int i = 0; i < E.size(); i++)
    {
        Fitness.push_back(pow(((maximum - E[i]) / (maximum - minimum + 0.000001) + 0.01), pressure));
        fsum += Fitness[i];
    }
    pc.push_back(Fitness[0] / fsum);
    for (int i = 1; i < Fitness.size(); i++)
    {
        pc.push_back(Fitness[i] / fsum + pc[i - 1]);
    }
    vector<vector<bool> > next_Pop;
    for (int i = 0; i < popSize - k; i++)
    {
        float r = random_01();
        bool c = false;
        for (int j = 0; j < pc.size(); j++)
        {
            if (r <= pc[j])
            {
                next_Pop.push_back(Pop[j]);
                c = true; break;
            }
        }
        if (!c)
            next_Pop.push_back(Pop[Pop.size() - 1]);
    }
    Pop = next_Pop;
}

void mutate(vector<vector<bool> >& Pt){
    float Ran;
    for (auto & i : Pt){
        for(auto && ii : i){
            Ran=random_01();
            if (Ran<=0.01)
                ii = 1-ii;
        }
    }
}
bool compare(pair<int, double> a, pair<int, double> b){
    if(a.second<b.second)
        return true;
    else
        return false;
}
pair <vector<bool>, vector<bool> > cx(const vector<bool>& c1, const vector<bool>& c2)
{
    auto a =c1, b = c2;
    int pos = 1 + rand() % (c1.size() - 3);
    for (int i = pos; i < c1.size(); i++) {
        a[i] = c2[i];
        b[i] = c1[i];
    }
    return make_pair(a, b);
}
void crossover(vector<vector<bool> >& Pt, int& popSize) {
    vector<pair<int, float> > p;
    int i;
    for (i = 0; i < Pt.size(); i++)
        p.push_back(make_pair(i, random_01()));
    sort(p.begin(), p.end(), compare);
    i=0;
    for (i=0;i<p.size();i+=2)
    {
        if (i+1==p.size() || p[i + 1].second>=0.6)//
            break;
        auto x = cx(Pt[p[i].first], Pt[p[i + 1].first]);
        Pt[p[i].first] = x.first;
        Pt[p[i + 1].first] = x.second;
    }
    if (p[i].second<0.6){
        float r;
        r= random_01();
        if (r >= 0.5)
        {
            auto x = cx(Pt[p[i].first], Pt[p[i + 1].first]);
            Pt[p[i].first] = x.first;
            Pt[p[i + 1].first] = x.second;}
    }
}

vector<vector<bool> > elitism(const vector<vector<bool> >& Pt, const vector<double>& E, const int& k)
{
    vector<pair<int, float> > p;
    vector<vector<bool> > el;
    int i;
    for (i=0;i<E.size();i++){
        p.push_back(make_pair(i,E[i]));
    }
    sort(p.begin(), p.end(), compare);
    for (i=0;i<k;i++){
        el.push_back(Pt[p[i].first]);
    }
    return el;
}
void Eval(vector<double>& E, int function, const vector<vector<bool> > Pt, size_t L, size_t ld, double a, double b, int p){
    for (int i = 0; i < Pt.size(); i++){
        E[i] = Evaluate(function, Pt[i], L, ld, a, b, p);
    }
}
double ag(const int& function, int generations, int popSize, const size_t& L, const size_t& ld, const double& a, const double& b, const int& p)
{
    int k = int(popSize * 0.07), pressure = 4, t = 0;
    vector<vector<bool> > Pt;
    double minimum=INT_MAX, maximum=INT_MIN;
    vector<double> E;
    for (int i = 0; i < popSize; i++) {
        Pt.push_back(GenerateRandomCandidate(L));
        E.push_back(Evaluate(function, Pt[i], L, ld, a, b, p));
    }
    while (t < generations) {
        t++;
        auto elit= elitism(Pt, E, k);
        selection(Pt, E, k, pressure, popSize);
        mutate(Pt);
        crossover(Pt, popSize);
        for (auto & i : elit)
        {
            Pt.push_back(i);
        }
        Eval(E, function, Pt, L, ld, a, b, p);
    }
    for (auto i = E.begin(); i != E.end(); i++)
    {
        if (*i > maximum)
            maximum = *i;
        if (*i < minimum)
            minimum = *i;
    }
    return minimum;
}
int main(int argc, char* argv[]) {

    // Pentru apel: ./main Nume_Funtie Dimensiune Precizie Number_of_Generations PopSize
    clock_t start, end;
    start = clock();
    int function;
    string nume;
    int D, px, p, generations, popSize;
    cin >> nume >> D >> px >> generations >> popSize;
    double a, b;
    if (nume=="DeJong") {
        function = 1;
        a = -5.12;
        b = 5.12;
    }
    else if (nume== "Schwefel") {
        function = 2;
        a = -500;
        b = 500;
    }
    else if (nume=="Rastrigin") {
        function = 3;
        a = -5.12;
        b = 5.12;
    }
    else if (nume== "Michalewicz") {
        function = 4;
        a = 0;
        b = PI;
    }
    p = pow(10,px);
    size_t l = ceil(log2(p*(b-a)));
    size_t L = D*l;
    int ii = 0, nr_max_it = 30;
    auto x = ag(function, generations, popSize, L, l, a, b, p);
    double minim = x, average = x;
    while(ii < nr_max_it)
    {
        x = ag(function, generations, popSize, L, l, a, b, p);
        average+=x;
        cout << "Minim valoare actuala = " << x << " pentru iteratia "<< ii<<endl;
        if(minim > x)
            minim = x;
        ii++;
    }
    cout << "Best value: " << minim << "\n" << "Average value: " << average/nr_max_it << "\n";
     end = clock();
    cout << " Elapsed time in seconds: " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) << " secunde";
    return 0;
}
