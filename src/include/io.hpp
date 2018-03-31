/*
Copyright or � or Copr.Fran�ois COKELAER ([17/03/2018])

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

#ifndef __IO_H__
#define __IO_H__

#include <iostream>     
#include <fstream>     
#include <vector>
#include <exception>
#include "imageBase.hpp"

struct imageparams {
	int type;
	int w;
	int h;
	int d;
};

/* reader */
template <class T>
void writeImageData(const T *p_data,
	std::string filename,
	int nbElt
	) {
	std::ofstream outputStream;
	outputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::ofstream rawdata(filename.c_str(), std::ios::out | std::ios::binary);
	try {
		rawdata.write((char *)p_data, nbElt*sizeof(T));
		rawdata.close();
	}
	catch (const std::ifstream::failure& e) {
		throw e;
	}
}

/* writer */
template <class T>
void readImageData(	T *p_data,
					std::string filename,
					int nbElt) {	
	std::ifstream inputStream;
	inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		inputStream.open(filename.c_str(), std::ios::in | std::ios::binary);
		inputStream.read((char *)p_data, nbElt * sizeof(T));
		inputStream.close();
	}
	catch (const std::ifstream::failure& e) {
		throw e;
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

	//fill image params
	params.type = std::atoi(vStringParams[0].c_str());
	params.w = std::atoi(vStringParams[1].c_str());
	params.h = std::atoi(vStringParams[2].c_str());
	params.d = std::atoi(vStringParams[3].c_str());

	return params;
}

template<class T>
void writeData(std::string  imagePath, const ImageBase<T> & im ) {
	writeImageData<T>(im.m_data.data(), imagePath, im.m_W*im.m_H*im.m_D);
}

/**/
template<class T> 
ImageBase<T>  readData(std::string  imagePath) {
	imageparams params = parseImagePath(imagePath);
	ImageBase<T> im(params.w, params.h, params.d);
	readImageData<T>(im.m_data.data(),
		imagePath,
		params.w*params.h*params.d);
	return im;
}

#endif