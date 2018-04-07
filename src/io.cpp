#include "io.hpp"
#include <iostream>
#include <string>
#include <exception>

bool checkinputs(const imageparams & params, int choice) {

	bool checker = true;

	//check on image size 
	switch (choice) {
	case 1:
	case 2:
		if (params.d != 1) { std::cout << "Stack detected, 2D algorithm will process image as a stack of 2D images" << std::endl; }
		break;
	case 3:
	case 4:
		if (params.d < 32) { std::cout << "Please specify a 32x32x32 3D image at least"; checker = false; }
		break;
	default: { std::cout << " No supported choice has been made , you will be asked to redo ..." << std::endl; checker = false; }
		break;
	}

	if (params.type != 1)
	{
		std::cout << " Only 1 byte i.e. unsigned char image format is support for now, please convert your input." << std::endl; checker = false;
	}


	return checker;
}

void promptMenu(	std::string & imagePath,
					ImageBase<unsigned char>  & im,
					int & choice,
					int & L,
					int & G,
					int & K,
					int & reconstruction) {

	std::cout << " /******************************************************************/" << std::endl;
	std::cout << " N-Dimensionnal Path Openings and Robust Path Openings computation : " << std::endl;
	std::cout << " F.Cokelaer source code under CeCILL - B license " << std::endl;
	std::cout << " Published in: IEEE Journal of Selected Topics in Signal Processing ( Volume: 6, Issue: 7, Nov. 2012 ) " << std::endl;
	std::cout << " /******************************************************************/" << std::endl;
	std::cout << std::endl;
	std::cout << " Data samples available into ./data " << std::endl;
	std::cout << std::endl;
	std::cout << " /******************************************************************/" << std::endl;
	bool validimage = false;
	while (!validimage) {

		std::cout << " Please enter image path " << std::endl;
		std::cout << " /******************************************************************/" << std::endl;
		//std::string imagePath;
		std::cin >> imagePath;
		imageparams params;
		//get image params and check ...
		try{
			params = parseImagePath(imagePath);
			im = readData<unsigned char>(imagePath);

			std::cout << std::endl;
			std::cout << " Please select the algorithm : " << std::endl;
			std::cout << "(1) - 2D Path Openings /" << std::endl;
			std::cout << "(2) - 2D Robust Path Openings / " << std::endl;
			std::cout << "(3) - 3D Path Openings / " << std::endl;
			std::cout << "(4) - 3D Robust Path Openings" << std::endl;
			std::cout << " /******************************************************************/" << std::endl;
			//int choice;
			std::cin >> choice;
			std::cout << std::endl;

			validimage = checkinputs(params, choice);
		}
		catch (const std::runtime_error & err) {
			
			std::cout << err.what() << " There is an error while opening image file, ... , retry ..." << std::endl;
		}


	}

	std::cout << " /******************************************************************/" << std::endl;
	std::cout << " Please enter algorithm parameters :" << std::endl;
	std::cout << " The filering length  :" << std::endl;
	std::cout << " /******************************************************************/" << std::endl;

	//int L;
	std::cin >> L;
	while (L < 2) {
		std::cout << " L should be strictly greater than 1, please enter L :" << std::endl;
		std::cin >> L;
	}

	//int G, reconstruction;
	if (choice == 2 || choice == 4) {
		std::cout << " For Robust path the allowed gap G:" << std::endl;
		std::cout << " /******************************************************************/" << std::endl;
		std::cin >> G;
		while (G < 1) {
			std::cout << " G should be strictly greater than 0, please enter G :" << std::endl;
			std::cin >> G;
		}
		std::cout << " Perform geodesic reconstruction of the gaps :  0 (no), 1 (yes):" << std::endl;
		std::cout << " /******************************************************************/" << std::endl;
		std::cin >> reconstruction;
		while (!(reconstruction == 1 || reconstruction == 0)) {
			std::cout << " Reconstruction param should be either equal to 0 or 1 :" << std::endl;
			std::cin >> reconstruction;
		}
	}
}


/*
Parse Image Path  and return a struct containing size and bytes params
*/
imageparams parseImagePath(std::string imagePath) {
	//filter out system path
	imagePath.substr(0, imagePath.find_last_of("\\/"));

	//find params : image format must be // imagename-"nbytes"-W-H-D
	imageparams params;
	std::string delimiter = "-";
	std::string stringParams = imagePath.substr(imagePath.find_first_of('-') + 1); //nbytes-W-H-D

	size_t pos = 0;
	std::vector<std::string> vStringParams;

	while (((pos = stringParams.find(delimiter)) != std::string::npos)) {
		vStringParams.push_back(stringParams.substr(0, pos));
		stringParams.erase(0, pos + delimiter.length());
	}
	vStringParams.push_back(stringParams);

	if (vStringParams.size() != 4)
		throw  std::runtime_error("Cannot parse image path, please check it and retry ");
	
	//fill image params
	params.type = std::atoi(vStringParams[0].c_str());
	params.w = std::atoi(vStringParams[1].c_str());
	params.h = std::atoi(vStringParams[2].c_str());
	params.d = std::atoi(vStringParams[3].c_str());

	if (params.w < 32 || params.h < 32 || params.d < 1)
		throw  std::runtime_error("Please check image dimensions , should be at least 32 x 32 x 1");

	return params;
}

