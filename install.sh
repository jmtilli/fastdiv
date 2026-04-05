#!/bin/sh

if [ '!' -f "lib/libfastdiv.a" -o '!' -f "lib/libfastdiv.so" -o '!' -f "lib/libfastdiv.so.1" ]; then
  echo "fastdiv not made"
  exit 1
fi

PREFIX="$1"

if [ "a$PREFIX" = "a" ]; then
  PREFIX=/usr/local
fi

P="$PREFIX"
H="`hostname`"

if [ '!' -w "$P" ]; then
  echo "No write permissions to $P"
  exit 1
fi
if [ '!' -d "$P" ]; then
  echo "Not a valid directory: $P"
  exit 1
fi

instlib2()
{
  if [ -e "$P/lib/$1/$2" ]; then
    ln "$P/lib/$1/$2" "$P/lib/.$2.fastdivinstold.$$.$H" || exit 1
  fi
  cp "$1/$2" "$P/lib/.$2.fastdivinstnew.$$.$H" || exit 1
  mv "$P/lib/.$2.fastdivinstnew.$$.$H" "$P/lib/$2" || exit 1
  if [ -e "$P/lib/.$2.fastdivinstold.$$.$H" ]; then
    # If you mount binaries across NFS, and run this command on the NFS server,
    # you might want to comment out this rm command.
    rm "$P/lib/.$2.fastdivinstold.$$.$H" || exit 1
  fi
}
instinc2()
{
  if [ -e "$P/include/$2" ]; then
    ln "$P/include/$2" "$P/include/.$2.fastdivinstold.$$.$H" || exit 1
  fi
  cp "$1/$2" "$P/include/.$2.fastdivinstnew.$$.$H" || exit 1
  mv "$P/include/.$2.fastdivinstnew.$$.$H" "$P/include/$2" || exit 1
  if [ -e "$P/include/.$2.fastdivinstold.$$.$H" ]; then
    # If you mount binaries across NFS, and run this command on the NFS server,
    # you might want to comment out this rm command.
    rm "$P/include/.$2.fastdivinstold.$$.$H" || exit 1
  fi
}
instsym()
{
  if [ "`readlink "$P/lib/$1"`" != "libfastdiv.so.1" ]; then
    ln -s libfastdiv.so.1 "$P/lib/.$1.smkinstnew.$$.$H" || exit 1
    mv "$P/lib/.$1.smkinstnew.$$.$H" "$P/lib/$1" || exit 1
  fi
}

instlib2 lib libfastdiv.a
instlib2 lib libfastdiv.so.1
instsym libfastdiv.so

instinc2 lib fastdivhdr.h

echo "All done, fastdiv has been installed to $P"
