# FIM

<p> <img src="https://skillicons.dev/icons?i=cpp,cmake" alt="C++ and CMake" /> </p>

A file integrity monitor written in C++. It walks a directory, hashes every file with SHA-256, and reports the digests.

> **Status:** v1. Hashing only. Baseline storage and change detection are not implemented yet.

## How it works

```text
traverse directory → open file (binary) → read in 8 KB chunks → SHA-256 → 64-char hex digest
```

- Recursive traversal with `std::filesystem::recursive_directory_iterator`
- Files are read in binary mode, in fixed-size chunks, so memory use stays constant for any file size
- One independent SHA-256 context per file (OpenSSL)

## Requirements

- C++17 compiler
- CMake 3.10+
- OpenSSL development libraries (`libssl-dev` on Debian/Ubuntu)

## Build

```bash
cmake -B build
cmake --build build
```

## Usage

```bash
./build/fim <target-directory>
```

## Project notes

Study notes from building v1 are in [`notes/`](notes/):

- [C++ Concepts](notes/fim-v1-cpp-concepts.md)
- [Syntax & Semantics](notes/fim-v1-syntax-semantics.md)

## Roadmap

- [ ] Store a baseline of hashes
- [ ] Compare against the baseline and report added, modified and deleted files
