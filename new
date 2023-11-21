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
int DURATION = 9;
int K = 7;
chrono::duration<long long, nano> TIME_MAX_NS;

vector<string> splitStringBySpaces(const string& s);
void solution(string format, string fileName, int cutoff);
vector<vector<vector<int>>> readFile(string format, string fileName, int cutoff);
vector<vector<vector<int>>> readFormatO(fstream& file, int cutoff);
vector<vector<vector<int>>> readFormatT(fstream& file, int cutoff);
void parseData(vector<vector<vector<int>>>& data);
vector<int> grasp(vector<vector<vector<int>>> data);
vector<int> choseCandidate(vector<vector<vector<int>>>& data, vector<int> availabilityMachines, vector<int> availabilityTasks);
vector<vector<int>> runGrasp(vector<vector<vector<int>>>& data);
void repr(vector<vector<int>> solution);
void updateSolution(vector<vector<int>>& solution, vector<int>& availabilityMachines, vector<int>& availabilityTasks, vector<int>& candidate);
int checkCutoff(int cutoff);
vector<vector<int>> buildSolution(vector<vector<vector<int>>> data, vector<int> queue);
int evalFenotype(vector<vector<vector<int>>> data, vector<int> queue);
vector<int> repair(vector<int>& chromosome, int tasks, int jobs);
void mutate(vector<int>& genotype);
pair<vector<int>, vector<int>> crossover(vector<int> mother, vector<int> father);
void GA(vector<vector<vector<int>>> data);
void _GA(vector<vector<vector<int>>> data, vector<vector<int>>& population, vector<vector<int>>& hallOfFame, int reproduction, int mutationChance, int maxElitar);
bool compMakeSpan(pair<vector<int>, int>& a, pair<vector<int>, int>& b);
int fiit(int best, int fenotype);
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


int main(int argc, char** argv) //format [O | T] file [path] truncate [int] k[int] time(s)[int]
{
    int duration, k;
    srand(time(NULL));
    if (argc == 4) {
        k = K;
        duration = DURATION;
    }
    else if (argc == 6) {
        k = stoi(argv[4]);
        duration = stoi(argv[5]);
    }
    else {
        printf("format [O | T] file [path] truncate [int] k [int] time(s)[int+]\n");
        return(0);
    }

    long long int cutoff = strtoll(argv[3], NULL, 10);
    string fileName = argv[2];
    string format = argv[1];


    TIME_MAX_NS = std::chrono::seconds{ duration };


    if (cutoff >= -1 && !fileName.empty() && (format == "O" || format == "T") && k >= 1)
    {
        K = k;
        solution(format, fileName, cutoff);

    }
    else
    {
        cout << "Invalid arguments" << endl;
        exit(1);
    }

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

vector<int> grasp(vector<vector<vector<int>>> data) {
    vector<vector<int>> solution(CUTOFF, vector<int>(MACHINES, 0));
    vector<int> availabilityMachines(MACHINES, 0);
    vector<int> availabilityTasks(CUTOFF, 0);
    vector<int> queue;

    while (true) {
        vector<int> candidate = choseCandidate(data, availabilityMachines, availabilityTasks);
        if (candidate == vector<int>(1, -1)) {
            break;
        }
        queue.push_back(candidate[2]);
        updateSolution(solution, availabilityMachines, availabilityTasks, candidate);
    }

    int time = max(*max_element(availabilityMachines.begin(), availabilityMachines.end()),
        *max_element(availabilityTasks.begin(), availabilityTasks.end()));
    vector<int> temp;
    temp.push_back(time);
    solution.push_back(temp);
    return queue;
}

vector<int> choseCandidate(vector<vector<vector<int>>>& data, vector<int> availabilityMachines, vector<int> availabilityTasks) {
    vector<int> whitelist;
    vector<vector<int>> candidates;
    int erased = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i].size() > 0) {
            auto temp = data[i][data[i].size() - 1];
            int currentTime = max(*max_element(availabilityMachines.begin(), availabilityMachines.end()),
                *max_element(availabilityTasks.begin(), availabilityTasks.end()));
            int additionalTime = max(availabilityMachines[temp[0]], availabilityTasks[i]) + temp[1] - currentTime;//time
            additionalTime = max(0, additionalTime);
            temp.push_back(additionalTime);
            candidates.push_back(temp);
        }

    }

    if (candidates.size() == 0) {
        return vector<int>(1, -1);
    }

    sort(candidates.begin(), candidates.end(), compAddTime);

    int choice = rand() % min(K, (int)candidates.size());


    auto chosen = candidates[choice];
    chosen.pop_back();
    data[chosen[2]].pop_back();

    return chosen;
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

