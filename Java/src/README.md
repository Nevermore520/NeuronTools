# SWC File Transfer 
This tool can transform the original swc files to new formatted files using different distance computing functions. 

# usage
The main function is in FTMain.java. Other files contains some helper functions and classes.<br/>
Refer to README file in [NeuronTools](https://github.com/Nevermore520/NeuronTools) for details on how to run this code and provide arguments in Eclipse.

It needs three arguments. <br/>
The first one is the path to the folder that contains all the orginal swc files. <br/>
The secont one is the path to a new folder that will contains result reformated file. <br/>
The thrid one is a number specifying which function to use when transfering files.

## Input SWC file format
Check the file format here: http://www.neuronland.org/NLMorphologyConverter/MorphologyFormats/SWC/Spec.html

## Output file format
Encoding:  ASCII text<br/>

### For function type 1,2 and 4
First line contains two numbers N and E, specifying how many nodes and how many edges in the result graph, which is a tree.<br/>
The following N lines each contains three floating point number specifying position of each node.<br/>
And the following E lines each contains two integer and one floating point number specifying two node IDs and distance between the two nodes.
### For function type 3
First line contains two numbers N and E, specifying how many nodes and how many edges in the result graph, which is a tree.<br/>
The following N lines each contains three floating point number specifying position of each node.<br/>
The following E lines each contains two integer specifying two node IDs, which means these two nodes are connected. <br/>
And the following N lines each contains one number specifying the function value for that node. The node order in this part is the same as it in the first part above.

Under contruction...
