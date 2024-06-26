#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <utility>
#include <vector>

using namespace std;

enum options {
    n_cities_opt,
    population_size_opt,
    mutation_percent_opt,
    n_iterations_opt
};

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

class city {
    int name;
    map<int, int> roads;

public:
    city(int Name = 0) : name(Name)
    {
    }
    int add_road(int to, int length)
    {
        if (length < 1)
            return -1;
        if (roads.count(to) == 1)
            return -1;
        roads[to] = length;
        return 0;
    }
    int get_road(int to)
    {
        if (roads.count(to) == 0)
            return -1;
        return roads[to];
    }
    void set_name(int new_name)
    {
        name = new_name;
    }
    int get_name()
    {
        return name;
    }
    city& operator=(const city& other)
    {
        this->name = other.name;
        this->roads = other.roads;
        return *this;
    }
};

class area {
    vector<city> cities;

public:
    area()
    {
    }
    int add_city(int new_city_name)
    {
        city new_city(new_city_name);
        for (auto& c : cities) {
            if (c.get_name() == new_city_name)
                return -1;
        }
        cities.push_back(new_city);
        return 0;
    }
    int add_city_road(int a, int b, int length)
    {
        if (a == b || length < 1)
            return -1;
        int a_exist = 0, b_exist = 0;
        int a_city_name = a < b ? a : b;
        int b_city_name = b > a ? b : a;
        city a_city;
        for (auto& c : cities) {
            if (c.get_name() == a_city_name) {
                a_exist = 1;
                a_city = c;
            }
            if (c.get_name() == b_city_name) {
                b_exist = 1;
            }
        }
        if (!a_exist || !b_exist)
            return -1;
        if (a_city.get_road(b_city_name) != -1)
            return -1;
        a_city.add_road(b_city_name, length);
        for (auto& c : cities)
            if (c.get_name() == a_city_name)
                c = a_city;
        return 0;
    }
    int get_city_road(int a, int b)
    {
        if (a == b)
            return -1;
        int a_city_name = a < b ? a : b;
        int b_city_name = b > a ? b : a;
        for (auto& c : cities) {
            if (c.get_name() == a_city_name)
                return c.get_road(b_city_name);
        }
        return -1;
    }
    void print()
    {
        for (auto& city_a : cities)
            for (auto& city_b : cities) {
                if (city_a.get_name() == city_b.get_name())
                    continue;
                cout << city_a.get_name() << " --> " << city_b.get_name()
                     << " : "
                     << get_city_road(city_a.get_name(), city_b.get_name())
                     << endl;
            }
    }
    void clear()
    {
        cities.clear();
    }
    int get_size()
    {
        return cities.size();
    }
};

area local_area;
vector<int> path;
vector<int> cities_list;

class individual {
    vector<int> genome;
    int genome_size;

public:
    individual() : genome_size(0)
    {
    }
    individual(vector<int> new_genome, int size)
        : genome(new_genome), genome_size(size)
    {
    }
    int add_chromosome(int chromosome)
    {
        for (auto& g : genome)
            if (g == chromosome)
                return -1;

        genome.push_back(chromosome);
        genome_size++;
        return 0;
    }
    vector<int> get_genome()
    {
        return genome;
    }
    void make_mutation()
    {
        int rand_limit = genome_size - 1;
        int a = rand() % rand_limit + 1;
        int b = rand() % rand_limit + 1;
        while (a == b)
            b = rand() % rand_limit + 1;
        swap(genome[a], genome[b]);
    }
    int size()
    {
        return genome_size;
    }
    size_t fitness() const
    {
        size_t length = 0;
        int prev = -1;
        int count = 0;
        for (auto& chromosome : genome) {
            count++;
            if (prev == -1) {
                prev = chromosome;
                continue;
            }
            length += local_area.get_city_road(prev, chromosome);
            prev = chromosome;
            if (count == genome_size)
                length += local_area.get_city_road(0, chromosome);
        }
        return length;
    }
};

bool operator<(const individual& a, const individual& b)
{
    return a.fitness() < b.fitness();
}

void crossing(vector<individual>& population, int mutation_chance)
{
    int population_size = population.size();

    int a_number = rand() % population_size;
    int b_number = rand() % population_size;

    while (a_number == b_number)
        b_number = rand() % population_size;

    vector<int> genome_a = population[a_number].get_genome();
    vector<int> genome_b = population[b_number].get_genome();

    int genome_size = genome_a.size();
    int cross_point = rand() % (genome_size - 1) + 1;

    individual child_a;
    individual child_b;
    for (int i = 0; i < cross_point; i++) {
        child_a.add_chromosome(genome_a[i]);
        child_b.add_chromosome(genome_b[i]);
    }
    for (int i = cross_point; i < genome_size; i++) {
        child_a.add_chromosome(genome_b[i]);
        child_b.add_chromosome(genome_a[i]);
    }
    if (child_a.size() < genome_size) {
        for (int i = cross_point; i < genome_size; i++) {
            child_a.add_chromosome(genome_a[i]);
            child_b.add_chromosome(genome_b[i]);
        }
    }

    if (rand() % 100 < mutation_chance) {
        child_a.make_mutation();
        child_b.make_mutation();
    }

    population.push_back(child_a);
    population.push_back(child_b);
}

void selection(vector<individual>& population, size_t population_size)
{
    sort(population.begin(), population.end());
    population.erase(population.begin() + population_size, population.end());
}

vector<individual>
make_initial_population(size_t population_size, vector<int> genome)
{
    individual adam(genome, genome.size());
    individual eva(genome, genome.size());

    eva.make_mutation();

    vector<individual> population = {adam, eva};

    while (population.size() < population_size)
        crossing(population, 100);

    selection(population, population_size);

    return population;
}

