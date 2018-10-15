# CORBA server/client for the Graphical Interface of Pinocchio and HPP

[![Building Status](https://travis-ci.org/gepetto/gepetto-viewer-corba.svg?branch=master)](https://travis-ci.org/gepetto/gepetto-viewer-corba)
[![Pipeline status](https://gepgitlab.laas.fr/gepetto/gepetto-viewer-corba/badges/master/pipeline.svg)](https://gepgitlab.laas.fr/gepetto/gepetto-viewer-corba/commits/master)
[![Coverage report](https://gepgitlab.laas.fr/gepetto/gepetto-viewer-corba/badges/master/coverage.svg?job=doc-coverage)](http://projects.laas.fr/gepetto/doc/gepetto/gepetto-viewer-corba/master/coverage/)

## Summary
* [gepetto-viewer-corba](#gepetto-viewer-corba)
  * [Setup](#setup)
  * [Dependencies](#dependencies)
  * [Install standalone urdfdom](#install-standalone-urdfdom)
* [gepetto-gui](#gepetto-gui)
  * [Usage](#usage)
    *  [Basic usage](#basic-usage)
    *  [Adding predefined robots and environments](#adding-predefined-robots-and-environments)
    *  [Loading plugins](#loading-plugins)
    *  [For HPP developpers](#for-hpp-developpers)
  * [Installation procedure](#installation-procedure)
    * [Dependencies](#dependencies)
    * [PythonQt](#pythonqt)
* [Troubleshooting](#troubleshooting)

## gepetto-viewer-corba
### Setup

To compile this package, it is recommended to create a separate build
directory:

    mkdir _build
    cd _build
    cmake [OPTIONS] ..
    make install

Please note that CMake produces a `CMakeCache.txt` file which should
be deleted to reconfigure a package from scratch.

### Makefile

A Makefile that installs the dependencies is provided for convinience. Please follow those steps:
  - copy `doc/Makefile`,
  - open and set the variable `SRC_DIR` and `INSTALL_DIR`,
  - optionally, change `OSG_PACKAGE` and `QT_VERSION`,
  - run `make all`

Note that the environment variables `PATH`, `LD_LIBRARY_PATH` and `PKG_CONFIG_PATH` should include
the path to the installation directory.

### Dependencies

The present software depends on several packages which
have to be available on your machine.

 - Libraries:
   - omniORB4 (version >= 4.1.4)
   - openscenegraph (version >= 3.2)
   - urdfdom (version >= 0.3.0)
   - GepettoViewer (Graphical Interface of Pinocchio and HPP)
 - Binaries:
   - omniNames (installed by sudo apt-get install omniorb-nameserver)
 - System tools:
   - CMake (>=2.6)
   - pkg-config
   - usual compilation tools (GCC/G++, make, etc.)

### Install standalone urdfdom

In order to read urdf files (see http://wiki.ros.org/urdf for the description), one haves to install the urdfdom package which can come either along ROS library or be installed as a standalone library. Next section describes the second procedure.

urdfdom depends on both console_bridge and urdfdom_headers. The installation of both dependencies can be done with the following command lines in a terminal :
  - `git clone git://github.com/ros/console_bridge.git && cd console_bridge && mkdir build && cd build && cmake .. && make && sudo make install`
  - `git clone git://github.com/ros/urdfdom_headers && cd urdfdom_headers && mkdir build && cd build && cmake .. && make && sudo make install`

Finally, you just need to apply the following command line to install urdfdom library :
  - `git clone git://github.com/ros/urdfdom && cd urdfdom && mkdir build && cd build && cmake .. && make && sudo make install`

## gepetto-gui
## Usage

#### Basic usage
Launch the binary file `gepetto-gui` and do as in [this video](http://homepages.laas.fr/jmirabel/raw/videos/hpp-gui-example.mp4).

#### Adding predefined robots and environments
For convenience, robots and environments can be predefined.

###### Automatic
* Robots:
```bash
# Use option --predefined-robots to change the robots setting file.
# OPTIONS="--predefined-robots other-robots"
OPTIONS=""

# PR2 from hpp_tutorial
gepetto-gui -g ${OPTIONS} --add-robot "PR2-hpp_tutorial,pr2,planar,pr2,hpp_tutorial,,_manipulation"
# HRP2
gepetto-gui -g ${OPTIONS} --add-robot "HRP2,hrp2,freeflyer,hrp2_14,hrp2_14_description,,"
# Romeo
gepetto-gui -g ${OPTIONS} --add-robot "Romeo,romeo,freeflyer,romeo,romeo_description,,H37V1"
```

* Environments:
```bash
# Use option --predefined-environments to change the environments setting file.
# OPTIONS="--predefined-environments other-environments"
OPTIONS=""

# Kitchen
gepetto-gui -g ${OPTIONS} --add-env "Kitchen,kitchen,iai_maps,kitchen_area"
```

###### Manually
The configuration files are - from the installation prefix - in `etc/gepetto-gui`.
Open `${CMAKE_INSTALL_PREFIX}/etc/gepetto-gui/robots.conf` and write:
```
[PR2 - hpp_tutorial]
RobotName=pr2
ModelName=pr2
RootJointType=planar
Package=hpp_tutorial
URDFSuffix=
SRDFSuffix=_manipulation

[HRP2]
RobotName=hrp2_14
ModelName=hrp2_14
RootJointType=freeflyer
Package=hrp2_14_description
URDFSuffix=
SRDFSuffix=
```

Open `${CMAKE_INSTALL_PREFIX}/etc/gepetto-gui/environments.conf` and write:
```
[Kitchen]
RobotName=Kitchen
Package=iai_maps
URDFFilename=kitchen_area
```

Note: Do not forget to replace `${CMAKE_INSTALL_PREFIX}` by a relevant path.

#### Loading plugins

###### Automatic
```bash
# Use option --config-file to change the settings file.
# OPTIONS="--config-file other-settings"
OPTIONS=""

# Using the core framework: hppcorbaserver
gepetto-gui -g ${OPTIONS} \
  --load-plugin hppwidgetsplugin.so \
  --load-plugin hppcorbaserverplugin.so \
  --load-plugin remoteimuplugin.so

# Using the manipulation framework: hpp-manipulation-server
gepetto-gui -g ${OPTIONS} \
  --load-plugin hppmanipulationwidgetsplugin.so \
  --load-plugin hppmanipulationplugin.so \
  --load-plugin hppmonitoringplugin.so \
  --load-plugin remoteimuplugin.so
```

###### Manually
Open `${CMAKE_INSTALL_PREFIX}/etc/gepetto-gui/settings.conf` and write:
```
[plugins]
hppcorbaserverplugin.so=true
remoteimuplugin.so=true
hppwidgetsplugin.so=true
```

The plugins are looked for in the directory `${CMAKE_INSTALL_PREFIX}/lib/gepetto-gui-plugins`

#### For HPP developpers
As [HPP], the *GUI* can be controlled using a python interface. When the *GUI* starts, it launches a server for both [HPP] and the Gepetto Viewer exactly as if you were manually launching the two commands `hppcorbaserver` and `gepetto-viewer-server`. This means that **you can run the same python scripts** and it will work !

When you do so, pay attention to the following points:
- the GUI has no way of knowing when to refresh the list of joints and bodies. **There is a refresh button in the `Tools` menu**.
- you can run the `hppcorbaserver` (or any server embedding it, like `hpp-manipulation-server`) externally. Use `Tools > Reset connection` when the CORBA client has to reconnect to a new server.
  In this case, set `hppcorbaserverplugin.so` and `hppmanipulationplugin.so` to `false` in your settings file because they are launching the servers themselves.
- moving the robot in the GUI while the server is processing data can lead to unexpected results, because you are modifying the *current configuration* of HPP when not expected.

## Installation procedure
There are a few dependencies to be installed before installing *gepetto-gui*.

### Dependencies
There are a few required dependencies and several optional ones.
#### Core interface
There are only one dependency:
* Qt 4: `sudo apt-get install qt4-dev-tools libqt4-opengl-dev libqtgui4`.

Optionally, for a better rendering:
* `oxygen-icon-theme`: `sudo apt-get install oxygen-icon-theme`

#### Plugins
It is possible to extend the abilities of `gepetto-gui` with plugins.

See [hpp-gui] and [hpp-plot] for examples.

### Installation of *gepetto-gui*
To install `gepetto-gui` you just have to follow the installation step at the top of this page.

### PythonQt
gepetto-gui offers the possibility to develop plugins in python. To do so, you must install PythonQt by following the instructions here: https://github.com/jmirabel/PythonQt#readme

# Troubleshooting

## `CORBA::TRANSIENT` when launching a server

It very often happens that the OmniNames server failed to start properly at boot.

To check if the server is running, run:
```bash
ps -C omniNames -o pid,args
```

If the process is not running, delete omniNames related log and backup files in `/var/lib/omniorb`. They may have different names on your computer, but most likely, something like:
```bash
rm /var/lib/omniORB/omninames-<computer_name>.log
rm /var/lib/omniORB/omninames-<computer_name>.bak
```
then restart the server:
```bash
sudo service omniorb4-nameserver restart
```

## Collada files are not displayed

The nodes are created and exists in the body tree widget but nothing appears in the scene viewer. This is due to a conflict between OSG and Qt. A work around is to convert the DAE to osg using the following command:
```bash
# <file> is the name of the file including the DAE extension.
# The output filename will end with .dae.osg.
osgconv <file> <file>.osg
```
If you have many files, you may run that at the root of the meshes subdirectories:
```bash
find ${root_of_meshes_directory} -iname "*.dae" -type f -exec osgconv {} {}.osg \;
```

[HPP]:http://projects.laas.fr/gepetto/index.php/Software/Hpp
[remoteimu]:https://github.com/jmirabel/remoteimu
[hpp-gui]:https://github.com/jmirabel/hpp-gui
[hpp-plot]:https://github.com/jmirabel/hpp-plot
