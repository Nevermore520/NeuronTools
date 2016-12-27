# NeuronTools

This repository contains all tools I used to perform data testing on neuron cell data.

## Java/src
The Java/src/ folder contains code to transform original swc files to new files, which has different file format, which can be used to compute Persistent Diagrams by using code in CPP/src/
## CPP/src
The CPP/src folder contains code to compute Persistent Diagrams from those files prepared by code in Java/src/.
## Project setup
I build the tools in above Java and CPP directories in Windows using Eclipse. 
### Eclipse for Java
1. Look here for how to install Eclipse for Java in Windows: https://www.ntu.edu.sg/home/ehchua/programming/howto/EclipseJava_HowTo.html
2. Open Eclipse, click File->New->Java Project, enter project name as FileCreatorInJava, then click Finish. 
3. Download all Java files in NeuronTools/Java/src/ (https://github.com/Nevermore520/NeuronTools/tree/master/Java/src), copy and paste them into the src folder under the newly created FileCreatorInJava project.
4. The code is ready to run and test in Eclipse.
### Eclipse for CPP
1. Look here for how to install MinGW and Eclipse for CPP in Windows: https://www3.ntu.edu.sg/home/ehchua/programming/howto/EclipseCpp_HowTo.html
2. Also the C++ code need Boost C++ library, which can be downloaded here: http://www.boost.org/
3. Open Eclipse for CPP, click File->New->C++ Project, enter project name as NeuronTools, select 'Hello World C++ Project' under Project type and select 'MinGW GCC' under Toolchains, and click Finish.
4. Replace files in the src folder under the newly created NeuronTools project with files in NeuronTools/CPP/src/ (https://github.com/Nevermore520/NeuronTools/tree/master/CPP/src)
5. Before running the code, make sure the boost path is added to your includes list. In Eclipse, click Project->Properties, click Settings under 'C/C++ Build', click Tool Settings tab, and add your Boost library install path(e.g. C:\software\boost_1_59_0) to the 'Include path (-I)' in 'GCC C++ Compiler/Includes'. Also add this path in 'GCC C Compiler/Includes'.
6. Now the code is ready to run and test.
## Wasserstein
The wasserstein/ folder contains code to compute Persistent Distance Matrix from Persistent Diagrams computed by code in CPP/src/.

Still under contruction...

