// Author: Anuj Ketkar
// Aggie Research Program: Graph Mining and Cybersecurity
// Exhuastivly generates all isomorphic patterns to the input graph
// and creates a LaTeX pdf of variants if there is a coordinate file

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <fstream>

// Global variables
int numEdges;
int numVertices;

// Helper functiton to swaps two vertices
void swap(std::vector<int> &a, std::vector<int> &b, int first, int second)
{
    for (int i = 0; i < numEdges; ++i)
    {
        if (a[i] == first) a[i] = second;
        else if (a[i] == second) a[i] = first;
        
        if (b[i] == first) b[i] = second;
        else if (b[i] == second) b[i] = first;
    }
}

// Displays current graph as string
std::string display(std::vector<int> &a, std::vector<int> &b)
{
    std::ostringstream stream;
    for (int i = 0; i < numEdges; ++i)
        stream << a[i] << " " << b[i] << "\n";
    std::string text(stream.str());
    return text;
}

// Exhaustivly calculates all isomorphic graphs recursively
void exhaustive(std::unordered_set<std::string> &permutations, std::vector<int> &a, std::vector<int> &b, int prev, int callStack)
{
    for (int i = 1; i <= numVertices; ++i)
    {
        swap(a, b, prev, i);
        std::string key = display(a, b);
        permutations.insert(key);
        if (callStack < numVertices)
            exhaustive(permutations, a, b, i, callStack + 1);
    }
}

//Appends each entry to the LaTeX code
void latexAppend(std::ofstream& latexWriter, std::vector<int> a, std::vector<int> b, std::vector<std::pair<double, double> > coordinates, std::vector<int> map)
{
    latexWriter << "\t\\begin{tikzpicture}" << std::endl;
    latexWriter << "\t\\tikzstyle{vertex}=[circle,fill=black!25,minimum size=12pt,inner sep=2pt]" << std::endl; 

    for(int i = 0; i < numVertices; ++i)
    {
        std::string xcoord = std::to_string(coordinates[i].first);
        xcoord.erase(xcoord.find_first_of('.') + 3, std::string::npos);
        std::string ycoord = std::to_string(coordinates[i].second);
        ycoord.erase(ycoord.find_first_of('.') + 3, std::string::npos);
        latexWriter << "\t\\node[vertex] (" << (i+1) <<  ") at (" << xcoord  << "," 
        << ycoord << ") {" << map[i+1] << "};" << std::endl;
    }
    for(int j = 0; j < numEdges; ++j)
        latexWriter << "\t\\draw (" << a[j] << ") -- (" << b[j] << ");" << std::endl;
    
    latexWriter << "\t\\end{tikzpicture}" << std::endl;
}

//Finds the alterations between the original and the given input
std::vector<int> mapAlterations(std::string input, int varNum, std::vector<int> &original)
{
    std::ifstream scanner;
    scanner.open(input + std::to_string(varNum) + ".graph");
    std::vector<int> altered;
    
    while(scanner)
    {
        int temp;
        scanner >> temp;
        if (scanner.fail())
            break;
        altered.push_back(temp);
    }
    scanner.close();
    
    std::vector<int> map(numVertices+1,0);
    for(int i = 0; i < original.size(); ++i)
        map[original[i]] = altered[i];
    return map; 
}

// Main method
int main()
{
    numEdges = 0;
    numVertices = 0;

    // File input
    std::string input;
    std::cout << "Enter graph name (without extension): ";
    std::cin >> input;
    std::ifstream fileReader;
    fileReader.open(input + ".graph");
    if (!fileReader.is_open())
    {
        std::cout << "Could not open file." << std::endl;
        return 1;
    }

    // Vectors that will hold the graphs
    std::vector<int> a;
    std::vector<int> b;
    std::vector<int> original;

    // Extract information from graph
    while (fileReader)
    {
        int temp;
        fileReader >> temp;
        if (fileReader.fail())
            break;

        a.push_back(temp);
        original.push_back(temp);
        if (temp > numVertices)
            numVertices = temp;

        fileReader >> temp;
        b.push_back(temp);
        original.push_back(temp);
        if (temp > numVertices)
            numVertices = temp;
        numEdges++;
    }
    fileReader.close();

    //Store originals for mapping differences
    std::vector<int> sourceA = a;
    std::vector<int> sourceB = b;
    
    //Exhasutively generate the permutations
    std::unordered_set<std::string> permutations;
    exhaustive(permutations, a, b, 1, 1);

    //Output files
    std::cout << "Generated all " << permutations.size() << " isomorphic variants of the input graph." << std::endl;
    std::cout << "Write all the files in the current directory? (y/n)" << std::endl;
    char response1;
    std::cin >> response1;
    if (! (response1 == 'y') && ! (response1 == 'Y'))
        return 0;
    int count = 1;
    for (std::unordered_set<std::string>::iterator iter = permutations.begin(); iter != permutations.end(); iter++)
    {
        std::string output = input + std::to_string(count) + ".graph";
        std::ofstream fileWriter(output);
        fileWriter << *iter;
        count++;
        fileWriter.close();
    }
    std::cout << "Successfully created all graph files.\n" << std::endl;

    //LaTeX Genetation
    std::cout << "Is there a " << input << ".txt file containing coordinates for the LaTeX display? (y/n)" << std::endl;
    char response2;
    std::cin >> response2;
    if(!(response2 == 'y') && !(response2 == 'Y'))
        return 0; 

    //Store the absolute coordinates from user
    std::vector<std::pair<double,double> > coordinates;
    std::ifstream pointReader;
    pointReader.open(input + ".txt");
    if (!pointReader.is_open())
    {
        std::cout << "Could not find file." << std::endl;
        return 1;
    }
    for(int i = 0; i < numVertices; ++i)
    {
        std::pair<double,double> point; 
        pointReader >> point.first;
        pointReader >> point.second;
        coordinates.push_back(point);
    }
    pointReader.close();

    //Create header in LaTeX code
    std::string name =  input + ".tex";
    std::ofstream latexWriter(name);
    latexWriter << "\\documentclass{article}" << std::endl;
    latexWriter << "\\usepackage[utf8]{inputenc}" << std::endl; 
    latexWriter << "\\usepackage{nopageno}" << std::endl;
    latexWriter << "\\usepackage{tikz}" << std::endl;
    latexWriter << "\\begin{document}" << std::endl; 

    //Append each entry to code
    for(int i = 1; i <= permutations.size(); ++i)
    {
        latexWriter << "\\begin{center}" << std::endl;
        latexWriter << "\\par\\noindent " << input << i << "\\par" << std::endl;
        std::vector<int> map = mapAlterations(input,i,original);
        latexAppend(latexWriter,a,b,coordinates,map);
        latexWriter << "\\end{center}" << std::endl;
    }
    latexWriter << "\\end{document}" << std::endl; 
    std::cout << "Created " << input << ".tex" << " to display graphs in LaTeX" << std::endl; 

    return 0;
}