vector<individual> find_shortest_path(
        int population_size,
        vector<int> cities_list,
        int n_iterations,
        int mutation_percent)
{
    vector<individual> population
            = make_initial_population(population_size, cities_list);
    for (int i = 0; i < n_iterations; i++) {
        crossing(population, mutation_percent);
        selection(population, population_size);
    }
    return population;
}

int get_right_path(size_t path_len)
{
    if (path_len == path.size())
        return -1;

    path.clear();
    cities_list.clear();

    for (size_t i = 0; i < path_len; i++) {
        path.push_back(i);
        cities_list.push_back(i);
    }
    for (size_t i = 0; i < path_len; i++)
        swap(path[rand() % (path_len - 1) + 1],
             path[rand() % (path_len - 1) + 1]);

    return 0;
}

int make_area(int n_cities)
{
    if (get_right_path(n_cities))
        return -1;

    local_area.clear();

    for (int i = 0; i < n_cities; i++)
        local_area.add_city(i);

    for (int i = 0; i < (n_cities - 1); i++)
        local_area.add_city_road(path[i], path[i + 1], 1);

    local_area.add_city_road(0, n_cities - 1, 1);

    for (int i = 0; i < n_cities - 1; i++)
        for (int j = i + 1; j < n_cities; j++)
            local_area.add_city_road(i, j, (rand() % 10) + 1);

    vector<int> cities_list;
    for (int i = 0; i < n_cities; i++)
        cities_list.push_back(i);

    return 0;
}

pair<double, int> experiment(
        int n_cities,
        int population_size,
        int n_iterations,
        int mutation_percent)
{
    srand(0);

    make_area(n_cities);

    vector<individual> population;
    double time = -wtime();
    population = find_shortest_path(
            population_size, cities_list, n_iterations, mutation_percent);
    time += wtime();
    int result_fitness = population[0].fitness();
    return make_pair(time, result_fitness);
}

void make_experiment(
        int n_cities,
        int population_size,
        int mutation_percent,
        int n_iterations,
        int step,
        options option,
        string filename)
{
    pair<double, int> result;
    ofstream file(filename);
    cout << filename << ":\n";
    cout << left << setw(20) << "n_cities" << setw(20) << "population_size"
         << setw(20) << "mutation_percent" << setw(20) << "n_iterations"
         << setw(20) << "time" << setw(20) << "path_len" << setw(20)
         << "accuracy" << endl;
    file << left << setw(20) << "n_cities" << setw(20) << "population_size"
         << setw(20) << "mutation_percent" << setw(20) << "n_iterations"
         << setw(20) << "time" << setw(20) << "path_len" << setw(20)
         << "accuracy" << endl;
    bool experiment_done = false;
    int end_value;

    switch (option) {
    case n_cities_opt:
        end_value = n_cities;
        n_cities = step;
        break;
    case population_size_opt:
        end_value = population_size;
        population_size = step;
        break;
    case mutation_percent_opt:
        end_value = mutation_percent;
        mutation_percent = 0;
        break;
    case n_iterations_opt:
        end_value = n_iterations;
        n_iterations = step;
        break;
    default:
        return;
    }

    while (!experiment_done) {
        result = experiment(
                n_cities, population_size, n_iterations, mutation_percent);
        file << left << setw(20) << n_cities << setw(20) << population_size
             << setw(20) << mutation_percent << setw(20) << n_iterations
             << setw(20) << result.first << setw(20) << result.second
             << setw(20) << ((double)n_cities / (double)result.second) * 100
             << endl;
        cout << left << setw(20) << n_cities << setw(20) << population_size
             << setw(20) << mutation_percent << setw(20) << n_iterations
             << setw(20) << result.first << setw(20) << result.second
             << setw(20) << ((double)n_cities / (double)result.second) * 100
             << endl;
        switch (option) {
        case n_cities_opt:
            n_cities += step;
            if (n_cities > end_value)
                experiment_done = true;
            break;
        case population_size_opt:
            population_size += step;
            if (population_size > end_value)
                experiment_done = true;
            break;
        case mutation_percent_opt:
            mutation_percent += step;
            if (mutation_percent > end_value)
                experiment_done = true;
            break;
        case n_iterations_opt:
            n_iterations += step;
            if (n_iterations > end_value)
                experiment_done = true;
            break;
        }
    }
    file.close();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;
    if (strcmp(argv[1], "1") == 0) {
        make_experiment(
                100,
                4,
                100,
                10000,
                10,
                n_cities_opt,
                "/home/pahansan/genalg/data/n_cities_exp.csv");
        make_experiment(
                100,
                100,
                100,
                10000,
                5,
                population_size_opt,
                "/home/pahansan/genalg/data/population_size_exp.csv");
        make_experiment(
                100,
                4,
                100,
                10000,
                1,
                mutation_percent_opt,
                "/home/pahansan/genalg/data/mutation_percent_exp.csv");
        make_experiment(
                100,
                4,
                100,
                50000,
                1000,
                n_iterations_opt,
                "/home/pahansan/genalg/data/n_iterations_exp.csv");
    }
    if (strcmp(argv[1], "2") == 0) {
        make_area(20);
        vector<individual> population
                = find_shortest_path(4, cities_list, 1000, 100);
        srand(0);
        int i = 0;
        while (population[0].fitness() != cities_list.size()) {
            population = find_shortest_path(4, cities_list, 1000, 100);
            i++;
        }
        cout << "fitness = " << population[0].fitness() << endl;
        cout << i << endl;
    }
    return 0;
}