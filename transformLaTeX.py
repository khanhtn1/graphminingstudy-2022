# Author: Anuj Ketkar
# Aggie Research Program: Graph Mining and Cybersecurity
# Created for the purpose of displaying the variant patterns in LaTeX


original = input("Enter the name of the original graph: ")
variation = input("Enter the variation number: ")
originalScan = open(original + ".graph")
variationScan = open(original + variation + ".graph")

swapMap = dict()
for line1, line2 in zip(originalScan, variationScan):
    for i in range(len(line1)):
        if line1[i].isdigit() and line1[i] not in swapMap:
            swapMap[line1[i]] = line2[i]

originalLatex = open(original + ".tex", "r")
newLatex = open(original + variation + ".tex", "w")

lines = originalLatex.readlines()

for line in lines:
    for i in range(len(line)):
        if line[i-1] == '{' and line[i+1] == '}':
            newLatex.write(swapMap[line[i]])
        else:
            newLatex.write(line[i])
 
originalLatex.close()
newLatex.close()
