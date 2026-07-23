## Goal
This file is an installation guide for new users. Please do everything mentioned in this document. 

## Downloading Jenny
**Git** is needed for this section. If you had not installed, you can install [Git](https://git-scm.com/install/windows/) from its official website. 
Open a new terminal window. Copy and paste the command below:

```bash
git clone https://github.com/hypernova-developer/Jenny.git

```

## Preparing Files

A compiler is needed for this section. If you do not have one, I highly recommend [MinGW](winlibs.com/) on Windows for performance and security.
Use the command below to go to the folder where source codes are at:

```bash
cd "Jenny/src/"

```

## Installing Jenny

All modules have been unified into a single core architecture. Before this section, be sure that you added your compiler as an environment variable. If everything is alright, you can use the command below to compile the final binary:

```bash
g++ .\jenny.cpp -o jenny.exe -std=c++23 -O3 -s -static -static-libgcc -static-libstdc++ -lole32 -loleaut32 -lwbemuuid -lwintrust -lbcrypt -liphlpapi

```

## Completing Installation

Define "...\Jenny\src" as an environment variable. After this, jenny.exe file will be executed from wherever you are.

## Configurating Jenny

Jenny uses [a configuration file](https://www.google.com/search?q=jenny.conf) for settings. Nothing will be burned into binaries. If you change settings and paths, you will not need to compile the files again. Before running **Jenny**, be sure that you edited [jenny.conf](https://www.google.com/search?q=jenny.conf).

## Information About Configurating

You have to be careful while typing the paths of your own files. The template is given in the file but not descripted. [Here](https://www.google.com/search?q=CONFIGURATION.md) is the description.

## Thanks

Thanks for reading this document. Good luck with your own projects.

> **hypernova-developer**

```
