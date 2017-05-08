# Visualization Tool

# How to use
1) Ensure that you have Python 3 installed with the following packages: scipy, scikit-learn, networkx, numpy, bokeh.
2) Run package.py to condense and pre-process the data directory into a single archive. The first argument is the path to the data directory, and the second is the name of the resulting file. For instance:
 ./package.py ./my_neurons_123 my_neurons_123.npz
 3) Run neuronplot.py to generate the plots. The first argument is the path to the data archive generated in the previous step, the second is the path to the distance matrix, and the third is the path to the label file. 
 The label file here need some specific format: each line starts with integer index, followed by a space, and followed by the relative path to the target *.swc neuron files. An index generator (printIndexFileFitJustinPath) is provided in [Java](https://github.com/Nevermore520/NeuronTools/blob/master/Java/src/IndexFileCreater.java) folder.

