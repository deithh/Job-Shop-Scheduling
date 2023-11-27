#include <iostream>
#include <cstring>
#include <utility>
#include <cstdlib>
#include <cstddef>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <unordered_map>
#define INF 2147483647 

using namespace std;

int MACHINES = -1;
int JOBS = -1;
int CUTOFF = -1;

int DURATION = 180;
bool DEBUG = 0;
bool LOG = 0;
char NAME[100];

int SIZE = 100;
int REPRODUCTION = 30;
double MUTATIONCHANCE = .02;
int MAXELITAR = 3;
int CHILDS = 12;
int POSSIBLEMUTATIONS = 1;

chrono::duration<long long, nano> TIME_MAX_NS;

vector<string> splitStringBySpaces(const string& s);
void solution(string format, string fileName, int cutoff);
vector<vector<vector<int>>> readFile(string format, string fileName, int cutoff);
vector<vector<vector<int>>> readFormatO(fstream& file, int cutoff);
vector<vector<vector<int>>> readFormatT(fstream& file, int cutoff);
void parseData(vector<vector<vector<int>>>& data);
vector<int> initSubject(vector<vector<vector<int>>> data);
vector<vector<int>> runGrasp(vector<vector<vector<int>>>& data);
void repr(vector<vector<int>> solution);
void updateSolution(vector<vector<int>>& solution, vector<int>& availabilityMachines, vector<int>& availabilityTasks, vector<int>& candidate);
int checkCutoff(int cutoff);
vector<vector<int>> buildSolution(vector<vector<vector<int>>> data, vector<int> queue);
int evalFenotype(vector<vector<vector<int>>> data, vector<int> queue);
vector<int> repair(vector<int>& chromosome, int tasks, int jobs);
void mutate(vector<int>& genotype);
vector<int> crossover(vector<int> mother, vector<int> father);
void GA(vector<vector<vector<int>>> data);
void _GA(vector<vector<vector<int>>> data, vector<pair<vector<int>, int>>& population, vector<pair<vector<int>, int>>& hallOfFame);
bool compMakeSpan(pair<vector<int>, int>& a, pair<vector<int>, int>& b);
vector<int> roulette(int n, vector<pair<vector<int>, int>> population);


