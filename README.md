# Memory-efficient DAG Sheduling Challenge

We have jobs to run with dependencies. (i.e. It is a directed acyclic graph.)
The peak memory could vary by the order of execution. Since finding the optimal solution is NP-hard, let's find **a decent solution in polynomial time**. You can refer to any papers or algorithms.

- Only one job can be executed at a time (No parallel execution)
- The evaluator handles memory management
    - Allocates memory when it is executed
    - Frees memory as soon as no other jobs are dependent
- A node that is not used by any other nodes are never be freed (Condsidered as an output node)
- Your solution must be in topological or the evaluator will emit an error

## Does that really matter?

For example, we have a graph below.

```
0(100MB) --> 1(50MB) --> 3(150MB) --> 5(10MB)
         \                         /
          -> 2(20MB) --> 4(40MB) --
```

Run it with order `0 1 2 3 4 5`,

```
ACTION         | CURRENT | PEAK
Alloc 0(100MB) |   100MB | 100MB
Alloc 1( 50MB) |   150MB | 150MB
Alloc 2( 20MB) |   170MB | 170MB
Free  0(100MB) |    70MB | 170MB
Alloc 3(150MB) |   220MB | 220MB
Free  1( 50MB) |   170MB | 220MB
Alloc 4( 40MB) |   210MB | 220MB
Free  2( 20MB) |   190MB | 220MB
Alloc 5( 10MB) |   200MB | 220MB
Free  3(150MB) |    50MB | 220MB
Free  4( 40MB) |    10MB | 220MB
```

Run it with order `0 1 3 2 4 5`, you can see the difference.

```
ACTION         | CURRENT | PEAK
Alloc 0(100MB) |   100MB | 100MB
Alloc 1( 50MB) |   150MB | 150MB
Alloc 3(150MB) |   300MB | 300MB
Free  1( 50MB) |   250MB | 300MB
Alloc 2( 20MB) |   270MB | 300MB
Free  0(100MB) |   170MB | 300MB
Alloc 4( 40MB) |   210MB | 300MB
Free  2( 20MB) |   190MB | 300MB
Alloc 5( 10MB) |   200MB | 300MB
Free  3(150MB) |    50MB | 300MB
Free  4( 40MB) |    10MB | 300MB
```

## How to run

### All-in-one script

This script runs configure, build and test. For doing the each step, please take a look at the content of the script.

```bash
./all-in-one.sh
```

```bash
# OUTPUT
tc/tc1.txt           # TC file path
36                   # Peak memory
0 1 2 3 4 5 6 7 8 9  # Execution order
tc/tc2.txt
220
0 1 2 3 4 5
```

### Run TCs individually

```bash
build/exec < tc/tc1.txt
```

```bash
# OUTPUT
36
0 1 2 3 4 5 6 7 8 9
```

## Participating Challenge

You can edit `solve.cc` file only. Please implement `solve()` function which is called from the runner. Please see `common.h` for more about `solve()` function.

## Detailed Description

Please note that the number(index) of nodes are 0-based. Say we have 5 nodes, the number of nodes will be assigned from 0 to 4.

### Input Format

#### Input parameter for `solve()`

You do not need to care much about input file format since they are pre-processed by the main function and you get a vector of nodes as the parameter for `solve()` function. If you want to create your own TCs then please take a look at the below description.

#### Input file format

In the first line two integers are given. The number of nodes(`N`) and edges(`M`), respectively.
The next line `N` integers are given which are required memory for the result of the job.
And the `M` lines will follow. Each line has two integers and the second integer is dependent on the first one. Say the line is `0 5`, it means that `0` must be done in order to run `5`.

### Output Format

#### Output format of `solve()` return value

You must return a vector of node numbers.

#### Output file format

The evaluator will automatically validate the solution and calculate the peak memory. It prints the peak memory at the first line then the execution order at the next line.
