# MM
This repository contains the source code of path based mathematical morphology tools for 2D and 3D images. 
(1)-2D complete path operators  => filter out features whose length < L
(2)-2D robuts path operators  => idem as complete path but allow gaps into the path : G={1,2,3}
(3)-3D complete path operators
(4)-3D robust path operators

Algorithm are runned on 4 and 7 orientations for 2D and 3D respectively, orientations are parallelized by using openMP.

****
- Clone the repository

git clone https://github.com/FrAnCOisCokELaER/MM.git

****
- Build 

Supported on win/unix/macos, ensure that your compiler support C++11 features.

Run cmake-gui
Click on "Configure" and select your generator
Click on "Generate"
Open the solution for windows and build or "make" on Linux

-Note that there are no need to link with other libs, app is self contained.

****
- Binaries also available into /bin

binaries for win compiled with  VS2015 x64 ( redist : https://www.microsoft.com/fr-fr/download/details.aspx?id=48145 )


****
- Data Samples into /data

 2D and 3D image are available into raw format in order to test both 2D and 3D algorithm.
In order to test on your own images, just convert to unsigned char raw format and encode image information into image path :
- e.g. myimage-x-x-x-x.raw    [-NB_OF_BYTES-NB_ROWS-NB_COLS-NB_FRAMES-]


