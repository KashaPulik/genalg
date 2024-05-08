#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <vector>

using namespace std;

#define N_CITIES 10

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
                cout << city_a.get_name() << " --> " << city_b.get_name() << " : " << get_city_road(city_a.get_name(), city_b.get_name()) << endl;
            }
    }
};

area local_area;

class individual {
    vector<int> genome;
    int genome_size;

public:
    individual() : genome_size(0)
    {
    }
    individual(vector<int> new_genome, int size) : genome(new_genome), genome_size(size)
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
        int a = rand() % (genome_size - 1) + 1;
        int b = rand() % (genome_size - 1) + 1;
        while (a == b)
            b = rand() % (genome_size - 1) + 1;
        swap(genome[a], genome[b]);
    }
    int size()
    {
        return genome_size;
    }
    int fitness() const
    {
        int length = 0;
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
    for (int i = 0; i < cross_point; i++)
        child_a.add_chromosome(genome_a[i]);
    for (int i = cross_point; i < genome_size; i++)
        child_a.add_chromosome(genome_b[i]);
    if (child_a.size() < genome_size)
        for (int i = cross_point; i < genome_size; i++)
            child_a.add_chromosome(genome_a[i]);

    individual child_b;
    for (int i = 0; i < cross_point; i++)
        child_b.add_chromosome(genome_b[i]);
    for (int i = cross_point; i < genome_size; i++)
        child_b.add_chromosome(genome_a[i]);
    if (child_b.size() < genome_size)
        for (int i = cross_point; i < genome_size; i++)
            child_b.add_chromosome(genome_b[i]);

    if (rand() % 100 < mutation_chance) {
        child_a.make_mutation();
        child_b.make_mutation();
    }

    population.push_back(child_a);
    population.push_back(child_b);
}

vector<individual> make_initial_population(size_t population_size, vector<int> genome)
{
    individual adam(genome, genome.size());
    individual eva(genome, genome.size());

    eva.make_mutation();

    vector<individual> population = {adam, eva};

    while (population.size() < population_size)
        crossing(population, 100);

    sort(population.begin(), population.end());

    population.erase(population.begin() + population_size, population.end());

    return population;
}

vector<individual> find_shortest_path(int population_size, vector<int> cities_list, int n_iterations)
{
    vector<individual> population = make_initial_population(population_size, cities_list);
    for (int i = 0; i < n_iterations; i++) {
        crossing(population, 100);
        sort(population.begin(), population.end());
        population.erase(population.begin() + population_size, population.end());
    }
    return population;
}

vector<individual> find_shortest_path_with_correction(int population_size, vector<int> cities_list)
{
    vector<individual> population = make_initial_population(population_size, cities_list);
    int n = 0;
    while (population[0].fitness() != population[population.size() - 1].fitness()) {
        n++;
        crossing(population, 100);
        sort(population.begin(), population.end());
        population.erase(population.begin() + population_size, population.end());
    }
    cout << n << endl;
    return population;
}

vector<int> get_right_path(int path_length)
{
    vector<int> path;
    for (int i = 0; i < path_length; i++)
        path.push_back(i);

    for (int i = 0; i < path_length; i++)
        swap(path[rand() % (path_length - 1) + 1], path[rand() % (path_length - 1) + 1]);

    return path;
}

int main(int argc, char* argv[])
{
    // for (int i = 0; i < 5; i++)
    //     local_area.add_city(i);
    // local_area.add_city_road(0, 1, 1);
    // local_area.add_city_road(0, 2, 3);
    // local_area.add_city_road(0, 3, 4);
    // local_area.add_city_road(0, 4, 5);
    // local_area.add_city_road(1, 2, 1);
    // local_area.add_city_road(1, 3, 4);
    // local_area.add_city_road(1, 4, 8);
    // local_area.add_city_road(2, 3, 5);
    // local_area.add_city_road(2, 4, 1);
    // local_area.add_city_road(3, 4, 2);
    // local_area.print();

    // srand(time(0));

    vector<int> right_path = get_right_path(N_CITIES);

    for (int i = 0; i < N_CITIES; i++)
        local_area.add_city(i);

    for (int i = 0; i < (N_CITIES - 1); i++)
        local_area.add_city_road(right_path[i], right_path[i + 1], 1);

    local_area.add_city_road(0, N_CITIES - 1, 1);

    for (int i = 0; i < N_CITIES - 1; i++)
        for (int j = i + 1; j < N_CITIES; j++)
            local_area.add_city_road(i, j, (rand() % 10) + 1);

    vector<int> cities_list;
    for (int i = 0; i < N_CITIES; i++)
        cities_list.push_back(i);
    int population_size = stoi(argv[1]);
    int n_iterations = stoi(argv[2]);
    vector<individual> population;
    double time = -wtime();
    population = find_shortest_path(population_size, cities_list, n_iterations);
    // population = find_shortest_path_with_correction(population_size, cities_list);
    time += wtime();
    cout << time << " seconds\n";
    vector<int> genome;
    genome = population[0].get_genome();
    for (auto& gen : genome) {
        cout << gen << " --> ";
    }
    cout << "0 : ";
    cout << population[0].fitness() << endl;

    // for (auto& individ : population) {
    //     genome = individ.get_genome();
    //     for (auto& gen : genome) {
    //         cout << gen << " --> ";
    //     }
    //     cout << "0 : ";
    //     cout << individ.fitness() << endl;
    // }
}