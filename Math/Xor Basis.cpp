int basis[d];
int sz;
void insertVector(int mask) {
  for (int i = 0; i < d; i++) {
    if ((mask & 1 << i) == 0) continue;

    if (!basis[i]) {
      basis[i] = mask;
      ++sz;
      return;
    }
    mask ^= basis[i];
  }
}
