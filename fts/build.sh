mkdir -p cmake/build
pushd cmake/build
cd cmaske/build
cmake -DCMAKE_PREFIX_PATH=/home/geb/.grpc ../..
make -j
