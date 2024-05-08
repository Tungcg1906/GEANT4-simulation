# !/bin/bash

for i in {1..5}
do 
    ./exampleB4a -m run2.mac 
    ./Ntuple_9 -f hadron.root -o /Users/tungnx/Documents/geant4/simulation/kaon_$i.root -e 5 -m 0
    #mv /Users/tungnx/Documents/geant4/simulation/build/kaon_$i.root /Users/tungnx/Documents/geant4/simulation/kaon_$i.root
    rm *.root
done
