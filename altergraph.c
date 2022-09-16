//Author: Anuj Ketkar
//Aggie Research Program: Graph Mining and Cybersecurity
//Created for the purpose of manually altering graph data

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Reverse all the edges in the graph
void reverse(int *a, int *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        int temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
    printf("Reversed all edges in graph\n");
}

//Flips one of the edges in the graph
void flip(int *a, int *b, int n)
{
    int left = 0;
    int right = 0;
    printf("Enter edge to flip (v1 v2): ");
    scanf("%d %d", &left, &right);

    for (int i = 0; i < n; i++)
    {
        if (a[i] == left && b[i] == right)
        {
            int temp = a[i];
            a[i] = b[i];
            b[i] = temp;
        }
    }
}

// Increments all vertices by a single value; wraps if goes above max
void increment(int *a, int *b, int n, int max)
{
    int add = 0;
    printf("Enter increment size: ");
    scanf("%d", &add);
    add %= max;
    if(add < 0)
        add = max + add;

    for (int i = 0; i < n; i++)
    {
        int tempA = a[i] + add;
        int tempB = b[i] + add;
        if (tempA > max)
            a[i] = tempA - max;
        else
            a[i] = tempA;
        if (tempB > max)
            b[i] = tempB - max;
        else
            b[i] = tempB;
    }
    printf("Incremented all vertices by %d\n", add);
}

// Swaps two vertices
void swap(int *a, int *b, int n)
{
    int first = 0;
    int second = 0;
    printf("Enter vertices to swap (x y): ");
    scanf("%d %d", &first, &second);

    for (int i = 0; i < n; i++)
    {
        if (a[i] == first)
            a[i] = second;
        else if (a[i] == second)
            a[i] = first;
        if (b[i] == first)
            b[i] = second;
        else if (b[i] == second)
            b[i] = first;
    }
    printf("Swapped positions of vertices %d and %d\n", first, second);
}

// Writes new graph to file
void write(int *a, int *b, int n)
{
    char *outputName = malloc(32);
    printf("Enter output file name: ");
    scanf("%s", outputName);
    FILE *writer = fopen(outputName, "w");

    for (int i = 0; i < n; i++)
    {
        fprintf(writer, "%d ", a[i]);
        fprintf(writer, "%d \n", b[i]);
    }
    printf("Graph written to %s\n", outputName);
    free(outputName);
    fclose(writer);
    
}

// Prints current graph
void print(int *a, int *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
        printf("%d\n", b[i]);
    }
}

// Main method
int main()
{
    int numEdges = 0;
    int numVertices = 0;
    char *inputName = malloc(32);
    printf("Enter input file name: ");
    scanf("%s", inputName);

    FILE *file = fopen(inputName, "r");
    char buffer[8];
    while (fgets(buffer, sizeof buffer, file) != NULL)
    {
        int temp1 = 0;
        int temp2 = 0;
        sscanf(buffer, "%d %d", &temp1, &temp2);
        if (temp1 > numVertices)
            numVertices = temp1;
        if (temp2 > numVertices)
            numVertices = temp2;
        numEdges++;
    }
    
    int a[numEdges];
    int b[numEdges];
    rewind(file);
    for (int i = 0; i < numEdges; i++)
    {
        fscanf(file, "%d", &a[i]);
        fscanf(file, "%d", &b[i]);
    }
    printf("Sucessfully loaded graph %s\n", inputName);
    print(a, b, numEdges);

    int menu = 1;
    while (menu)
    {
        int choice = 0;
        printf("------------------------------\nMENU: \n 1. Reverse all the graph's edges \n 2. Flip a single edge \n 3. Increment all labels by a value \n 4. Swap positions of two vertices \n 5. Write the graph to new file \n 6. Quit \n------------------------------\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            reverse(a, b, numEdges);
            print(a, b, numEdges);
            break;
        case 2:
            flip(a, b, numEdges);
            print(a, b, numEdges);
            break;
        case 3:
            increment(a, b, numEdges, numVertices);
            print(a, b, numEdges);
            break;
        case 4:
            swap(a, b, numEdges);
            print(a, b, numEdges);
            break;
        case 5:
            write(a, b, numEdges);
            break;
        default:
            menu = 0;
            break;
        }
    }
    free(inputName);
    fclose(file);
    return 0;
}
