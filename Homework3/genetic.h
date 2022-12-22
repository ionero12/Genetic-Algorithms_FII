#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <iterator>
#include <cmath>
#include <chrono>
#include <utility>
#include <string>
#include <queue>
#include <functional>
#include <ostream>
using namespace std;
#include "fh.h"
#include "file.h"
#include "hc.h"

struct elitismCandidate
{
	double value;
	int	position;
	elitismCandidate() { }
	elitismCandidate(double v, int p)
	{
		value = v;
		position = p;
	}
	friend bool operator>(const elitismCandidate& c1, const elitismCandidate& c2);
};

class geneticAlgorithm
{
private:
	static int r_count;
	const tspMap& current_map;
	unsigned int standard_pop_size;
	int number_of_gens;
	int	limit;
	int	optimal_gen;
	vector<int> optimal_tour;
	vector<int> mold;
	vector<vector<int>> pop;
	double cross_prob, mutation_chance, elitism_rate;

public:
	geneticAlgorithm(const tspMap& map) : current_map(map)
	{
		++r_count;
		standard_pop_size = 150;
		number_of_gens = 1500;
		limit = 350;
		elitism_rate = 0.10;
		cross_prob = 0.35;
		mutation_chance = 0.015;
		optimal_gen = 1;
	}

	void runGeneticAlgorithm(int runs);
	void hypermutation(int& count);
	void crossover();
	void evalGen(vector<double>& fit);
	void selection();
	void chromosome_generator(vector<int>& val);
	void set_mold();
	void init_pop();
	void Genetic_Algorith();
	vector<int>	decodeElem(const vector<int>& x);
	double calc_f_value();
	unsigned int select1(const vector<double>& fs);
	unsigned int det_optimal_chromosome();
};


