# Monocular-Human-Detection-YOLO
![CICD Workflow status](https://github.com/robosac333/Monocular-Human-Detection-YOLO/actions/workflows/run-unit-test-and-upload-codecov.yml/badge.svg) [![codecov](https://codecov.io/gh/robosac333/Monocular-Human-Detection-YOLO/branch/main/graph/badge.svg)](https://codecov.io/gh/robosac333/Monocular-Human-Detection-YOLO) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Step 1: OpenCV Installation
Open a terminal and run the following commands to remove any existing 

### Install Required Dependencies
```sh
sudo apt update
sudo apt install build-essential cmake git libgtk2.0-dev pkg-config \
libavcodec-dev libavformat-dev libswscale-dev libjpeg-dev \
libpng-dev libtiff-dev libjasper-dev libv4l-dev libatlas-base-dev gfortran \
python3-dev
```

### Download OpenCV 4.10.0
```sh
cd ~
git clone https://github.com/opencv/opencv.git
cd opencv
git checkout 4.10.0
```

### OpenCV installations:
```sh
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j$(nproc)
sudo make install
```

## Step 2: YoloV11 Model Download and Conversion

- Download the YOLOV3 weights [here](https://drive.google.com/drive/folders/1hTzRYYRm9nEMRbxYX1bqpsGFVuv-hrrX?usp=sharing). 

- We have updated the UML Diagram in the Phase1 to correctly represent functionalities as per the product backlog 

![UML Phase 1](Monocular-Human-Detection-YOLO/Documentation/Phase1/UML_Phase1.png)

