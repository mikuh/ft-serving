# ft-serving
A fasttext inference serving.

## Build the example using cmake:

```shell
mkdir -p cmake/build
pushd cmake/build
cmake -DCMAKE_PREFIX_PATH=/home/geb/.grpc ../..
make -j
```