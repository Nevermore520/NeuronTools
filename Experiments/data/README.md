
# Test Data Set

## data_379
This data set is taken from [1] consists 379 neurons, taken from the Chklovskii archive (Drosophila) of NeuroMorpho.Org, manually categorized into 89 types [2]. All the skeletons including the type information can be downloaded from http://neuromorpho.org under the Drosophila - Chklovskii category.

## data_127
This data set consists 127 neurons. The original data set has 114 neurons, from four families: Purkinje, olivocerebellar neurons, Spinal motoneurons and hippocampal interneurons, downloaded from neuronmorpho.org.
Specifically, the 16 Purkinje reconstructions [3, 4, 5, 6] have only dendrites with no axons. The 68 olivocerebellar neurons reconstructions [7] have only axons, with no dendrites. The 17 spinal motoneurons reconstructions [8] have complete dendrites, but only the initial branches of the axons. In this case, we keep only the dendrites in our experiments. The 13 hippocampal interneurons reconstructions [9] have both dendrites and axons. In this case, we separate each hippocampal interneuron reconstruction into two trees: one for dendrites and one for axons.
Finally, we obtain 127 neurons, some of them are dendrite trees and some are axonal trees.

## data_1268
This data set includes 1268 neuron cells, out of which the primary cell class is known for 1130 cells, and there are two primary cell classes: interneurons and principal cells. Both of these classes have complete dendrites. The interneurons have moderately complete axons, the principal cells have incomplete axons. We have not separated the dendrite and axonal trees in this case.
This data set comes from the Human Brain Project [10] and downloaded from NeuroMorpho.Org.

## References
1. Takemura SY, Bharioke A, Lu Z, Nern A, Vitaladevuni S, Rivlin PK, et al. A visual motion detection circuit suggested by Drosophila connectomics. Nature. 2013;500(7461):175{181.
2. Zhao T, Plaza SM. Automatic Neuron Type Identification by Neurite Localization in the   Drosophila Medulla; 2014.
3. Rapp M, Segev I, Yarom Y. Physiology, morphology and detailed passive models of guinea-pig cerebellar Purkinje cells. The Journal of Physiology. 1994;474(1):101{118.
4. Vetter P, Roth A, Hausser M. Propagation of Action Potentials in Dendrites Depends on Dendritic Morphology. Journal of Neurophysiology. 2001;85(2):926{937.
5. Chen XR, Heck N, Lohof AM, Rochefort C, Morel MP, Wehrle R, et al. Mature Purkinje Cells Require the Retinoic Acid-Related Orphan Receptor-a (RORa) to Maintain Climbing Fiber Mono-Innervation and Other Adult Characteristics. The Journal of Neuroscience. 2013;33(22):9546{9562.
6. Martone ME, Zhang S, Gupta A, Qian X, He H, Price DL, et al. The cell-centered database. Neuroinformatics. 2003;1(4):379{395. doi:10.1385/NI:1:4:379.
7. Brown KM, Sugihara I, Shinoda Y, Ascoli GA. Digital morphometry of rat cerebellar climbing fibers reveals distinct branch and bouton types. The Journal of Neuroscience. 2012;32(42):14670{14684.
8. Li Y, Brewer D, Burke RE, Ascoli GA. Developmental changes in spinal motoneuron dendrites in neonatal mice. The Journal of Comparative Neurology. 2005;483(3):304{317. doi:10.1002/cne.20438.
9. Ascoli GA, Brown KM, Calixto E, Card JP, Galvan EJ, Perez-Rosello T, et al. Quantitative morphometry of electrophysiologically identified CA3b interneurons reveals robust local geometry and distinct cell classes. The Journal of Comparative Neurology. 2009;515(6):677{695. doi:10.1002/cne.22082.
10. Markram H, Muller E, Ramaswamy S, Reimann MW, Abdellah M, Sanchez CA, et al. Reconstruction and Simulation of Neocortical Microcircuitry. Cell. 2015;163:456{492.
