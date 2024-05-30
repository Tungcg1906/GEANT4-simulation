# !/bin/bash

for i in {1..2}
do 
    ./exampleB4a -m runtest.mac -s $i
    ./Ntuple_9 -f hadron_.root -o /Users/tungnx/Documents/geant4/simulation/root_$i.root -e 1000 -m 0
    #mv /Users/tungnx/Documents/geant4/simulation/build/kaon_$i.root /Users/tungnx/Documents/geant4/simulation/kaon_$i.root
    rm *.root
done
