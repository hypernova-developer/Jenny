## Goal
This file is an installation guide for new users. Please do everything mentioned in this document. 

## Downloading Jenny
**Git** is needed for this section. If you had not installed, you can install Git [here](https://git-scm.com/install/windows/)
Open a new terminal window. Copy and paste the command below:

```bash
git clone https://github.com/hypernova-developer/Jenny.git
```

## Preparing Files
A compiler is needed for this section. If you do not have one, I highly recommend GCC (GNU Compiler Collection). You can install it [here](winlibs.com)
Use the command below to go to the folder where source codes are at:

```bash
cd "src/"
```

## Installing Jenny
You will have to compile all of the source codes inside the folder. Before this section, be sure that you added your compiler as an environment variable. If everything is alright, you can use the commands below:

```bash
g++ .\jenny.cpp -o jenny.exe -O3 -lole32 -loleaut32 -lwbemuuid
```

```bash
g++ .\SentinelCore.cpp -o SentinelCore.exe -O3 -std=c++17 -lwintrust -lbcrypt -liphlpapi
```

```bash
g++ .\compilex.cpp -o compilex_engine.exe -O3 -std=c++20
```

```bash
g++ .\ProtocolX.cpp -o ProtocolX.exe -O3 -std=c++20
```

## Completing Installation
Define "...\Jenny\src" as an environment variable. After this, jenny.exe file will be executed from wherever you are. 

## Thanks
Thanks for reading this document. Good luck with your own projects.
 > **hypernova-developer**
