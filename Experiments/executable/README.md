
# Executable Files

## Example workflow
![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/img/executables.png)

## Example command line
Change file format for all input swc files (run in Windows) <br/>
`java -jar FTMain.jar ..\data\data_127 ..\data\output` <br/>
Generate persistence diagram files (run in Windows) <br/>
`NTMain.exe ..\data\output ..\data\pd` <br/>
Generate distance matrix file (run in Linux)<br/>
`wasserstein_dist ../data/pd ../data/dist_mat`

## Descriptor functions
The FTMain.jar has three arguments, and the 3rd one is the discriptor function.
| number        | discriptor function        |
| ------------- |---------------------------:|
| 0             | geodesic distance          |
| 1             | geodesic+straight line     |
| 2             | Volume weighted            |
| 3             | Euclidean distance function|
