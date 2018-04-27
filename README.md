# FP-growth algorithm implementation in C

## Getting Started

### Installing

#### For Linux:

Make sure you have `gcc`, and:

```
make
```

If you want debug with gdb, you can:

```
make debug
```

#### For Windows:

Please install `x86_64-w64-mingw32-gcc`,
You can use it to compile windows PE file on Linux:

```
make windows
```

## Running the tests

```
./fp_growth test.data 2
```

Output:

```
finish, spent: 0.000167s
frq: 18
free fp_tree
Please press any key to exit.
```

## Usage

```
Usage: ./fp-growth FILE.data MIN_SUP [OUTPUT_FILE_NAME]
```

## Input file format

| User Id | Transaction length | Item IDs                     |
|---------|--------------------|------------------------------|
| 8 Bytes | 4 Bytes            | Transaction length * 4 Bytes |
