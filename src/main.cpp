/*
Copyright or © or Copr.François COKELAER ([17/03/2018])

francois.cokelaer@gmail.com

This software is a computer program whose purpose is to[describe
functionalities and technical features of your software].

This software is governed by the CeCILL - B license under French law and
abiding by the rules of distribution of free software.You can  use,
modify and / or redistribute the software under the terms of the CeCILL - B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights, and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading, using, modifying and / or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate, and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in - depth computer knowledge.Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and / or
data to be ensured and, more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL - B license and that you accept its terms.
*/

#include <iostream>
#include <string>
#include <exception>
#include <chrono>
#include "io.hpp"
#include "imageBase.hpp"
#include "RPO2D_template.hpp"
#include "RPO_update_3D.hpp"


bool checkinputs(const imageparams & params, int choice) {

	bool checker = true;
	
	//check on image size 
	switch (choice) {
		case 1:
		case 2: 
		case 4:
			//if (params.d != 1) { std::cout << "Wrong image depth dimension, please select a 2D image" << std::endl; checker = false; }
			if (params.w < 32 || params.h < 32) { std::cout << "Please specify a 32x32 2D image at least";}
			break;
		case 3:
			if (params.w < 32 || params.h < 32 || params.w < 32) { std::cout << "Please specify a 32x32x32 3D image at least"; }
			break;
		default: { std::cout << " No supported choice has been made , you will be asked to redo ..." << std::endl; checker = false; }
			break;
	}
	return checker;
}

void promptMenu(	std::string & imagePath,
					int & choice,
					int & L,
					int & G,
					int & K,
					int & reconstruction) {

	std::cout << " /******************************************************************/" << std::endl;
	std::cout<<" N-Dimensionnal Path Openings and Robust Path Openings computation : "<<std::endl;
	std::cout<<" F.Cokelaer source code under CeCILL - B license "<<std::endl;
	std::cout << " Published in: IEEE Journal of Selected Topics in Signal Processing ( Volume: 6, Issue: 7, Nov. 2012 ) " << std::endl;
	std::cout << " /******************************************************************/" << std::endl;
	std::cout << std::endl;
	std::cout<<" Data samples available into ./data "<<std::endl;
	std::cout << std::endl;
	std::cout << " /******************************************************************/" << std::endl;
	std::cout << std::endl;
	std::cout << " Please select the algorithm : " << std::endl;
	std::cout << "(1) - 2D Path Openings / (2) - 2D Robust Path Openings / (3) - 3D Path Openings / (4) - 3D Robust Path Openings"  << std::endl;
	std::cout << " /******************************************************************/" << std::endl;
	//int choice;
	std::cin>>choice;
	std::cout<< " Please enter image path " << std::endl;
	std::cout << " /******************************************************************/" << std::endl;
	//std::string imagePath;
	std::cin >> imagePath;
	imageparams params = parseImagePath(imagePath);

	while (!checkinputs(params, choice)) {

		std::cout << "(1) - 2D Path Openings / (2) - 2D Robust Path Openings / (3) - 3D Path Openings / (4) - 3D Robust Path Openings"  << std::endl;
		std::cout << " /******************************************************************/" << std::endl;
		std::cin >> choice;

		std::cout << " Please enter new image path " << std::endl;
		std::cout << " /******************************************************************/" << std::endl;
		std::string imagePath;
		std::cin >> imagePath;
		
		//get image params and check ...
		params = parseImagePath(imagePath);
	}
	std::cout << std::endl;
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

int main (int argc, char *argv[])
{

	std::string imagePath;
	int choice, L, G, K, reconstruction;

	/*
	* Prompt Menu for User
	*/
	promptMenu(		imagePath,
					choice,
					L,
					G,
					K,
					reconstruction);

	/*
	* Load image from input path : 
	*/

	IUCHAR input = readData<unsigned char>(imagePath);
	IUCHAR output = input;

	/*
	* timing start
	*/
	auto start = std::chrono::steady_clock::now();
	
	switch (choice) {
		case 1: 
				nsPO2D::UNION_PO2D<unsigned char>(input.m_data.data(), output.m_data.data(), L, input.m_W, input.m_H);
				writeData<unsigned char>(imagePath + "_resPO2D-L-" + std::to_string(L) + ".raw", output);
	
			break;
		case 2:
			nsPO2D::UNION_RPO2D<unsigned char>(input.m_data.data(), output.m_data.data(), L, G, reconstruction, input.m_W, input.m_H);
			writeData<unsigned char>(	imagePath + "_resRPO2D-L-" + 
										std::to_string(L) + "-G-"+std::to_string(G) + 
										"-recons-" + std::to_string(reconstruction)+ ".raw", output);
			break;
		case 3: 
					PO3D::UNION_PO3D<unsigned char>(input.m_data.data(), output.m_data.data(), L, input.m_W, input.m_H, input.m_D);
					writeData<unsigned char>(imagePath + "_resPO3D-L-" + std::to_string(L) + ".raw", output);
					break;
		case 4 :
			PO3D::UNION_RPO3D<unsigned char>(input.m_data.data(), output.m_data.data(), L, G, reconstruction, input.m_W, input.m_H, input.m_D);
			writeData<unsigned char>(imagePath + "_resRPO3D-L-" +
				std::to_string(L) + "-G-" + std::to_string(G) +
				"-recons-" + std::to_string(reconstruction) + ".raw", output);
			break;
		default:
			throw new std::runtime_error("Algorithm choise : Should not happen");

	}

	/*
	* timing stop
	*/
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout<<std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;

	system("PAUSE");
	return 0;
}
