# Smart Air Pollution Monitor

Firmware for an IoT thing which monitors the pollution in the air. 

## Project Debug
To enabe the print as a DEBUG method, check the ```common.h``` file and set ```ENABLE_DEBUG``` to ```1```. 
The prefered tools is [Putty](https://www.putty.org/) but you can use whatever tool you want. The baudrate is ```115200```.

## Project Setup
First Make sure to have all dependencies set up. If not check the Windowa Setup Dependencies section (You have to do this only once). Then, after everything has been installed correctly do as follows:
- Clone the git repository in your desired folder using 
```
git clone https://github.com/Figuejojo/SmartAirPollutionMonitor.git 
``` 
- Then to clone inside the folder where the project was created, run the following command.
```
git submodule update --init --recursive
```
- Open Visual Studio Code (VSCode) by typing ***code*** from the ***Developer Command Prompt for VS ...*** (Do not confuse it with the normal command console).
- Open the folder of the project in VSCode
- Choose the GCC arm-none-eabi most recent version (10.3.1 when this was written). Change it below if *No kit* is displayed otherwise.
- Then click the *Build* buttom to build the project.

## Load the program
First, put the Raspberry Pi Pico into mass storage mode by clicking the *BOOT* button and then connecting it to the computer. Then check the build folder for a SAPM.uf2 file and drag it into the RPI-RP2 device folder.  

## Windows Setup Dependencies

For the installation of dependencies and SDK setup please refer to:
- Raspberry Pi Getting Starting Guide [link](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- Setting Up RP pico in Windows [link](https://vanhunteradams.com/Pico/Setup/PicoSetup.html)

### Dependencies Download Links
- Install [ARM GCC Compiler](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads). 
- Install [CMake](https://cmake.org/download/)
- Install [GIT](https://git-scm.com/download/win)
- Install [Build Tools for Visual Studio](https://visualstudio.microsoft.com/downloads/)
- Install [Visual Studio Code (VSCode)](https://visualstudio.microsoft.com/downloads/)
- Install [Python 3](https://www.python.org/downloads/windows/)

### SDK Setup
Clone the SDK repository from the raspberry pi pico github inside your preferred folder. To do so, open a command prompt (CMD) and navigate to your folder. Then execute the following command.

    git clone -b master https://github.com/raspberrypi/pico-sdk.git
Then, change the location of your CMD to be inside the newly cloned folder called pico-sdk. Then execute the following command.

    git submodule update --init

Now create a new environmental variable named **PICO_SDK_PATH** with the direction path of the SDK folder as *value*.

# License

[MIT](https://choosealicense.com/licenses/mit/)