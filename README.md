# Faiss

[Faiss](https://github.com/facebookresearch/faiss) - efficient similarity search and clustering - for Ruby

Learn more about [Faiss](https://engineering.fb.com/data-infrastructure/faiss-a-library-for-efficient-similarity-search/)

[![Build Status](https://travis-ci.org/ankane/faiss.svg?branch=master)](https://travis-ci.org/ankane/faiss)

## Installation

First, install the [Faiss C++ library](https://github.com/facebookresearch/faiss/blob/master/INSTALL.md). For Homebrew, use:

```sh
brew install faiss
```

Add this line to your application’s Gemfile:

```ruby
gem 'faiss'
```

Faiss is not available for Windows yet

## Getting Started

Prep your data

```ruby
objects = [
  [1, 1, 2, 1],
  [5, 4, 6, 5],
  [1, 2, 1, 2]
]
```

Build an index

```ruby
index = Faiss::IndexFlatL2.new(4)
index.add(objects)
```

Search

```ruby
distances, ids = index.search(objects, 3)
```

Save an index [master]

```ruby
index.save("index.bin")
```

Load an index [master]

```ruby
Faiss::Index.load("index.bin")
```

> Use `Faiss::IndexBinary` to load binary indexes

## Basic Indexes

Exact search for L2

```rb
Faiss::IndexFlatL2.new(d)
```

Exact search for inner product

```rb
Faiss::IndexFlatIP.new(d)
```

Hierarchical navigable small world graph exploration

```rb
Faiss::IndexHNSWFlat.new(d, m)
```

Inverted file with exact post-verification

```rb
Faiss::IndexIVFFlat.new(quantizer, d, nlists)
```

Locality-sensitive hashing

```rb
Faiss::IndexLSH.new(d, nbits)
```

Product quantizer (PQ) in flat mode

```rb
Faiss::IndexPQ.new(d, m, nbits)
```

IVFADC (coarse quantizer+PQ on residuals)

```rb
Faiss::IndexIVFPQ.new(quantizer, d, nlists, m, nbits)
```

IVFADC+R (same as IVFADC with re-ranking based on codes)

```rb
Faiss::IndexIVFPQR.new(quantizer, d, nlists, m, nbits, m_refine, nbits_refine)
```

## Binary Indexes

Index binary vectors

```rb
Faiss::IndexBinaryFlat.new(d)
```

Speed up search with an inverse vector file

```rb
Faiss::IndexBinaryIVF.new(quantizer, d, nlists)
```

## K-means Clustering

Train

```ruby
kmeans = Faiss::Kmeans.new(4, 2)
kmeans.train(objects)
```

Get the centroids

```ruby
kmeans.centroids
```

## PCA

Train

```ruby
mat = Faiss::PCAMatrix.new(40, 10)
mat.train(objects)
```

Apply

```ruby
mat.apply(mt)
```

## Product Quantizer

Train

```ruby
pq = Faiss::ProductQuantizer.new(32, 4, 8)
pq.train(objects)
```

Encode

```ruby
pq.compute_codes(objects)
```

Decode

```ruby
pq.decode(codes)
```

## Data

Data can be an array of arrays

```ruby
[[1, 2, 3], [4, 5, 6]]
```

Or a Numo NArray

```ruby
Numo::SFloat.new(3, 2).seq
```

## History

View the [changelog](https://github.com/ankane/faiss/blob/master/CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/faiss/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/faiss/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone https://github.com/ankane/faiss.git
cd faiss
bundle install
bundle exec rake compile
bundle exec rake test
```
