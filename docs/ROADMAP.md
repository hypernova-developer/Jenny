## Goal
This document is prepared to make the next changes visible for all of the people and the lonely developer. 

## v7.x.x-LTS
- The Linux compatibility feature from v7.0.0-BETA will be fully available and ready to use without bugs or issues. 
- The package manager accessibility feature will be removed completely. 

## v8.x.x-LTS
- Starting from this release, the [Dawn Package Manager](https://github.com/DawnPackageSystem/dawn) will be integrated to Jenny. 
- The `winget` and `apt`/`dnf`/`pacman`/etc. support will be removed before Dawn Package Manager works stable as a module of Jenny.

## NOTICE: 
- v7.x.x-LTS version of Jenny will not have a quite long support time unlike v5.1.9-LTS. The lack of the package manager access feature will be a negative part of the next long-term support release.
- Another reason for the native package manager access being replaced by Dawn is that the `check-updates` and `upgrade-apps` commands are not used by even `hypernova-developer`, the lonely maintainer.
- Also, the integration of Dawn Package Manager will increase the UX of the Jenny ecosystem. The philosophy of Jenny is to combine all of the tools made by `hypernova-developer` and make them accessible from one tool but only different commands. 

## v9.x.x-LTS
NOTICE: The plans for this release are not definite. They might change in time. 
- The Jenny Toolkit CLI project will switch back to the modular layout. 
