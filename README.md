# opencl-matrix-add
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)



## Build

```bash
$ clang++ main.cpp -o main -lOpenCL -std=c++11
```

## Usage

Generate two input matrices with columns separated by commas and rows separated by newlines.

Example matrix:
```
1,2,3,4,5,6
1,1,1,1,1,1
1,1,1,1,1,1
```

Provide the two input matrices and number of columns:

```bash
$ ./main a.txt b.txt 6
```


Output is written to stdout so can be piped to a new file.

stdout:
```bash
$ ./main a.txt b.txt 6
2,4,6,8,10,12
2,2,2,2,2,2
6,2,6,2,6,2
```

or pipe:
```bash
$ ./main a.txt b.txt 6 > results.txt
```


