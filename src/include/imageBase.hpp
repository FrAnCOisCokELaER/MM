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

#ifndef __IMAGEBASE_H__
#define __IMAGEBASE_H__

template <class T>
class ImageBase {

public:
	ImageBase() { 
		m_W = 0; m_H = 0; m_D = 0;
		m_data.resize(0);
	}

	ImageBase(int w, int h, int d) {
		m_W = w; m_H = h; m_D = d;
		m_data.resize(w*h*d);
	}

	virtual  ~ImageBase() {
		m_data.clear();
	}

	void clearData() {

	}

	ImageBase(const ImageBase & image) {
		m_W = image.m_W;
		m_H = image.m_H;
		m_D = image.m_D;
		m_data = image.m_data;
	}

	std::vector<T> m_data;
	int m_W;
	int m_H;
	int m_D;
};


typedef ImageBase<unsigned char> IUCHAR;
typedef ImageBase<unsigned short> IUSHORT;
typedef ImageBase<float> IFLOAT;

#endif