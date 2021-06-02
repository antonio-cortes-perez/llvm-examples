void matMul4x4(int *A, int *B, int *C) {
  const int Size = 4;
  for (int IdxA = 0; IdxA < Size; ++IdxA) {
    for (int IdxB = 0; IdxB < Size; ++IdxB) {
      int *RowA = A + IdxA * Size;
      int *ColB = B + IdxB;
      *C = 0;
      for (int Idx = 0; Idx < Size; ++Idx, ++RowA, ColB += Size) {
        *C += *RowA * *ColB;
      }
      C++;
    }
  }
}
