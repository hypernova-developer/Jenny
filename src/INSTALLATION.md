## Goal
This file is an installation guide for new users. Please do everything mentioned in this document. 

## Downloading Jenny
**Git** is needed for this section. If you had not installed, you can install Git [here](https://git-scm.com/install/windows/)
Open a new terminal window. Copy and paste the command below:

```powershell
git clone https://github.com/hypernova-developer/Jenny.git
```

## Preparing Files
A compiler is needed for this section. If you do not have one, I highly recommend GCC from GNU. You can install it [here](winlibs.com)
Use the command below to go to the folder where source codes are at:

```powershell
cd "src/"
```

## Installing Jenny
You will have to compile all of the source codes inside the folder. Before this section, be sure that you added your compiler as an environment variable. If everything is alright, you can use the commands below:

```powershell
g++ .\jenny.cpp -o jenny.exe -O3 -lwininet -lole32 -loleaut32 -lwbemuuid
```

```powershell
g++ .\SentinelCore.cpp -o SentinelCore.exe -O3 -std=c++17 -lwintrust -lbcrypt -liphlpapi
```

```powershell
g++ .\compilex.cpp -o compilex|engine.exe -O3 -std=c++20
```

