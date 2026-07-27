## Goal
This document is prepared to make the next changes visible for all of the people and the lonely developer. 

## v7.x.x-LTS
- The Linux compatibility feature from v7.0.0-BETA will be fully available and ready to use without bugs or issues. 
- The package manager accessibility feature will be removed completely. 

## v8.x.x-LTS
- Starting from this release, the [Dawn Package Manager](https://github.com/DawnPackageSystem/dawn) will be integrated to Jenny. 
- The `winget` and `apt`/`dnf`/`pacman` etc. support will be removed before Dawn Package Manager works stable as a module of Jenny at v7.x.x-LTS.

## NOTICE: 
- v7.x.x-LTS version of Jenny will not have a quite long support time unlike v5.1.9-LTS. The lack of the package manager access feature will be a negative part of the next long-term support release.
- Another reason for the native package manager access being replaced by Dawn is that the `check-updates` and `upgrade-apps` commands are not used by even `hypernova-developer`, the lonely maintainer.
- Also, the integration of Dawn Package Manager will increase the UX of the Jenny ecosystem. The philosophy of Jenny is to combine all of the tools made by `hypernova-developer` and make them accessible from one tool but only different commands.

## NOTICE 
- The plans for the releases starting from v9.x.x-ALPHA are not definite. They will change in time. 

## v9.x.x-LTS
- The Jenny Toolkit CLI project will switch back to the modular layout at least for the new modules. The integrated old modules will not be separated again from the main code.
- This will make it easier to integrate a project like Mahoraga which took more than 3100 lines of code to Jenny.
- This benefit will make Jenny become a lot wider toolkit including almost all of the other tools made by `hypernova-developer`. 

## v10.x.x-LTS
- This release is going to be the milestone for the Jenny ecosystem.
- Jenny will be able to get extended areas of use. Jenny will be able to run almost all of the tools that can even be coded by another maintainer using configuration files. 

### The Configuration Files
The configuration files will include some of the main keywords of programs. 
- The Title of the Binary File
- The Path of the Binary File
- The Parameters that the Binary File Can Get
- The Configuration files that the Binary File Needs

## v11.x.x-LTS
- Starting from this release, the separated modules from the `main.cpp` file will be permanently deleted from the Jenny Toolkit CLI project.
- Instead of losing those modules, Jenny will start using them with the new feature that makes Jenny be able to trigger any other 
