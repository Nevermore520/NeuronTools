# Wasserstein distance tool
This is a great tool to compute Wasserstein distance. For updated version and more details, please check their repository: https://bitbucket.org/grey_narn/geom_matching

The original program is used for computing Wasserstein distances between persistence 
diagrams using the geometric version of auction algorithm. I make some changes so that 
the current version can compute Wasserstein distance matrix among all persistence diagrams
in a user input folder.

Wasserstein distance $W_{q, p}(X, Y)$ between two persistent diagrams is
the minimum over all perfect matchings between $X$ and $Y$ ( $y(x)$ is the point of $Y$
matched to $x \in X$ ) of the following expression:
$ ( \sum \| x - y(x) \|_p ^ { q } ) ^ { 1 / q} $

# Dependencies

Requires boost 1.58 or higher.
Your compiler must support C++11.

# Usage:

To use a standalone command-line utility wasserstein_dist:

`wasserstein_dist distance_diagrams_path output_distance_matrix_file_path`

See also code in wasserstein/example/wasserstein_dist.cpp.

## Input File Format
Check here(https://github.com/Nevermore520/NeuronTools/blob/master/CPP/src/README.md) for the output file format, which is used as input file by this code.

## Output File Format
There is one output file for this program.<br/>
The first line contains two numbers D and D specifying the dimension of the distance matrix.<br/>
The following D lines each contains D number, and they work as the distance matrix.

# License

See wasserstein/license.txt

# Building

CMakeLists.txt in the root directory can be used to make the library (contained 
in wasserstein/src/ directory) and the command-line utility (in wasserstein/example/ directory)
to compute the distance between two diagrams in txt files.

On Linux/Mac:

mkdir build <br/>
cd build <br/>
cmake .. <br/>
make

On Windows (checked with Visual Studio 2015, Community version)
use cmake-gui to create the solution in build directory and build it with VS.
