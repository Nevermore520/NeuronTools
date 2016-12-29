# Bottleneck distance tool
This is a great tool to compute Bottleneck distance. For updated version and more details, please check their repository: https://bitbucket.org/grey_narn/hera/src/1bfe54d664d929884a579973b00960b4348c4258/geom_bottleneck/?at=master

# Dependencies

The program uses the ANN library (http://www.cs.umd.edu/~mount/ANN/),
modified to support deletion of points from k-d trees.
The modified version is contained in "bottleneck/src/ann" and "bottleneck/iclude/ANN"
directories, there is no need to build ANN separately or include ANN's headers.

Your compiler must support C++11.

# Usage:

To use a standalone command-line utility bottleneck_dist:

`bottleneck_dist path_to_persistence_diagrams_folder path_to_output_distance_matrix_file` 

See also code in example/bottleneck_dist.cpp.

## Input File Format
Check [here](https://github.com/Nevermore520/NeuronTools/blob/master/CPP/src/README.md) for the output file format, which is used as input file by this code.

## Output File Format
There is one output file for this program.<br/>
The first line contains two numbers D and D specifying the dimension of the distance matrix.<br/>
The following D lines each contains D number, and they work as the distance matrix.

# License

The program is distributed under Lesser GPL license.

# Building

CMakeLists.txt in the root directory can be used to make the library (contained 
in bottleneck/ directory) and the command-line utility (in example/ directory)
to compute the distance between two diagrams in txt files.

On Linux/Mac:

mkdir build<br/>
cd build<br/>
cmake ..<br/>
make

On Windows (checked with Visual Studio 2015, Community version)
use cmake-gui to create the solution in build directory and build it with VS.