int fiit(int best, int fenotype) {
    return best - fenotype;
}

vector<int> roulette(int n, vector<pair<vector<int>, int>> population) {
    vector<int> choosen;
    long long int sum = 0;
    for (auto& i : population) {
        sum += i.second;
    }

    for (int i = 0; i < n; i++) {
        long long int temp = 0;
        int random = rand() % sum;
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

vector<vector<int>> initPopulation(vector<vector<vector<int>>> data, int size) {
    vector<vector<int>> population;
    for (int i = 0; i < size; i++) {
        population.push_back(grasp(data));
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

pair<vector<int>, vector<int>> crossover(vector<int> mother, vector<int> father) {
    pair<vector<int>, vector<int>> childs;
    vector<int> son, daughter;
    int cut = rand() % mother.size();
    for (int i = 0; i < cut; i++) {
        daughter.push_back(mother[i]);
        son.push_back(father[i]);
    }
    for (int i = cut; i < mother.size(); i++) {
        daughter.push_back(father[i]);
        son.push_back(mother[i]);
    }
    childs.first = son;
    childs.second = daughter;
    return childs;
}

void GA(vector<vector<vector<int>>> data) {
    int SIZE = 100;
    int REPRODUCTION = 50;
    double MUTATIONCHANCE = .03;
    int MAXELITAR = 0;
    auto population = initPopulation(data, SIZE);
    vector<vector<int>> hallOfFame;
    for (int i = 0; i <= 500; i++) {
        _GA(data, population, hallOfFame, REPRODUCTION, MUTATIONCHANCE, MAXELITAR);
        printf("%d \n", evalFenotype(data, hallOfFame[i]));
    }
    repr(buildSolution(data, hallOfFame[500]));


}

bool compMakeSpan(pair<vector<int>, int>& a, pair<vector<int>, int>& b) {
    return a.second < b.second;
}

void _GA(vector<vector<vector<int>>> data, vector<vector<int>>& population, vector<vector<int>>& hallOfFame, int reproduction, int mutationChance, int maxElitar) {
    vector<pair<vector<int>, int>> evaluation;
    vector<vector<int>> toReproduction;

    for (auto& genotype : population) {
        auto eval = pair<vector<int>, int>(genotype, evalFenotype(data, genotype));
        evaluation.push_back(eval);
    }
    //include best of alltime
    maxElitar = min(maxElitar, (int)hallOfFame.size());
    for (int i = 0; i < maxElitar; i++) {
        toReproduction.push_back(hallOfFame[hallOfFame.size() - 1 - i]);
    }
    //pick best from this gen and save
    auto elitar = *min_element(evaluation.begin(), evaluation.end(), compMakeSpan);
    hallOfFame.push_back(elitar.first);

    auto worst = *max_element(evaluation.begin(), evaluation.end(), compMakeSpan);
    for (auto& genotype : evaluation) {
        genotype.second = fiit(5000, genotype.second);
    }

    auto indexs = roulette(reproduction, evaluation);
    for (int& i : indexs) {
        toReproduction.push_back(evaluation[i].first);
    }
    auto noise = grasp(data);
    shuffle(noise.begin(), noise.end(), default_random_engine(time(0)));
    toReproduction.push_back(noise);
    //reproduce crossover mutate
    for (auto& i : population) {
        int g1, g2;
        g1 = rand() % toReproduction.size();
        g2 = rand() % toReproduction.size();
        auto childs = crossover(toReproduction[g1], toReproduction[g2]);
        auto nextGen = childs.second;
        if (rand() % 2 == 0) {
            auto nextGen = childs.first;
            repair(nextGen, CUTOFF, MACHINES);
            for (int i = 0; i < nextGen.size(); i++)
                if (((rand() % 1001) / 1000) < mutationChance) {
                    mutate(nextGen);
                }
            i = nextGen;
        }
    }
}
