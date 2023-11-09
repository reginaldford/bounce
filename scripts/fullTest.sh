#!/bin/sh
cd ..
make &&
./bounce -g -o k &&
./bounce -k k -i Makefile -o Makefile.b &&
./bounce -k k -i Makefile.b -o Makefile2 -d &&
echo "Key:"
head k | xxd &&
echo "Differences:"
diff -a Makefile Makefile2
rm Makefile2 Makefile.b k

