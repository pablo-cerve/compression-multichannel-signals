# Compression of Multichannel Signals with Irregular Sampling Rates and Data Gaps

## [1] Datasets
All our experimental datasets can be downloaded from [this page](https://drive.google.com/drive/folders/1YCb9Zzr8uLmYRSrCAMa3SGBqaVyrbWyW) (`datasets.zip` file is a zipped version of the `datasets` folder).

## [2] Compilation
1. Compile the C++ code with [CMake](https://cmake.org/download/) (version >= 3.10.3)
```
$ cd [PROJECT_PATH]
$ [CMAKE_PATH]/cmake .
$ [CMAKE_PATH]/cmake --build . --target run -- -j 4
```
2. Run the tests
```
$ [PROJECT_PATH]/run test
```


## [3] Code and Decode Dataset Files
- The same executable (i.e. `[PROJECT_PATH]/run`) is used for coding and decoding.
- The action is defined by the first argument (`code` or `decode`).

### [3.1] Code
```
$ [PROJECT_PATH]/run code [INPUT_FILE_PATH] [OUTPUT_FILE_PATH] [CODER_NAME] [w] error_mode=epsilon [ε1] [ε2] ... [εn]
```
All the arguments are required:
- `[INPUT_FILE_PATH]`: path of the dataset file to be coded
- `[OUTPUT_FILE_PATH]`: path of the coded file
- `[CODER_NAME]`: name of the coder (i.e. `PCA*`, `APCA*`, `FR*`, `PWLH*`, `PWLH*Int`, `CA*`, `SF*`, `GAMPS*`)
- `[w]`: value for the window size parameter *w*, e.g. `4`, `8`, `16`, `32`, `64`, `128`, `256`
- `[ε1] [ε2] ... [εn]`: `εi` represents the (non-negative integer) error bound ε (as defined in Definition 1.1) for data column `i`. The number of error bounds must match the number of data columns in the input dataset file to be coded.

#### [3.1.1] Code with error parameter *e*
Instead of passing arguments `error_mode=epsilon [ε1] [ε2] ... [εn]`, one can pass arguments `error_mode=e [e]`
```
$ [PROJECT_PATH]/run code [INPUT_FILE_PATH] [OUTPUT_FILE_PATH] [CODER_NAME] [w] error_mode=e [e]
```
- `[e]` represents the error parameter *e* (as defined in Section 4.1), which can have values `0`, `3`, `5`, `10`, `20`, or `30`

The error bounds for each dataset file are precomputed in file `src/dataset/epsilon_mapper.cpp`.

### [3.2] Decode
```
$ [PROJECT_PATH]/run decode [INPUT_FILE_PATH] [OUTPUT_FILE_PATH]
```
All the arguments are required:
- `[INPUT_FILE_PATH]`: path of the coded file to be decoded
- `[OUTPUT_FILE_PATH]`: path of the decoded file
  
### [3.3] Examples
1. Code and decode file from dataset `IRKIS` with coder `APCA*` and window size parameter `8`, using `error_mode=epsilon`
```
$ cd [DATASETS_PATH]
$ [PROJECT_PATH]/run code \[1\]IRKIS/vwc_1202.dat.csv vwc_1202.dat.csv.compressed1.bin APCA* 8 error_mode=epsilon 2 3 2 2 1 2 2 1 4 2
$ [PROJECT_PATH]/run decode vwc_1202.dat.csv.compressed1.bin vwc_1202.dat.csv.decompressed1.csv
```

2. Code and decode file from dataset `IRKIS` with coder `FR*` and window size parameter `32`, using `error_mode=e`
```
$ cd [DATASETS_PATH]
$ [PROJECT_PATH]/run code \[1\]IRKIS/vwc_1202.dat.csv vwc_1202.dat.csv.compressed2.bin APCA* 32 error_mode=e 15
$ [PROJECT_PATH]/run decode vwc_1202.dat.csv.compressed2.bin vwc_1202.dat.csv.decompressed2.csv
```

## [4] External Code
- Implementation of algorithms `PWLH*`, `SF*` and `GAMPS*` reuse part of the source code from the framework cited in [6]. This code is in folder `external/benchmarkLinux`.
- We use the CACM87 implementation [29, 30] of the arithmetic coder, which is written in C. This code is in folder `external/ari`.

[6] N. Q. V. Hung, H. Jeung, K. Aberer, An Evaluation of Model- Based Approaches to Sensor Data Compression, IEEE Transactions on Knowledge and Data Engineering 25 (11) (2013) 2434–2447. doi:10.1109/TKDE.2012.237

[29] I.H. Witten, R.M. Neal, and J.G. Cleary. Arithmetic Coding for Data Compression. Communications of the ACM, 30(6):520–540, 1987.

[30] Data Compression With Arithmetic Coding. https://marknelson.us/posts/2014/10/19/data-compression-with-arithmetic-coding.html, 2014.
