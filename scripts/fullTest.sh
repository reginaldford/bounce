#!/bin/sh
# Choose your file or we will use the bounce binary for the test

os=$(uname -s)

if [ "$os" = "OpenBSD" ]; then
  make="gmake"
else
  make="make"
fi

if [ "$#" = 1 ] ; then
  file="$1"
else
  file="bounce"
fi

cd ..
$make &&
./bounce -g -o k &&
./bounce -k k -i $file -o $file.b &&
./bounce -k k -i $file.b -o $file.2 -d &&
echo "Key:"
head k | xxd &&
echo "Differences:"
diff -a $file $file.2 | head
if [ $? = 0 ] ; then
  rm $file2 $file.b k
fi

