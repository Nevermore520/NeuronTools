
# Executable Files

## Example workflow
![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/img/executables.png)

## Example command line
Change file format for all input swc files
`java -jar FTMain.jar ..\data\data_127 ..\data\output` <br/>
Generate persistence diagram files
`NTMain.exe ..\data\output\pd`
Generate distance matrix file
`wasserstein_dist ../data/pd ../data/dist_mat`
