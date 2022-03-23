# BBMap

## Dependency
### wllvm
```
pip install wllvm
```


## Building
Using LLVM-10 (higher versions don't compile)

```bash
cd BBMapPass
mkdir build
cd build
cmake ..
make
```

Find in `BBMapPass/build/bbmap` a file: `libBBMapPass.so`.


## Usage
opt -load <path_to_libBBMapPass.so> -bbmap < target.bc > /dev/null

### Usage with wllvm


```
export LLVM_COMPILER=clang
export CFLAGS="-O0 -g"
export CXXFLAGS="-O0 -g"
CC=wllvm ./configure
make

extract-bc <target>
opt -load <path_to_libBBMapPass.so> -bbmap < target.bc > /dev/null
```
