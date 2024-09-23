# Job Shop Scheduling
This project is a C++ application designed to solve optimization problems using a Greedy Randomized Adaptive Search Procedure (GRASP) approach. The program supports different problem formats, such as Beasley and Taillard, and gives users control over various parameters, including the number of tasks to consider, the randomness factor, and the computation time. The core algorithm iteratively improves the solution by combining greedy heuristics with randomization, striving to find high-quality solutions efficiently. The best solution found during execution is then output to the console.

## Problem instances and format description  
https://people.brunel.ac.uk/~mastjjb/jeb/orlib/files/jobshop1.txt  
http://mistic.heig-vd.ch/taillard/problemes.dir/ordonnancement.dir/ordonnancement.html  

## Compilation  
To compile the program, use the following command:  
```
g++ solution.cpp -o main
```

## Usage  
To run the program, use the following syntax:  
```
./main [format] [instance path] [number of tasks to consider] [optional K] [optional t]
```

## Parameters  
format:  
O for Beasley format  
T for Taillard format  

instance path: Relative or absolute path to the .txt file containing the problem instance.  

number of tasks to consider: -1 if the program should read all tasks, or the specific number of tasks to read from the instance.  

Optional Parameters  
(If using optional parameters, both must be provided):  

K: A parameter controlling the level of randomness in the GRASP algorithm, default is set to 5.  
t: A parameter controlling the algorithm's runtime (excluding I/O operations), given in seconds (integers only), default is set to 5.

## Output
The program prints the best solution found to the console (stdout).
