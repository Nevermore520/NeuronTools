# Visualization Tool

# How to use
1) Ensure that you have Python 3 installed with the following packages: scipy, scikit-learn, networkx, numpy, bokeh.
2) Run package.py to condense and pre-process the data directory into a single archive. The first argument is the path to the *.swc data  directory, and the second is the name of the resulting file. For instance: ./package.py ./my_neurons_123 my_neurons_123.npz
 3) Run neuronplot.py to generate the plots. The first argument is the path to the data archive generated in the previous step, the second is the path to the distance matrix, and the third is the path to the label file. For instance:
 ./neuronplot.py ./my_neurons_123.npz ./distance_matrix.txt ./labels.txt
 Format of the distance matrix: the first line contains 2 integers indicating the dimension of the distance matrix, and the remaining is a N by N distance matrix.
 Format of the label file: each line starts with integer index, followed by a space, and followed by the relative path to the target *.swc neuron file from the *.swc data directory. An index generator (printIndexFileFitJustinPath) is provided in [Java](https://github.com/Nevermore520/NeuronTools/blob/master/Java/src/IndexFileCreater.java) folder.

