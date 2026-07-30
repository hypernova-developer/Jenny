## Goal

This file is an installation guide for Linux users. Please follow all steps mentioned in this document to complete the setup.

## Downloading Jenny

**Git** is required for this section. If it is not installed, you can install it using your distribution's package manager (e.g., `sudo apt install git` on Debian/Pardus/Ubuntu).

Open a terminal window, then copy and paste the command below:

```bash
git clone https://github.com/hypernova-developer/Jenny.git

```

## Preparing Files

A C++ compiler (such as GCC/g++) and essential build tools are required for this section. On Pardus/Debian/Ubuntu, you can install them via `sudo apt install build-essential`.

Use the command below to navigate to the source code directory:

```bash
cd "Jenny/src/"

```

## Installing Jenny

All modules have been unified into a single core architecture. Ensure that `g++` is installed and accessible in your system's `PATH`. If everything is ready, run the following command to compile the binary:

```bash
g++ -std=c++20 -O3 -Wall -Wextra jenny.cpp -o jenny -pthread

```

## Completing Installation

To run `jenny` globally from any terminal directory, move the compiled binary to `/usr/local/bin` (or add the current directory to your shell's `PATH`):

```bash
sudo mv jenny /usr/local/bin/

```

After this step, you can execute `jenny` from anywhere in your system.

## Configuring Jenny

Jenny relies on a configuration file (`jenny.conf`) for system paths and settings. Nothing is hardcoded into the binaries. If you modify your paths or settings in `jenny.conf`, you will not need to recompile the project. Before running **Jenny**, make sure you have edited `jenny.conf` according to your environment.

## Information About Configuration

Be careful when entering directory paths (e.g., using UNIX-style forward slashes `/`). While a default template is provided within the configuration file, you can check [CONFIGURATION.md](https://www.google.com/search?q=CONFIGURATION.md) for detailed descriptions of each field.

## Thanks

Thanks for reading this document. Good luck with your projects!

> **hypernova-developer**
