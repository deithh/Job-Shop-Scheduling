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
#define INF 2147483647 

using namespace std;

int MACHINES = -1;
int JOBS = -1;
int CUTOFF = -1;
int DURATION = 5;
int K = 5;
chrono::duration<long long, nano> TIME_MAX_NS;

vector<string> splitStringBySpaces(const string& s);
void solution(string format, string fileName, int cutoff);
vector<vector<vector<int>>> readFile(string format, string fileName, int cutoff);
vector<vector<vector<int>>> readFormatO(fstream& file, int cutoff);
vector<vector<vector<int>>> readFormatT(fstream& file, int cutoff);
void parseData(vector<vector<vector<int>>>& data);
vector<vector<int>> grasp(vector<vector<vector<int>>> data);
vector<int> choseCandidate(vector<vector<vector<int>>>& data, vector<int> availabilityMachines, vector<int> availabilityTasks);
vector<vector<int>> runGrasp(vector<vector<vector<int>>>& data);
void repr(vector<vector<int>> solution);
void updateSolution(vector<vector<int>>& solution, vector<int>& availabilityMachines, vector<int>& availabilityTasks, vector<int>& candidate);
int checkCutoff(int cutoff);

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
    else {
        k = stoi(argv[4]);
        duration = stoi(argv[5]);
    }

    long long int cutoff = strtoll(argv[3], NULL, 10);
    string fileName = argv[2];
    string format = argv[1];


    TIME_MAX_NS = std::chrono::seconds{ duration };


    if (cutoff >= -1 && !fileName.empty() && fileName.substr(fileName.length() - 4) == ".txt" && (format == "O" || format == "T") && k >= 1)
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
    auto solution = runGrasp(tasks);
    repr(solution);
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


vector<vector<int>> grasp(vector<vector<vector<int>>> data) {
    vector<vector<int>> solution(CUTOFF, vector<int>(MACHINES, 0));
    vector<int> availabilityMachines(MACHINES, 0);
    vector<int> availabilityTasks(CUTOFF, 0);

    while (true) {
        vector<int> candidate = choseCandidate(data, availabilityMachines, availabilityTasks);
        if (candidate == vector<int>(1, -1)) {
            break;
        }
        updateSolution(solution, availabilityMachines, availabilityTasks, candidate);
    }

    int time = max(*max_element(availabilityMachines.begin(), availabilityMachines.end()),
        *max_element(availabilityTasks.begin(), availabilityTasks.end()));
    vector<int> temp;
    temp.push_back(time);
    solution.push_back(temp);
    return solution;
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

vector<vector<int>> runGrasp(vector<vector<vector<int>>>& data) {
    int bestTime = INF;
    vector<vector<int>> bestSolution;
    vector<vector<int>> currSolution;
    int currTime;

    const auto start = chrono::high_resolution_clock::now();

    while (true) {
        auto int_ns = chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
        if (int_ns > TIME_MAX_NS) {
            break;
        }
        currSolution = grasp(data);
        currTime = currSolution[currSolution.size() - 1][0];
        if (currTime < bestTime) {
            bestSolution = currSolution;
            bestTime = currTime;
        }

    }
    return bestSolution;
}