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

bool checkinputs(const imageparams & params, int choice);

void promptMenu(	std::string & imagePath,
					ImageBase<unsigned char> & im,
					int & choice,
					int & L,
					int & G,
					int & K,
					int & reconstruction);

imageparams parseImagePath(std::string imagePath);

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




template<class T>
void writeData(std::string  imagePath, const ImageBase<T> & im ) {
	writeImageData<T>(im.m_data.data(), imagePath, im.m_W*im.m_H*im.m_D);
}

/**/
template<class T> 
ImageBase<T>  readData(std::string  imagePath) {
	imageparams params = parseImagePath(imagePath);
	ImageBase<T> im(params.w, params.h, params.d);
	bool readOK = false;
	while (!readOK) {
		try {
			readImageData<T>(im.m_data.data(), imagePath, params.w*params.h*params.d);
			readOK = true;
		}
		catch (const std::ifstream::failure& e) {
			std::cout << e.what() << std::endl;
			std::cout << "Wrong image path please , re-enter a valid image path :" << std::endl;
			std::cin >> imagePath;
		}
	}
	return im;
}


#endif