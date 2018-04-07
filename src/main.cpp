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
#include "PO2D.hpp"
#include "PO3D.hpp"

int main(int argc, char *argv[])
{

	std::string imagePath;
	int choice, L, G, K, reconstruction;

	/*
	* Prompt Menu for User, load data
	*/
	IUCHAR input;
	promptMenu(	imagePath,
				input,
				choice,
				L,
				G,
				K,
				reconstruction);


	//init output
	IUCHAR output = input;

	/*
	* timing start
	*/
	auto start = std::chrono::steady_clock::now();
	

	std::string  outputPath;
	switch (choice) {
		case 1:
		{
			  for (int k = 0; k < input.m_D; ++k) 
				  PO2D::UNION_PO2D<unsigned char>(	&(input.m_data.data()[k*input.m_W*input.m_H]),
													&(output.m_data.data()[k*input.m_W*input.m_H]), 
													L, input.m_W, input.m_H);
			  
				outputPath = imagePath + "_resPO2D-L-" + std::to_string(L) + ".raw";
				
		}
			break;
		case 2:
		{
				  for (int k = 0; k < input.m_D; ++k)
					  PO2D::UNION_RPO2D<unsigned char>(	 &(input.m_data.data()[k*input.m_W*input.m_H]),
														 &(output.m_data.data()[k*input.m_W*input.m_H]), 
														 L, G, reconstruction, input.m_W, input.m_H);

				  outputPath = imagePath + "_resRPO2D-L-" + std::to_string(L) + "-G-" + std::to_string(G) + "-recons-" + std::to_string(reconstruction) + ".raw";
		}
			break;
		case 3: 
				PO3D::UNION_PO3D<unsigned char>(input.m_data.data(), output.m_data.data(), L, input.m_W, input.m_H, input.m_D);
				outputPath = imagePath + "_resPO3D-L-" + std::to_string(L) + ".raw", output;
				break;
		case 4 :
				PO3D::UNION_RPO3D<unsigned char>(input.m_data.data(), output.m_data.data(), L, G, reconstruction, input.m_W, input.m_H, input.m_D);
				outputPath = imagePath + "_resRPO3D-L-" + std::to_string(L) + "-G-" + std::to_string(G) +"-recons-" + std::to_string(reconstruction) + ".raw";
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

	writeData<unsigned char>(outputPath, output);
	std::cout << "Result written in : " << outputPath << std::endl;


	system("PAUSE");
	return 0;
}
