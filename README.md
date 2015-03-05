# jemallocator (C++)

## Introduction

A header-only C++11 STL allocator adaptor for [jemalloc](https://github.com/jemalloc/jemalloc).

Features:
* Convenient API
* Comprehensive test suite

## Installation

If you're a developer, ensure jemalloc is installed and available on your system path.

The following commands build and install jemallocator:

```bash
$ git clone git://github.com/bigdatadev/jemallocator.git
$ cd jemallocator
$ autoreconf -i
$ ./configure && make check && make install
```

## Contributing

Just send me a pull request. Please take a look at the project issues and see how you can help.

## Author

* [@bigdatadev](https://github.com/bigdatadev)

## Copyright

See [LICENSE](LICENSE) document