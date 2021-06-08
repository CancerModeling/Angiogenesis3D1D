# Angiogenesis3D1D

[![CircleCI](https://circleci.com/gh/CancerModeling/Angiogenesis3D1D.svg?style=shield)](https://circleci.com/gh/CancerModeling/Angiogenesis3D1D) [![GitHub release](https://img.shields.io/github/release/CancerModeling/Angiogenesis3D1D.svg)](https://GitHub.com/CancerModeling/Angiogenesis3D1D/releases/) [![GitHub license](https://img.shields.io/github/license/CancerModeling/Angiogenesis3D1D.svg)](https://github.com/CancerModeling/Angiogenesis3D1D/blob/main/LICENSE) [![GitHub issues](https://img.shields.io/github/issues/CancerModeling/Angiogenesis3D1D.svg)](https://github.com/CancerModeling/Angiogenesis3D1D/issues) [![GitHub repo size](https://img.shields.io/github/repo-size/CancerModeling/Angiogenesis3D1D.svg)](https://GitHub.com/CancerModeling/Angiogenesis3D1D/)

<p align="center"> <img src="https://github.com/CancerModeling/Angiogenesis3D1D/blob/main/assets/logo/logo.png" width="300"> </p>

## Table of contents

  - [Introduction](#Introduction)
  - [Examples](#Examples)
  - [Installation](#Installation)
    * [Dependencies](#Dependencies)
    * [Building the code](#Building-the-code)
    * [Recommendations for quick build](#Recommendations-for-quick-build)

## Introduction
3D-1D tumor growth model for simulation of angiogenesis. The code is used to generate results in the papers

> Marvin Fritz, Prashant K. Jha, Tobias Köppl, J. Tinsley Oden, and Barbara Wohlmuth. **Analysis of a new multispecies tumor growth model coupling 3D phase-fields with a 1D vascular network.** *Nonlinear Analysis: Real World Applications 61 (2021): 103331*. 

> Marvin Fritz, Prashant K. Jha, Tobias Köppl, J. Tinsley Oden, Andreas Wagner, and Barbara Wohlmuth. **Modeling and simulation of vascular tumors embedded in evolving capillary networks.** *arXiv preprint arXiv:2101.10183 (2021)*. **To appear in Computer Methods in Applied Mechanics and Engineering**.

## Examples
Example of tumor growth with two vessels (artery and vein) is provided in the directory [two_vessels](examples/two_vessels). Python scripts are used to create various input files. 

To run the example, first copy the `examples` directory inside the build directory. In shell script, we use relative path to obtain the executible. To run the example from other directory, one needs to modify the [run script](examples/two_vessels/run.sh) and provide the path to binary directory in the variable `EXEC_DIR` in run script. After the `examples` directory is copied, `cd` to the `two_vessels` directory and run the example using
```sh
python3 two_vessels.py run
```
Above will create following input files used in the code:
- input.in - File that provides final time, mesh size, and values of parameters in the model
- tum_ic_data_`tag`.csv - Provides the location and size of tumor cores
- two_vessels_`tag`.dgf - Provides the initial vessel network

## Installation

### Dependencies
Core dependencies are:
  - [cmake](https://cmake.org/) (3.10.2 or above) 
    * recommend to install using `apt-get`
  - [vtk](https://vtk.org/) (7.1.1)
    * recommend to install using `apt-get`
    * required to output simulation results in `.vtu` format
  - [petsc](https://github.com/petsc/petsc) (3.13.3 or above)
    * required to build libmesh
    * see further below on how to build this
  - [libmesh](https://github.com/libMesh/libmesh) (1.5.0 or above)
    * core library
    * see further below on how to build this
  - [aixlog](https://github.com/badaix/aixlog) (1.2.4)
    * included as external library in the code
    * for logging
  - [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) (1.2.4)
    * included as external library in the code
    * for reading csv file
  - [gmm](http://getfem.org/project/libdesc_gmm.html)
    * included as external library in the code
    * provides framework to solve linear systems associated to the 1D networks

Dependencies for running the examples:
  - [python3](https://www.python.org/)
    * required to run the test python scripts
  - [numpy](https://numpy.org/)
    * required to run the test python scripts

### Building the code
Assuming all dependencies are installed in global path (`/usr/local/` etc), we build the code using
```sh
git clone https://github.com/CancerModeling/Angiogenesis3D1D.git

cd Angiogenesis3D1D && mkdir build && cd build

cmake   -DEnable_Documentation=ON \
        -DEnable_Tests=ON \
        -DCMAKE_BUILD_TYPE=Release \
        ..

make -j 4

ctest --verbose
```

If libmesh and petsc are installed at custom paths, we will do
```sh
git clone https://github.com/CancerModeling/Angiogenesis3D1D.git

cd Angiogenesis3D1D && mkdir build && cd build

cmake   -DLIBMESH_DIR="<libmesh install path>" \
        -DPETSC_LIB="<petsc install path>/lib" \
        -DEnable_Documentation=ON \
        -DEnable_Tests=ON \
        -DCMAKE_BUILD_TYPE=Release \
        .. 

make -j 4

ctest --verbose
```

### Recommendations for quick build
1. Install most of the dependencies using `apt-get`:
```sh
sudo apt-get update 
  
sudo apt-get install -y build-essential ubuntu-dev-tools \
  wget curl lzip \
  cmake gfortran \
  libopenmpi-dev openmpi-bin \
  libboost-all-dev libvtk7-dev \
  liblapack-dev libblas-dev \
  doxygen doxygen-latex graphviz ghostscript \
  python3-pip 

# pyvista and pandas are not required, so they can be excluded
pip3 install numpy
```

2. Build petsc and libmesh. You can follow the steps used in building the [dockerfile](https://github.com/prashjha/dockerimages/blob/main/angio-base-bionic/Dockerfile). We plan to include seperate shell script to install petsc and libmesh.

3. Use instructions in previous section to build `Angiogenesis3D1D`. 

### Docker
For `circle-ci` testing, we use docker images `prashjha/angio-base-bionic` and `prashjha/angio-base-focal` of ubuntu 18.04 and 20.04 with petsc and libmesh installed. The associated dockerfiles can be found [here](https://github.com/prashjha/dockerimages). 

In [Packages](https://github.com/orgs/CancerModeling/packages?repo_name=Angiogenesis3D1D), docker images of `Angiogenesis3D1D` is provided. 

## Citations
If this library was useful in your work, we recommend citing the following articles:

> Fritz, M., Jha, P. K., Köppl, T., Oden, J. T., Wagner, A., & Wohlmuth, B. (2021). Modeling and simulation of vascular tumors embedded in evolving capillary networks. arXiv preprint arXiv:2101.10183.

> Fritz, M., Jha, P. K., Köppl, T., Oden, J. T., & Wohlmuth, B. (2021). Analysis of a new multispecies tumor growth model coupling 3D phase-fields with a 1D vascular network. Nonlinear Analysis: Real World Applications, 61, 103331.

## Developers
  - [Prashant K. Jha](pjha.sci@gmail.com)
  - [Tobias Koeppl](koepplto@ma.tum.de)
  - [Andreas Wagner](wagneran@ma.tum.de)