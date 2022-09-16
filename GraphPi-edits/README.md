# Modified GraphPi Files
These files have been edited to enable pattern input in the format that the peregrine graph mining program uses.

## Steps
1. [Replace pattern.* files](#1-replace-pattern.*-files)
2. [Replace CMakeLists.txt file](#2-replacecmakelists.txt-file)
3. [Add pattern_count_file.cpp](#3-add-pattern_count_file.cpp)
4. [Compile GraphPi](#4-compile-graphpi)

# 1. Replace pattern.* files
Take the pattern.h file and replace the file of the same name in the include directory. Take the pattern.cpp file and do the same in the src directory.

# 2. Replace CMakeLists.txt file
Take the CMakeLists.txt file and replace the same file in the tianhe directory. Do not replace any other CMakeLists.txt, this will prevent the program from compiling.

# 3. Add pattern_count_file.cpp 
Take the pattern_count_file.cpp and place it in the tianhe directory. 

# 4. Compile GraphPi
Once all the files are added, navigate into the main GraphPi directory and run
'''
cmake CMakeLists.txt
'''
After cmake builds all the makefiles, then run
'''
make
'''
and GraphPi should be built. 

Once GraphPi has been built, the pattern_count_file will be placed in the bin directory. The program has the following layout
'''
/bin/pattern_count_file <dataset_name> <graph_file> <pattern_file>
