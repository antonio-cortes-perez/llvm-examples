int simpleArithmetic(int A, int B, int C, int D) {
  A = B + C;
  B = A - D;
  C = B + C;
  D = A - D;
  return D;
}
