# Graph-Mining-Performance-Study
Repository for the Graph Mining Performance Study.
This project is under the Aggie Research Program at Texas A&M, under Dr. Khanh Nguyen.

TLDR; It has been observed that different permutation of vertex ids on isomorphic patterns can result in different performance in term of execution time and memory consumption for graph mining systems. We investigate the proliferation and severity of this problem.

## Contents
1. [Isomorpher](#isomorpher)
2. [Bash Scripts](#bash-scripts)
3. [GraphPi edits](#graphpi-edits)


# Isomorpher
The file *isomorpher.cpp* is a C++ program that allows you enter an input graph in the [Peregrine](https://github.com/pdclab/peregrine) format and it will exhuastively generate all V! isomorphic variants of the pattern (where V is the number of vertices). It writes each variant in a seperate output file in the current directory and can addtionally generate LaTeX source code that displays each of them. 

The program accepts two file inputs, one named **pattern.graph** that contains each of the edges of the source pattern. An example of this is shown below for a "house" patern: 
```
1 2
1 4
1 5
2 3
2 5
3 4
```

The other input is only neccesary for the purposes of the LaTeX display. It should be named **pattern.txt** and contain the absolute coordinates of the vertices in the graph, where 0 0 is the top center position. Regardless of the order of the edges in .graph file, the .txt must have each coordinate on the line number respective to which vertex it corresponds to. An example of this for the "house" pattern: 
```
-0.75 -1
0.75 -1
0.75 -2
-0.75 -2
0 0
```

To run the program, use 
```console
g++ isomorpher.cpp
./a.out
```
Since it is exhuastively generating all patterns, be wary of patterns with large amounts of vertices.

## Deprecated files
*altergraph.c* is a prototype that allowed manipulation of graphs with the ability to reverse all edges, flip one edge, increment all labels, and swap two vertices. The use for manual alteration was superseded by the ecxhuastive generation, so this file is deprecated.  

*tranformLaTeX.py* is another now redundant program whose functionality has been carried over to the main program. It has the ability to copy a given LaTeX source file to match another graph rather than the one it originally represented.


# Bash scripts

There are four bash scripts in this repository. These bash scripts monitor the usage of processes running on UNIX systems. 

## memusage.sh
The memory usage script monitors all the cores of the CPU and dumps the results in a text file. It also supports running the given process multiple times and averaging the results.
To edit the output file and the amount of runs that the script does, edit the lines 26 and 27:
```
outfile="memprofile.txt"                                                   
runs=2                                                                     
```
To run the script, use the format
```console
memusage COMMAND [ARGS]
```

## testsuite.sh
The test suite script takes in a folder of different graphs, and gets the timing and memory profiles of each graph. The output is stored in testsuiteresults.txt.

To run the script, use the format
```console
testsuite PATTERN_DIR COMMAND [ARGS]
```
where the PATTERN_DIR is directory that stores the patterns, and COMMAND is the graph mining command *minus* the pattern. Adding the patterin into the COMMAND will cause the program to fail.

# GraphPi edits

This is a copy of the open source [GraphPi](https://github.com/thu-pacman/GraphPi) project with modifications to allow for the input files from Peregrine to be used in its constructor. Visit the original repository for further information. 


