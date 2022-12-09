#include <iostream>
#include<vector>
#include<cstdlib>
#include<random>
using namespace std;

#define PI 3.14159265358979323846
#define M 10

double a, b; //margini
int precision = 5; //precizie
unsigned int D; //dimensiuni
int ld; //lungime

double deJong(vector<double>& v)
{
	double result = 0;
	for (int i = 0; i < v.size(); i++)
	{
		result += (v[i] * v[i]);
	}
	return result;
}

double schwefel(vector<double>& v)
{
	double result = 0;
	for (int i = 0; i < v.size(); i++)
	{
		result += (v[i] * sin(sqrt(abs(v[i]))) * -1);
	}
	return result;
}

double rastrigin(vector<double>& v)
{
	double result = 10 * v.size();
	for (int i = 0; i < v.size(); i++)
	{
		result += ((v[i] * v[i]) - (10 * cos(2 * PI * v[i])));
	}
	return result;
}

double michalewicz(vector<double>& v)
{
	double result = 0;
	for (int i = 1; i <= v.size(); i++)
	{
		result += (sin(v[i - 1]) * pow(sin((i * v[i - 1] * v[i - 1]) / PI), M * 2));
	}
	result *= -1;
	return result;
}

int length(double a, double b, int precision)
{
	return ceil(log((b - a) * pow(10, precision)) / log(2));
}

vector<double> decode(vector<char>& bits, int ld, unsigned int D, double a, double b)
{
	vector<double> vcs;
	int start, end;
	for (int i = 0; i < D; ++i) {
		start = i * ld;
		end = (i + 1) * ld;
		unsigned long bi = 0;
		for (auto i = start; i != end; ++i)
		{
			bi *= 2;
			bi += bits[i];
		}
		double x = bi / (pow(2, ld) - 1);
		x *= (b - a);
		x += a;
		vcs.push_back(x);
	}
	return vcs;
}

void hillClimbingBest(unsigned int D, int ld, double a, double b, double (*func)(vector<double>&))
{
	int t = 0;
	clock_t start, end;
	start = clock();
	srand((unsigned int)(start));
	int L = ld * D;
	double best = INT_MAX;
	vector<char>bits;
	while (t < 500)
	{
		bool local = false;
		bits.clear();
		for (int i = 0; i < L; i++)
			bits.push_back(rand() % 2);
		vector<double> d = decode(bits, ld, D, a, b);
		double vc = func(d);
		int bestbit = 0;
		do {
			double initial = vc;
			for (int i = 0; i < L; ++i)
			{
				bits[i] = !bits[i];
				d = decode(bits, ld, D, a, b);
				double vn = func(d);
				if (vn < vc)
				{
					vc = vn;
					bestbit = i;
				}
				bits[i] = !bits[i];
			}
			if (vc == initial)
			{
				local = true;
			}
			else
			{
				bits[bestbit] = !bits[bestbit];
			}
		} while (local == false);
		if (vc < best)
			best = vc;
		t++;
	}
	cout << "\nMinimul global= " << best;
	end = clock();
	cout << "\nExecutia pentru dimensiunea " << D << " folosind HCB a durat: " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) << " secunde";
}

void hillClimbingFirst(unsigned int D, int ld, double a, double b, double (*func)(vector<double>&))
{
	int t = 0;
	clock_t start, end;
	start = clock();
	srand((unsigned int)(start));
	int L = ld * D;
	double best = INT_MAX;
	vector<char>bits;
	while (t < 500)
	{
		bool local = false;
		bits.clear();
		for (int i = 0; i < L; i++)
			bits.push_back(rand() % 2);
		vector<double> d = decode(bits, ld, D, a, b);
		double vc = func(d);
		do {
			double initial = vc;
			for (int i = 0; i < L; ++i)
			{
				bits[i] = !bits[i];
				d = decode(bits, ld, D, a, b);
				double vn = func(d);
				if (vn < vc)
				{
					vc = vn;
					break;
				}
				bits[i] = !bits[i];
			}
			if (vc == initial) {
				local = true;
			}
		} while (local == false);
		if (vc < best)
			best = vc;
		t++;
	}
	cout << "\nMinimul global= " << best;
	end = clock();
	cout << "\nExecutia pentru dimensiunea " << D << " folosind HCF a durat: " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) <<" secunde";
}

