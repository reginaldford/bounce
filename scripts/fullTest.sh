#!/bin/sh
# Choose your file or we will use the bounce binary for the test
if [ "$#" -eq 1 ] ; then
  file="$1"
else
  file="bounce"
fi

cd ..
make &&
./bounce -g -o k &&
./bounce -k k -i $file -o $file.b &&
./bounce -k k -i $file.b -o $file.2 -d &&
echo "Key:"
head k | xxd &&
echo "Differences:"
diff -a $file $file.2
if [ $? -eq 0 ] ; then
  rm $file2 $file.b k
fi

