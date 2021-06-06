# llvm-examples

## Building a project.
* In the project directory (e.g. Dom).
* mkdir build && cd build
* CC=clang-12 CXX=clang++-12 cmake -G"Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
* export LLVM_DIR=/usr/lib/llvm-12
* ninja && $LLVM_DIR/bin/opt -load-pass-plugin ./libDom.so -passes=dom -disable-output ../../inputs/MatMul4x4.ll

## Linting a project.
* Only one time:
* ln -s build/compile_commands.json compile_commands.json
* $LLVM_DIR/bin/clang-tidy Dom.cpp
* $LLVM_DIR/bin/clang-format -i Dom.cpp