void simulatedAnnealing(unsigned int D, int ld, double a, double b, double (*func)(vector<double>&)) 
{
	clock_t start, end;
	start = clock();
	srand((unsigned int)(start));
	int L = ld * D;
	double best = INT_MAX;
	vector<char> bits;
	bits.clear();
	for (int i = 0; i < L; i++)
		bits.push_back(rand() % 2);
	vector<double> d = decode(bits, ld, D, a, b);
	double vc = func(d);
	double temperature = 100;
	while (temperature > 0.000000001)
	{
		int t = 0;
		while (t < 500)
		{
			int random = rand() % L;
			bits[random] = !bits[random];
			d = decode(bits, ld, D, a, b);
			double vn = func(d);
			if (vn < vc) 
			{
				vc = vn;
			}
			else
				if ((double)rand() / (RAND_MAX + 1) < exp(-1 * abs(vn - vc) / temperature)) 
				{
					vc = vn;
				}
				else 
				{
					bits[random] = !bits[random];
				}
			t++;
			if (vc < best)
				best = vc;
		}
		temperature *= 0.99; 

	}
	cout << "\nMinimul global= " << best;
	end = clock();
	cout << "\nExecutia pentru dimensiunea " << D << " folosind SA a durat: " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) << " secunde";
}

void solve(double a, double b, int precision, double (*func)(vector<double>&), const char* function) 
{

	ld = length(a, b, precision);
	clock_t start, end;
	cout << "\nIncepe analiza functiei " << function << "\n";

	//Hill-Climbing Best Improvement

	start = clock();
	cout << "Incepe hill-climbing best\n";
	cout << "\nIncepe executia pentru dimensiunea 5\n";
	for (int i = 0; i < 30; ++i)
	{
		hillClimbingBest(5, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 5\n";
	}
	cout << "\nIncepe executia pentru dimensiunea 10\n";
	for (int i = 0; i < 30; ++i) 
	{
		hillClimbingBest(10, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " for size 10\n";
	}
	cout << "Incepe executia pentru dimensiunea 30\n";
	for (int i = 0; i < 30; ++i) 
	{
		hillClimbingBest(30, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " for size 30\n";
	}
	end = clock();
	cout << function << " with HCB took " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) << " seconds.\n";
	
	//Hill-Climbing First Improvement

	start = clock();
	cout << "\nIncepe Hill-Climbing First\n";
	cout << "\nIncepe executia pentru dimensiunea 5\n";
	for (int i = 0; i < 30; ++i) 
	{
		hillClimbingFirst(5, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 5\n";
	}
	cout << "\nIncepe executia pentru dimensiunea 10\n";
	for (int i = 0; i < 30; ++i) 
	{ 
		hillClimbingFirst(10, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 10\n";
	}
	cout << "\nIncepe executia pentru dimensiunea 30\n";
	for (int i = 0; i < 30; ++i) 
	{ 
		hillClimbingFirst(30, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 30\n";
	}
	end = clock();
	cout << function << " with hcf took " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) << " seconds.\n";
	
	//Simulated Annealing

	start = clock();
	cout << "Incepe simulated-annealing\n";
	cout << "Incepe executia pentru dimensiunea 5\n";
	for (int i = 0; i < 30; ++i) 
	{ 
		simulatedAnnealing(5, ld, a, b, func);
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 5\n";
	}
	cout << "\nIncepe executia pentru dimensiunea 10\n";
	for (int i = 0; i < 30; ++i) 
	{ 
		simulatedAnnealing(10, ld, a, b, func);		
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 10\n";
	}
	cout << "\nIncepe executia pentru dimensiunea 30\n";
	for (int i = 0; i < 30; ++i) 
	{ 
		simulatedAnnealing(30, ld, a, b, func);		
		cout << "\nS-a terminat iteratia " << i << " pentru dimensiunea 30\n";
	}
	end = clock();
	cout <<"\n"<< function << " with SA took " << (double)((double)(end - start) / (double)(CLOCKS_PER_SEC)) << " seconds.\n";

}

int main()
{
	//DEJONG
	solve(-5.12, 5.12, 5, deJong, "DeJong");
	//SCHWEFEL
	solve(-500, 500, 5, schwefel, "Schwefel");
	//Rastrigin
	solve(-5.12, 5.12, 5, rastrigin, "Rastrigin");
	//Michalewicz
	solve(0, PI, 5, michalewicz, "Michalewicz");
	return 0;
}



