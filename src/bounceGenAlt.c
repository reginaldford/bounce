// Read https://github.com/reginaldford/bounce/blob/main/LICENSE for license info

unsigned char *bounceGenAlt(unsigned char *key) {
  static unsigned char altKey[256];
  for (unsigned char i = 0; i < 255; i++)
    altKey[i] = key[(i + 64) % 256]+64 % 256;
  return altKey;
}
