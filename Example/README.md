# Summary
This document contains three manually created neuron trees, as well as their persistent diagrams, vectorized persistent diagrams, and pairwise distances.

# Tree Plot
Below are three manually created neuron trees. They are created in two dimensonal space to have a easier view. The second one and the third one are built based on the first one by adding some extra hairs along edges of the first one. Note that the perturbation of Tree 2 is bigger than the perturbation of Tree 3, that is, the extra hair of Tree 2 is longer than that of Tree 3. All of them have the same root (5.0, 5.0). The triple(x, y : dist) beside each point shows the position of this point and its Euclidean distance to the root of tree. The original *.swc files are [here](https://github.com/Nevermore520/NeuronTools/tree/master/Example/neuron%20files).
<p align="center">
  <b>Neuron Tree 1</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/tree%20plots/swc_with_func_val_T1.png)
<p align="center">
  <b>Neuron Tree 2</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/tree%20plots/swc_with_func_val_T2.png)
<p align="center">
  <b>Neuron Tree 3</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/tree%20plots/swc_with_func_val_T3.png)

# Persistent Diagrams
Below are persistent diagrams of those three neuron trees. These diagrams are computed based on the euclidean distance shown in the plots above, which works as function values inside each trees. Note that tree 2 and tree 3 have some same points as tree 1, and they have their own points near the diagonal y = x. This is caused by the perturbation of tree 2 and tree 3 from tree 1.
<p align="center">
  <b>Persistent Diagram of Neuron Tree 1</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/persistent%20diagrams/persistent_diagram_T1.png)
<p align="center">
  <b>Persistent Diagram of Neuron Tree 2</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/persistent%20diagrams/persistent_diagram_T2.png)
<p align="center">
  <b>Persistent Diagram of Neuron Tree 3</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/persistent%20diagrams/persistent_diagram_T3.png)

# Vectorized Persistent Diagrams
Below are plots of vectorized persistent diagrams. The green line in each plot represents vectorized persistent diagram of Tree 1 and red one represent that of Tree 2 and Tree 3 in each plot. Note that the vectors from Tree 1 and Tree 3 are closer to each other than that of Tree 1 and Tree2. 
<p align="center">
  <b>Vectorized Persistent Diagrams of Neuron Tree 1 and Tree 2</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/vectorized%20persistent%20diagrams/vector_T1_T2.png)
<p align="center">
  <b>Vectorized Persistent Diagrams of Neuron Tree 1 and Tree 3</b><br>
</p>

![alt tag](https://github.com/Nevermore520/NeuronTools/blob/master/Example/vectorized%20persistent%20diagrams/vector_T1_T3.png)

# Pairwise Distance
Below are three distance measures among those three trees.

<b> Wasserstein distance among persistent diagrams</b>

|         | Tree 1 | Tree 2 | Tree 3   |
| ------- | ------:|-------:|---------:|
| Tree 1  | 0.0    | 2.47   | 0.015    |
| Tree 2  | 2.47   | 0.0    | 2.49     |
| Tree 3  | 0.015  | 2.49   |  0.0     |

<b> Bottleneck distance among persistent diagrams</b> 

|         | Tree 1 | Tree 2 | Tree 3   |
| ------- | ------:|-------:|---------:|
| Tree 1  | 0.0    | 1.5    | 0.007    |
| Tree 2  | 1.5    | 0.0    | 1.5      |
| Tree 3  | 0.007  | 1.5    |  0.0     |

<b> Euclidean distance among vectorized persistent diagrams</b> 

|         | Tree 1 | Tree 2 | Tree 3   |
| ------- | ------:|-------:|---------:|
| Tree 1  | 0.0    | 42.59  | 0.3      |
| Tree 2  | 42.59  | 0.0    | 42.30    |
| Tree 3  | 0.3    | 42.30  | 0.0      |