vector<string> splitStringBySpaces(const string& s) {
    vector<string> tokens;
    istringstream ss(s);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool compAddTime(vector<int>& a, vector<int>& b) {
    return a[4] < b[4];
}

bool compMakeSpan(pair<vector<int>, int>& a, pair<vector<int>, int>& b) {
    return a.second < b.second;
}

int main(int argc, char** argv) //format [O | T] file [path] truncate [int] k[int] time(s)[int]
{
    int duration;
    srand(time(NULL));
    strcpy(NAME, argv[2]);
    if (argc == 4) {
        duration = DURATION;
    }
    else if (argc == 6) {
        duration = stoi(argv[4]);
        LOG = stoi(argv[5]);
    }
    else if (argc == 12){
        duration = stoi(argv[4]);
        LOG = stoi(argv[5]);
        SIZE = stoi(argv[6]);
        REPRODUCTION = stoi(argv[7]);
        MUTATIONCHANCE = stod(argv[8]);
        MAXELITAR = stoi(argv[9]);
        CHILDS = stoi(argv[10]);
        POSSIBLEMUTATIONS = stoi(argv[11]);
    }
    else {
        printf("format [O | T] file [path] truncate [int]\n");
        exit(0);
    }

    long long int cutoff = strtoll(argv[3], NULL, 10);
    string fileName = argv[2];
    string format = argv[1];


    TIME_MAX_NS = std::chrono::seconds{duration};


    if (!(cutoff >= -1 && !fileName.empty() && (format == "O" || format == "T")))
    {
        cout << "Invalid arguments" << endl;
        exit(1);
    }

    solution(format, fileName, cutoff);

}

void repr(vector<vector<int>> solution) {
    int solutionTime = solution[solution.size() - 1][0];
    printf("%d\n", solutionTime);
    for (int i = 0; i < solution.size() - 1; i++) {
        for (auto j : solution[i]) {
            printf("%d ", j);
        }
        printf("\n");
    }
}

void solution(string format, string fileName, int cutoff) {

    vector<vector<vector<int>>> tasks = readFile(format, std::move(fileName), cutoff);
    parseData(tasks);
    GA(tasks);

}

int checkCutoff(int cutoff) {
    if (cutoff > JOBS) {
        cout << "Cutoff is greater than number of jobs" << endl;
        exit(1);
    }
    if (cutoff == -1) {
        cutoff = JOBS;
    }
    CUTOFF = cutoff;
    return cutoff;
}

vector<vector<vector<int>>> readFile(string format, string fileName, int cutoff) {
    vector<vector<vector<int>>> tasks;
    fstream file;
    file.open(fileName, ios::in);

    if (!file) {
        cout << "File not found" << endl;
        exit(1);
    }

    if (format == "O") {
        tasks = readFormatO(file, cutoff);
    }
    if (format == "T") {
        tasks = readFormatT(file, cutoff);
    }
    file.close();
    return tasks;
};

vector<vector<vector<int>>> readFormatO(fstream& file, int cutoff) {
    vector<vector<vector<int>>> tasks;
    vector<vector<int>> jobs;
    vector<int> pair;

    int iter = 0;
    string line;
    std::vector<std::string> temp;

    getline(file, line);
    temp = splitStringBySpaces(line);
    JOBS = stoi(temp[0]);
    MACHINES = stoi(temp[1]);

    cutoff = checkCutoff(cutoff);

    while (getline(file, line) && (iter < cutoff)) {

        iter++;
        jobs.clear();
        temp = splitStringBySpaces(line);
        for (int i = 0; i < MACHINES; i++) {
            pair.clear();
            auto push = stoi(temp[i * 2]);
            pair.push_back(push);
            push = stoi(temp[i * 2 + 1]);
            pair.push_back(stoi(temp[i * 2 + 1]));
            jobs.push_back(pair);
        }
        tasks.push_back(jobs);
    }

    return tasks;
}

vector<vector<vector<int>>> readFormatT(fstream& file, int cutoff) {

    // Task (machine, time), (machine, time)
    vector<vector<vector<int>>> tasks;
    vector<vector<int>> jobs;
    vector<int> pair;

    string line;
    std::vector<std::string> temp;

    getline(file, line);

    temp = splitStringBySpaces(line);
    JOBS = stoi(temp[0]);

    MACHINES = stoi(temp[1]);

    cutoff = checkCutoff(cutoff);

    getline(file, line); //skip line

    for (int i = 0; i < cutoff; i++) {
        jobs.clear();
        getline(file, line);
        temp = splitStringBySpaces(line);
        for (int j = 1; j <= MACHINES; j++) {
            // 0 cos, 1 time
            pair.clear();
            pair.push_back(0);
            pair.push_back(stoi(temp[j - 1]));
            jobs.push_back(pair);

        }
        tasks.push_back(jobs);
    }
    for (int i = 0; i < JOBS - cutoff; i++) {
        getline(file, line);
    }

    getline(file, line);//skip line

    for (int i = 0; i < cutoff; i++) {

        getline(file, line);
        temp = splitStringBySpaces(line);
        for (int j = 1; j <= MACHINES; j++) {
            tasks[i][j - 1][0] = stoi(temp[j - 1]) - 1;
        }

    }

    return tasks;
}

void parseData(vector<vector<vector<int>>>& data) {
    int taskIter = 0;
    for (auto& tasks : data) {
        int iter = 0;
        for (auto& job : tasks) {
            job.push_back(taskIter);
            job.push_back(iter);
            iter++;
        }
        taskIter++;
        reverse(tasks.begin(), tasks.end());
    }
}

vector<int> initSubject(vector<vector<vector<int>>> data) {
    vector<int> queue;

    for (int i = 0; i < CUTOFF; i++) {
        for (int j = 0; j < MACHINES; j++)
            queue.push_back(i);
    }
    shuffle(queue.begin(), queue.end(), default_random_engine(time(0)));
    return queue;
}

void updateSolution(vector<vector<int>>& solution, vector<int>& availabilityMachines, vector<int>& availabilityTasks, vector<int>& candidate) {
    int machine = candidate[0];
    int time = candidate[1];
    int taskId = candidate[2];
    int priority = candidate[3];
    int startTime = max(availabilityMachines[machine], availabilityTasks[taskId]);
    solution[taskId][priority] = startTime;
    availabilityMachines[machine] = time + startTime;
    availabilityTasks[taskId] = time + startTime;
}

vector<vector<int>> buildSolution(vector<vector<vector<int>>> data, vector<int> queue) {
    //self expl name
    vector<vector<int>> solution(CUTOFF, vector<int>(MACHINES, 0));
    vector<int> availabilityMachines(MACHINES, 0);
    vector<int> availabilityTasks(CUTOFF, 0);
    for (auto& current : queue) {
        auto job = data[current][data[current].size() - 1];
        data[current].pop_back();
        updateSolution(solution, availabilityMachines, availabilityTasks, job);
    }
    int time = max(*max_element(availabilityMachines.begin(), availabilityMachines.end()),
        *max_element(availabilityTasks.begin(), availabilityTasks.end()));
    vector<int> temp;
    temp.push_back(time);
    solution.push_back(temp);
    return solution;
}

int evalFenotype(vector<vector<vector<int>>> data, vector<int> queue) {
    //return makespan of chromosome
    auto solution = buildSolution(data, queue);
    return solution[solution.size() - 1][0];
}

vector<int> roulette(int n, vector<pair<vector<int>, int>> population) {
    vector<int> choosen;
    double sum = 0;
    for (auto& i : population) {
        sum += 1.0 / i.second;
    }

    for (int i = 0; i < n; i++) {
        double temp = 0;
        double random = rand() / RAND_MAX * sum;
        for (int j = 0; j < population.size(); j++) {
            temp += population[j].second;
            if (random < temp) {
                choosen.push_back(j);
                break;
            }
        }

    }
    return choosen;
}

vector<pair<vector<int>, int>> initPopulation(vector<vector<vector<int>>> data, int size) {
    vector<pair<vector<int>, int>> population;
    for (int i = 0; i < size; i++) {
        pair<vector<int>, int> One;
        One.first = initSubject(data);
        One.second = evalFenotype(data, One.first);
        population.push_back(One);
    }
    return population;
}

vector<int> repair(vector<int>& genotype, int tasks, int jobs) {
    unordered_map<int, int> counter;

    for (int gen : genotype) {
        counter[gen]++;
    }


    //mark elements to change as -1 
    for (const auto& pair : counter) {
        int task = pair.first;
        int jobsCount = pair.second;

        if (jobsCount > jobs) {
            int overflow = jobsCount - jobs;
            for (int i = 0; i < overflow; i++) {
                auto it = find(genotype.begin(), genotype.end(), task);
                *it = -1;
            }
        }
    }
    //add lacking elements
    for (int i = 0; i < tasks; i++) {
        int lack = jobs - counter[i];

        if (lack > 0) {
            for (int j = 0; j < lack; j++) {
                auto it = find(genotype.begin(), genotype.end(), -1);
                *it = i;
            }
        }
    }
    return genotype;

}

void mutate(vector<int>& genotype) {
    int position = rand() % genotype.size();
    int nextPosition;
    while ((nextPosition = rand() % genotype.size()) == position);
    iter_swap(genotype.begin() + position, genotype.begin() + nextPosition);
}

vector<int> crossover(vector<int> mother, vector<int> father) {
    vector<int> s1;
    int cut1 = rand() % mother.size();
    int cut2 = rand() % mother.size();
    if (cut2 < cut1) {
        swap(cut1, cut2);
    }
    for (int i = 0; i < cut1; i++) {
        s1.push_back(mother[i]);
    }
    for (int i = cut1; i < cut2; i++) {
        s1.push_back(father[i]);
    }
    for (int i = cut2; i < mother.size(); i++) {
        s1.push_back(mother[i]);
    }
    return s1;
}

void GA(vector<vector<vector<int>>> data) {

    vector<pair<vector<int>, int>> population = initPopulation(data, SIZE);
    vector<pair<vector<int>, int>> hallOfFame;

    const auto start = chrono::high_resolution_clock::now();
    int iterations = 0;

    while(1){
        iterations++;
        _GA(data, population, hallOfFame);

        auto int_ns = chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
        if(LOG){
            //time;tasks;machines;worktime;name;iteration
            printf("%d;", hallOfFame[0].second);
            printf("%d;", CUTOFF);
            printf("%d;", MACHINES);
            printf("%f;", chrono::duration_cast<chrono::milliseconds>(int_ns).count()/1000.0);
            printf("%s;", NAME);
            printf("%d\n", iterations);
        }

        if (int_ns > TIME_MAX_NS) {
            break;
        }
    }
    if(!LOG)
        repr(buildSolution(data, hallOfFame[0].first));
}

void _GA(vector<vector<vector<int>>> data, vector<pair<vector<int>, int>>& population, vector<pair<vector<int>, int>>& hallOfFame) {
    vector<vector<int>> toReproduction;

    auto indexs = roulette(REPRODUCTION, population);
    for (int& i : indexs) {
        toReproduction.push_back(population[i].first);
    }
    for (auto i : hallOfFame) {
        toReproduction.push_back(i.first);
    }

    for (int i = 0; i < CHILDS; i++) {
        int g1, g2;
        g1 = rand() % toReproduction.size();
        g2 = rand() % toReproduction.size();
        auto nextGen = crossover(toReproduction[g1], toReproduction[g2]);
        repair(nextGen, CUTOFF, MACHINES);
        pair<vector<int>, int> evaluated(nextGen, evalFenotype(data, nextGen));
        population.push_back(evaluated);
    }

    for (auto& evaluation : population) {
        auto& genotype = evaluation.first;
        auto& fenotype = evaluation.second;

        for (int i = 0; i < POSSIBLEMUTATIONS; i++) {
            if ((rand() / RAND_MAX) < MUTATIONCHANCE) {
                mutate(genotype);
            }
            fenotype = evalFenotype(data, genotype);
        }
    }

    sort(population.begin(), population.end(), compMakeSpan);

    int toRemove = population.size() - SIZE;
    for (int i = 0; i < toRemove; i++) {
        population.pop_back();
    }

    auto elitar = *min_element(population.begin(), population.end(), compMakeSpan);
    hallOfFame.push_back(elitar);

    sort(hallOfFame.begin(), hallOfFame.end(), compMakeSpan);

    toRemove = hallOfFame.size() - MAXELITAR;
    for (int i = 0; i < toRemove; i++) {
        hallOfFame.pop_back();
    }
}
