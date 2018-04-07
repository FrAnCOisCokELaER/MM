# MM
This repository contains the source code of path based mathematical morphology tools for 2D and 3D images. 

(1)-2D complete path operators  => filter out features whose length < L

(2)-2D robuts path operators  => idem as complete path but allow gaps into the path : G={1,2,3}

(3)-3D complete path operators

(4)-3D robust path operators

Algorithms run on 4 and 7 orientations for 2D and 3D respectively, orientations are parallelized by using openMP.

****
- Clone the repository

git clone https://github.com/FrAnCOisCokELaER/MM.git

****
- Build 

Supported on win/unix, ensure that your compiler support C++11 features.

Run cmake-gui


Click on "Configure" and select your generator

Click on "Generate"

Open the solution for windows and build or "make" on Linux

-Note that there are no need to link with other libs, app is self contained.

****
- Bin also available into /build/Release

binaries for win compiled with  VS2015 x64 ( redist : https://www.microsoft.com/fr-fr/download/details.aspx?id=48145 )


****
- Data Samples into /data

 2D and 3D image are available into raw format in order to test both 2D and 3D algorithm.
In order to test on your own images, just convert to unsigned char raw format and encode image information into image path :
- e.g. myimage-x-x-x-x.raw    [-NB_OF_BYTES-NB_ROWS-NB_COLS-NB_FRAMES]

Read/Write simply with imageJ into File->Import->Raw / File->Save As ->raw
````
https://imagej.nih.gov/ij/download.html
````
****
- Execution : run Path_Operators.exe 

````
D:\work\Git\MM\build\Release>Path_Operators.exe
 /******************************************************************/
 N-Dimensionnal Path Openings and Robust Path Openings computation :
 F.Cokelaer source code under CeCILL - B license
 Published in: IEEE Journal of Selected Topics in Signal Processing ( Volume: 6, Issue: 7, Nov. 2012 )
 /******************************************************************/

 Data samples available into ./data

 /******************************************************************/
 Please enter image path
 /******************************************************************/
d:\work\git\mm\data\neuron-1-512-512-1.raw

 Please select the algorithm :
(1) - 2D Path Openings /
(2) - 2D Robust Path Openings /
(3) - 3D Path Openings /
(4) - 3D Robust Path Openings
 /******************************************************************/
2

 /******************************************************************/
 Please enter algorithm parameters :
 The filering length  :
 /******************************************************************/
40
 For Robust path the allowed gap G:
 /******************************************************************/
1
 Perform geodesic reconstruction of the gaps :  0 (no), 1 (yes):
 /******************************************************************/
0
 2D ROBUST PATH OPENINGS UNION ON 4 ORIENTATIONS : L = 40 G = 1
orientation4 1 -1: passed
orientation1 1 0  : passed
orientation2 0 1  : passed
orientation3 1 1 : passed

308.358 ms
Result written in : d:\work\git\mm\data\neuron-1-512-512-1.raw_resRPO2D-L-40-G-1-recons-0.raw
Appuyez sur une touche pour continuer...

````




