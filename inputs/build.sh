set -eux

LLVM_DIR=/usr/lib/llvm-11

$LLVM_DIR/bin/clang-format -i $1.c

$LLVM_DIR/bin/clang-tidy $1.c --

$LLVM_DIR/bin/clang -O2 -Xclang -disable-llvm-passes -fno-discard-value-names \
                    -S -emit-llvm $1.c -o $1.ll

$LLVM_DIR/bin/opt -mem2reg -S $1.ll -o $1.ll

$LLVM_DIR/bin/opt -disable-output -dot-cfg $1.ll
