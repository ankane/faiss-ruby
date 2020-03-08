#!/usr/bin/env bash

set -e

CACHE_DIR=$HOME/faiss/$FAISS_VERSION

if [ ! -d "$CACHE_DIR" ]; then
  wget https://github.com/facebookresearch/faiss/archive/v$FAISS_VERSION.tar.gz
  tar xvfz v$FAISS_VERSION.tar.gz
  cd faiss-$FAISS_VERSION
  ./configure --without-cuda
  make
  cd ..
  mv faiss-$FAISS_VERSION $CACHE_DIR
else
  echo "Faiss cached"
fi

cd $CACHE_DIR
make install
