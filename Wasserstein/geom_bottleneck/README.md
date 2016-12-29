Accompanying paper: M. Kerber, D. Morozov, A. Nigmetov. Geometry Helps To Compare Persistence Diagrams (ALENEX 2016, http://www.geometrie.tugraz.at/nigmetov/geom_dist.pdf)

Bug reports can be sent to "nigmetov EMAIL SIGN tugraz DOT at".

# Dependencies

The program uses the ANN library (http://www.cs.umd.edu/~mount/ANN/),
modified to support deletion of points from k-d trees.
The modified version is contained in "bottleneck/src/ann" and "bottleneck/iclude/ANN"
directories, there is no need to build ANN separately or include ANN's headers.

Your compiler must support C++11.

# Usage:

1. To use a standalone command-line utility bottleneck_dist:

bottleneck_dist file1 file2  [relative_error]. 

If relative error is not supplied, the exact distance is computed.
file1 and file2 must contain persistence diagrams in plain text format 
(one point per line, empty lines are ignored, comments can be made with #):

# this is how your input can look like
x_1 y_1 # two real numbers per line
...
# empty lines or comments are ignored
x_n y_n 

2. To use from your code:

#include "bottleneck.h"

// All classes and functions are in geom_bt namespace
// (including modified ANN's classes).

std::vector<std::pair<double, double>> diagram1, diagram2;
// any container class that supports range-for loops will do.
// A pair represents a single point, 
// first component = x-coordinate,
// second component = y-coordinate.
// ...
// load your diagrams into diagram1, diagram2 (off-diagonal points).
// If you data is in plain text format, you can use readDiagramPointSet function:

if (!geom_bt::readDiagramPointSet("diagram1.txt", diagram1)) {
    // something went wrong: function returns true if it successfully read the file
    }

// OK: diagram1.txt was read.
// ...
// to get exact distance:
double btDist = geom_bt::bottleneckDistExact(diagram1, diagram2);
// to get 1% approximation
double btDistApprox = geom_bt::bottleneckDistApprox(diagram1, diagram2, 0.01);
// ..............................................................................
// if diagrams will be used many times, you may want to avoid copying them
// to DiagramPointSet (which is done internally in each call to 
bottleneckDistExact/bottleneckDistApprox) and do it yourself once. 
// Constructor takes two iterators:
geom_bt::DiagramPointSet ds1(diagram1.begin(), diagram1.end());
geom_bt::DiagramPointSet ds2(diagram2.begin(), diagram2.end());
btDist = geom_bt::bottleneckDistExact(ds1, ds2);
btDistApprox = geom_bt::bottleneckDistApprox(ds1, ds2, 0.01);

Necessary projections (diagonal points) will be added in the bottleneckDistApprox
function.

See also code in example/bottleneck_dist.cpp.

# Remarks:

1) If bottleneckDistApprox is called with epsilon = 0.0, it will never return.
2) Empty diagrams are not considered as error.
3) Modifications made in the ANN code are only valid for 2-dimensional k-d trees.
Do not use the modified ANN files from the project folder anywhere else.
4) You can switch to non-geometric version by using another typedef in 
bottleneck/include/neigb_oracle.h.

# License

The program is distributed under Lesser GPL license.

# Building

CMakeLists.txt in the root directory can be used to make the library (contained 
in bottleneck/ directory) and the command-line utility (in example/ directory)
to compute the distance between two diagrams in txt files.

On Linux/Mac:

mkdir build
cd build
cmake ..
make

On Windows (checked with Visual Studio 2015, Community version)
use cmake-gui to create the solution in build directory and build it with VS.
