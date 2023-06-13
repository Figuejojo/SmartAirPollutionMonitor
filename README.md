# Smart Air Pollution Monitor
Firmware for a Smart IoT Air Pollution Monitor dissertation project for the University of York by the MSc Embedded Wireless System 22/23.

- [1 Project Setup](#ProjectSetup)
 - [1.1 Dependencies](#Dependencies)
 - [1.2 Project Setup Structure ](#PStructure)
   - [1.2.1 SDK Setup](#SDKSetup)
   - [1.2.2 Project Setup](#PSetup)
- [2 Project Development](#PDev)
	- [2.1 Project Debug](#PDebug)
	- [2.2 Load the program](#LoadProgram)
- [3 References](#Ref)

   
<a name="ProjectSetup"></a>
## 1 Project Setup
The Project Setup consist in Downloading all the dependencies, cloning the SDK, cloning this git project and putting it all together for it to build.

<a name="Dependencies"></a>
### 1.1 Dependencies 
The following links are the direct download link. You can refer to this [link](https://vanhunteradams.com/Pico/Setup/PicoSetup.html) on how to install everything.
- Install [ARM GCC Compiler](https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-win32.exe?rev=29bb46cfa0434fbda93abb33c1d480e6&hash=B2C5AAE07841929A0D0BF460896D6E52s) version 10.3 or check the [compiler versions](https://developer.arm.com/downloads/-/gnu-rm).
- Install [CMake](https://github.com/Kitware/CMake/releases/download/v3.27.0-rc2/cmake-3.27.0-rc2-windows-x86_64.msi) version 3.27 for Windows x86-64-bits or check the [CMake versions](https://cmake.org/download/).
- Install [GIT](https://github.com/git-for-windows/git/releases/download/v2.41.0.windows.1/Git-2.41.0-64-bit.exe) version 2.41 for Windows x86-64-bits or check the [git versions](https://git-scm.com/downloads).
- Install [Visual Studio Code (VSCode)](https://code.visualstudio.com/docs/?dv=win) or check the [VScode Versions](https://code.visualstudio.com/).
- Install [Python 3](https://www.python.org/ftp/python/3.11.4/python-3.11.4-amd64.exe) Version 3.11.4 for Windows x86-64-bits or check [Python3 versions](https://www.python.org/downloads/).
- Install [Build Tools for Visual Studio](https://aka.ms/vs/17/release/vs_BuildTools.exe) or check the [Build Tools versions](https://visualstudio.microsoft.com/es/downloads/#build-tools-for-visual-studio-2022).
For the Visual Studio Tools you must check C++ Build Tools with the following elements:
  - MSCV
  - Windows 10 or 11 SDK (Depending on your computer)
  - C++ Cmake Tools for Windows
  - Testing tools core features - Build Tools
  - C++ AddressSanitizer


 <a name="PStructure"></a>
### 1.2 Project Setup Structure 
For the project to build correctly, it must have the same folder structure as below. Here pico-sdk and SmartAirPollutionMonitor are git repositories downloaded inside the same folder.
```
Project_Folder
	├── pico-sdk
	├── SmartAirPollutionMonitor
	└── .
```


 <a name="SDKSetup"></a>
#### 1.2.1 SDK Setup
Clone the SDK repository from the raspberry pi pico GitHub inside your preferred folder. To do so, open a command prompt (CMD) and navigate to your folder. Then you can go ahead and execute the following command.

```shell
C:\Path\YourFolder> git clone -b master https://github.com/raspberrypi/pico-sdk.git
``` 
Then, change the location of your CMD to be inside the newly cloned folder called pico-sdk. Then execute the following command.
```shell
C:\Path\YourFlder\pico-sdk> git submodule update --init
```

 <a name="PSetup"></a>
#### 1.2.2 Project Setup
First Make sure to have all dependencies installed correctly. If not check the Windows Setup Dependencies section or the references at the end. Then, after everything has been installed correctly do as follows:
1.  Clone the git repository in your desired folder using 
```shell
C:\Path\YourProject> git clone https://github.com/Figuejojo/SmartAirPollutionMonitor.git
``` 
2. Get all the submodules clone inside the project, and run the following command inside the Smart Air Pollution Monitor Folder created above:
```shell
C:\Path\YourProject\SmartAirPollutionMonitor> git submodule update --init --recursive
```
3. Open Visual Studio Code (VSCode) by typing ***code*** from the ***Developer Command Prompt for VS ...*** (Do not confuse it with the normal command console).
4. Open the Smart Air Pollution Monitor project folder in VSCode
5. Choose the GCC arm-none-eabi most recent version (10.3.1 when this was written). If it didn't give you the option click where *No kit* is shown below.
6. Then click the *Build* button to build the project. If you got any issues please refer to the references section.
7. Finally if it builds correcly you must see a build folder inside the project with a file named: ```SAPM.uf2```


 <a name="PDev"></a>
## 2 Project Development
Now that everything is setup, this section has details on how to debug and load the program into the Raspberry pi pico-w.

 <a name="PDebug"></a>
### 2.1 Project Debug
To enable the print as a DEBUG method, check the ```SAPMS\common.h``` file and set ```ENABLE_DEBUG``` to ```1```. 
The preferred Serial COM tool is [Putty](https://www.putty.org/) and the Baudrate is set at ```115200```.

This [link](https://the.earth.li/~sgtatham/putty/latest/w64/putty-64bit-0.78-installer.msi) is for the direct download of Putty's version 0.78 for Windows 64-bit x86 

<a name="LoadProgram"></a>
### 2.2 Load the program
First, put the Raspberry Pi Pico into mass storage mode by clicking the *BOOT* button and then connecting it to the computer. Then check the build folder for a SAPM.uf2 file and drag it into the RPI-RP2 device folder.  

<a name="Ref"></a>
## 3 References
For the installation of dependencies and SDK setup please refer to:
- Raspberry Pi Getting Starting Guide [link](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- Setting Up RP pico in Windows [link](https://vanhunteradams.com/Pico/Setup/PicoSetup.html)

# License

[MIT](https://choosealicense.com/licenses/mit/)
