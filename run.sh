rm -rf build/ && 
mkdir build && 
cp src/Roboto-Black.ttf build/ && 
cd build && 
cmake .. && 
make && chmod +x ./Roboto-Black.ttf &&
./MyProject && cd ..