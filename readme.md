# GBAATM-Rebirth
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/854bf910a8ed4ec2ad2249a2045d3fbb)](https://app.codacy.com/manual/mte90/GBAATM-Rebirth?utm_source=github.com&utm_medium=referral&utm_content=Mte90/GBAATM-Rebirth&utm_campaign=Badge_Grade_Settings)
[![License](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)   

This project is a refactoring of the old GBAATM (last release around 2011) with the original source code as starting point (got from Cracker, the original author).  
The purpose is a new UI, multiplatform support and maybe new features (with code refactoring and optimization).  
The original source code is available at [this page](https://github.com/Mte90/GBAATM-Rebirth/releases/tag/original).  
The tool to convert a file to the C++ binary representation is avalaible [there](https://github.com/Mte90/GBAATM-Rebirth/releases/tag/bin2array).

![](https://user-images.githubusercontent.com/403283/81478548-88cc8380-921e-11ea-8a90-fc25344fecbc.png)

## Roadmap/Status

* Ported all the features
  * Removed Slowmo
* Revamp the original source code
  * Refactored and optimized to avoid crash and rom damaged
  * Vblank code injected to ROM improved to use 32 bit addresses
* Add new features to UI
  * Checksum - DONE
  * UI in Tabs - DONE
  * Save the path and menu title - DONE
* Auto generate builds for Linux/OSX/Windows - DONE

## Compile

Use QT Creator or this way.

```
mkdir build
cd build
qmake -makefile -o Makefile "CONFIG+=release" ../GBAATM-Rebirth.pro
make
```

## Auto build

When a new tag is generated if prefixed with `v` will be created a new release with the builds attached. If it is prefixed with `pre` it will be the same but it will be marked as pre release on GitHub.