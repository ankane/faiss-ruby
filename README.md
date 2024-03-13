# Faiss Ruby

[Faiss](https://github.com/facebookresearch/faiss) - efficient similarity search and clustering - for Ruby

Learn more about [Faiss](https://engineering.fb.com/data-infrastructure/faiss-a-library-for-efficient-similarity-search/)

[![Build Status](https://github.com/ankane/faiss-ruby/actions/workflows/build.yml/badge.svg)](https://github.com/ankane/faiss-ruby/actions)

## Installation

First, ensure BLAS, LAPACK, and OpenMP are installed. For Mac, use:

```sh
brew install libomp
```

For Ubuntu, use:

```sh
sudo apt-get install libblas-dev liblapack-dev
```

Then add this line to your application’s Gemfile:

```ruby
gem "faiss"
```

It can take a few minutes to compile the gem. Windows is not currently supported.

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

Save an index

```ruby
index.save("index.bin")
```

Load an index

```ruby
index = Faiss::Index.load("index.bin")
```

> Use `Faiss::IndexBinary` to load binary indexes

## Basic Indexes

Exact search for L2

```ruby
Faiss::IndexFlatL2.new(d)
```

Exact search for inner product

```ruby
Faiss::IndexFlatIP.new(d)
```

Hierarchical navigable small world graph exploration

```ruby
Faiss::IndexHNSWFlat.new(d, m)
```

Inverted file with exact post-verification

```ruby
Faiss::IndexIVFFlat.new(quantizer, d, nlists)
```

Locality-sensitive hashing

```ruby
Faiss::IndexLSH.new(d, nbits)
```

Scalar quantizer (SQ) in flat mode

```ruby
Faiss::IndexScalarQuantizer.new(d, qtype)
```

Product quantizer (PQ) in flat mode

```ruby
Faiss::IndexPQ.new(d, m, nbits)
```

IVF and scalar quantizer

```ruby
Faiss::IndexIVFScalarQuantizer.new(quantizer, d, nlists, qtype)
```

IVFADC (coarse quantizer+PQ on residuals)

```ruby
Faiss::IndexIVFPQ.new(quantizer, d, nlists, m, nbits)
```

IVFADC+R (same as IVFADC with re-ranking based on codes)

```ruby
Faiss::IndexIVFPQR.new(quantizer, d, nlists, m, nbits, m_refine, nbits_refine)
```

## Binary Indexes

Index binary vectors

```ruby
Faiss::IndexBinaryFlat.new(d)
```

Speed up search with an inverse vector file

```ruby
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

Save a quantizer

```ruby
pq.save("pq.bin")
```

Load a quantizer

```ruby
pq = Faiss::ProductQuantizer.load("pq.bin")
```

## Data

Data can be an array of arrays

```ruby
[[1, 2, 3], [4, 5, 6]]
```

Or a Numo array

```ruby
Numo::NArray.cast([[1, 2, 3], [4, 5, 6]])
```

## History

View the [changelog](CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/faiss-ruby/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/faiss-ruby/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone --recursive https://github.com/ankane/faiss-ruby.git
cd faiss-ruby
bundle install
bundle exec rake compile
bundle exec rake test
```
