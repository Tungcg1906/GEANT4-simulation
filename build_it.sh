conda activate geant4 
export CMAKE_PREFIX_PATH=/home/enric/miniconda3/envs/geant4/

DIR_PATH=/home/enric/University/Thesis/Cal-SNN/GEANT4-simulation/simulation
cd $DIR_PATH

BUILD_DIR=build
if [ -d "$BUILD_DIR" ]; then
  cd $BUILD_DIR
  echo "Cleaning sub-directory $BUILD_DIR."
  rm -r *
else
  echo "Creating sub-directory $BUILD_DIR."
  mkdir $BUILD_DIR
  cd $BUILD_DIR
fi

cmake ../
make
