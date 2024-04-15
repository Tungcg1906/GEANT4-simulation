# Geant4 simulation

Geant4 simulation data for ["Paticle Detector optimization with Deep Learning techniques"](https://github.com/Tungcg1906/Particle-Detectors-optimization-with-Deep-Learning-techniques) project. 

 ![Data](https://github.com/Tungcg1906/Particle-Detectors-optimization-with-Deep-Learning-techniques/blob/main/images/geant4_sim_test.png)


### Dataset old structure

- Each folder contains 100 .root files with 1000 events each, labeled with a file number (i.e. proton_i.root with i=1,…,100).
- Each file contains two trees:
  
 ![Data](https://github.com/Tungcg1906/Particle-Detectors-optimization-with-Deep-Learning-techniques/blob/main/images/data-struct.png)



| **part_info (MC truth of particle hits)**  | **E_dep (the energy stored by the hit in each cell)** |
| ------------- | ------------- |
| Cut: particle momentum >2MeV  | Cut: energy deposit > 1keV  |
| event_id: int to label all hits belonging to the same event  | event_id: same as before, int ID to filter single events  |
| pdg_id: PDG encoder for particle species  | #N/A |
| Track_id: int label for the track the hit belongs to  | #N/A |
| parent_id: int label for reconstructing where hit was originated from  | #N/A |
| pos_x,pos_y,pos_z: hit position  | Cell_no: int ID corresponding to which cell is collecting the deposit  |
| Mom: particle momentum  | #N/A |
| edepo: energy deposited in the hit  | edep: energy deposited in cell  |


### Dataset new structure

####################################################################################################

_In progress - awaiting results_

####################################################################################################

### Execute the code
We need to compile it inside a build folder. 

```C++
mkdir build

cd build

cmake ../
```

This way we set the dependencies for our project. Now to compile it, inside the build folder:
```C++
Make
```
This creates the executable - default name is exampleB4a
```C++
./exampleB4a // Run with graphic
/exampleB4a -m run2.mac -t 4 // Run without graphic
```
