
# Executable Files

## Example workflow
![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/img/executables.png)

## Example command line
Change file format for all input swc files (run in Windows) <br/>
`java -jar FTMain.jar ..\data\data_127 ..\data\output 0` <br/>
Generate distance matrix file from vectorized Persistent Diagrams(run in Windows)<br/>
`NTMain_vectorization ..\data\pd ..\data\dist_mat.txt 100` <br/>
Generate persistence diagram files (run in Windows) <br/>
`NTMain.exe ..\data\output ..\data\pd` <br/>
Generate distance matrix file (run in Linux)<br/>
`wasserstein_dist ../data/pd ../data/dist_mat`

## Descriptor functions
The FTMain.jar has three arguments, and the 3rd one is the discriptor function.

| number        | descriptor function        |
| ------------- | --------------------------:|
| 0             | geodesic distance          |
| 1             | geodesic+straight line     |
| 2             | Volume weighted            |
| 3             | Euclidean distance function|
| 4             | Density count function     |
| 5             | y-coordinate function      |

If customized function is needed, please use NTMain_with_func_value.exe directly. The input file format can be found [here](https://github.com/Nevermore520/NeuronTools/tree/master/CPP/src_with_func_value#input-file-format).

The NTMain_vectorization.exe has at least three arguments. The first one is the path to the folder that contains result files generated by code here. The second one is the path to a new file that will contain the result distance matrix. And the third one is the vector size. There is an extra optional argument specify the sample range of each Persistent Diagram.

For more details about the code, please check it here https://github.com/Nevermore520/NeuronTools
