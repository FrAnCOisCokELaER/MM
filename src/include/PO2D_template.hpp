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

#ifndef __PO2D_HPP__
#define __PO2D_HPP__

#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <cstdlib>

#define max(a,b) ((a > b) ? a : b)


/******create neighbour pixel's index from a given orientation vector ********/
void createNeighbourhood(		std::vector<int> const & orientation,
								std::vector<int> & upstreamN,
								std::vector<int> & downstreamN,
								int nb_col_padded
						)
 {
                                
    int y_shift = orientation[0];
    int x_shift = orientation[1];                                   

    //vertical orientation
    if((y_shift == 1 && x_shift == 0) || (y_shift == -1 && x_shift == 0) ) {
    
       upstreamN.push_back(nb_col_padded +1);
       upstreamN.push_back(nb_col_padded -1);
       upstreamN.push_back(nb_col_padded);
       
       downstreamN.push_back(-(nb_col_padded + 1));
       downstreamN.push_back(-nb_col_padded + 1);
       downstreamN.push_back(-nb_col_padded);
    }
    //horizontal orientation
    if((y_shift==0 && x_shift == 1) || (y_shift==0 && x_shift == -1)) {
    
       upstreamN.push_back(nb_col_padded + 1);
       upstreamN.push_back(-nb_col_padded + 1);
       upstreamN.push_back(1);
       
       downstreamN.push_back(-nb_col_padded - 1);
       downstreamN.push_back(nb_col_padded - 1);
       downstreamN.push_back(-1);
    }
    //1st diagonal
    if((y_shift == 1 && x_shift == 1) || (y_shift == -1 && x_shift == -1)) {

       upstreamN.push_back(nb_col_padded);
       upstreamN.push_back(1);
       upstreamN.push_back(nb_col_padded + 1);

       downstreamN.push_back(-nb_col_padded);
       downstreamN.push_back(-1);
       downstreamN.push_back(-nb_col_padded - 1);
    }
    //2nd diagonal
    if((y_shift == 1 && x_shift == -1) || (y_shift == -1 && x_shift == 1)) {

       upstreamN.push_back(-1);
       upstreamN.push_back(nb_col_padded);
       upstreamN.push_back(nb_col_padded - 1);
       downstreamN.push_back(1);
       downstreamN.push_back(-nb_col_padded);
       downstreamN.push_back(-nb_col_padded+1);
    }

}
template <class T>
bool pointer_value_comparison(const T *a, const T *b)
{

        return ( (*a) < (*b));
}
//sorting pixel grey level value of input image by ascending order
template <class T>
void sort_image_value(	std::vector<T> &originalI,
                  		int image_size,
                  		std::vector<int> &im_idx_sort)
{
        //allocate memory for sorted pointers array
        std::vector<T*> sorted_ptr(image_size);
        //fill sorted pointers array
        for (int i=0; i<image_size; ++i)
            sorted_ptr[i] = &originalI[0] + i;

        //sorting ptr's int value
		std::sort(sorted_ptr.begin(), sorted_ptr.end(), pointer_value_comparison<T>);
        //associated image indices to sorted ptr value
        for (int i =0;i<image_size;++i)
        	im_idx_sort[i] = (int) (sorted_ptr[i] - &originalI[0]);

}
//path propagation
inline void propagatePath(	int seed_index,
	 						std::vector<int> & downstreamI,
	 						std::vector<int> & flagI,
	 						std::vector<int> const & upstreamN,
	 						std::vector<int> const & downstreamN,
	 						std::queue<int> & queueQ,
	 						std::queue<int> & queueC
	 						)
{
    //max propagation length
    int max_lenght;
	int curr_upstreamN_idx;
    //init Qq with all active upstream neighbours
	for(int i = 0; i < (int) upstreamN.size(); ++i) {
    	curr_upstreamN_idx = seed_index + upstreamN[i];
        // if active and not in queueQ
        if ( flagI[curr_upstreamN_idx]&(1<<0) &&
        	!(flagI[curr_upstreamN_idx]&(1<<1))) {
        		//put in Qc
                queueQ.push(curr_upstreamN_idx);
                //set corresponding flag to 1
                flagI[curr_upstreamN_idx] |= (1<<1);
            }
 	}

	int curr_downstreamN_idx;
	int prop_upstreamN_idx;
    while(!queueQ.empty()) {
		//get first element of queueC
        curr_upstreamN_idx = queueQ.front();
        max_lenght = -1;
        for (int i = 0; i < (int) downstreamN.size(); ++i) {
        	//get downstream neighbours index
        	curr_downstreamN_idx = curr_upstreamN_idx + downstreamN[i];
        	//get maximum downstream length
        	if (downstreamI[curr_downstreamN_idx] > max_lenght) {
        		max_lenght = downstreamI[curr_downstreamN_idx ] ;
        	}
        }
        //check changes in lenght
        if ( (max_lenght + 1) <  downstreamI[curr_upstreamN_idx] ) {
        	//update length
        	downstreamI[curr_upstreamN_idx] =  max_lenght + 1;
        	//propagate the path
        	//here the upstream pixels are pushed into queueQ_temp
            for(int i = 0; i < (int) upstreamN.size(); ++i) {
            	prop_upstreamN_idx = curr_upstreamN_idx + upstreamN[i];
                if(flagI[prop_upstreamN_idx ]&(1<<0) &&
                !(flagI[prop_upstreamN_idx ]&(1<<1)) ) {
                	queueQ.push(prop_upstreamN_idx);
                	flagI[prop_upstreamN_idx ] |= (1<<1);
                }
            }
         	//enqueue current neighbour in Qc
         	if(!(flagI[curr_upstreamN_idx]&(1<<2))) {
         		queueC.push(curr_upstreamN_idx);
            	flagI[curr_upstreamN_idx] |= (1<<2);
         	}
         }
         //pop first element of queueQ
         queueQ.pop();
         //set corresponding queueQ flag  to 0
         flagI[curr_upstreamN_idx] &= ~(1<<1);
     } //end while   
 }
//processing function
template <class T>
void PO2D(		   T *input_buffer,
				   T *output_buffer,
				   std::vector<int> & orientation,
				   int L,
				   int dimx,
				   int dimy
		   )	  
{	

	/**padding**/
	int nb_col_padded = dimx+4;
	int nb_row_padded = dimy+4;
	int image_size = nb_col_padded*nb_row_padded;

	/*****temporay images initialisation***********/
	std::vector<T> originalI;
	std::vector<int> downstreamI;
	std::vector<int> upstreamI;
	std::vector<int> flagI;
	std::vector<int> sorted_im_idxI;
	
	originalI.resize(image_size,0);
	downstreamI.resize(image_size,0);
	upstreamI.resize(image_size,0);
	sorted_im_idxI.resize(image_size);	

	//flag image manage the threshold decomposition,
	//the enqueuing process in Qq and Qc and also
	//prevent deactivated pixels durin path propagation
	//from being considered as a seed further in the program
	flagI.resize(image_size,0);
	/***********flags description***************/
	/* in flagI, 3 flags are used in this implementation :
	/*         1            0
	flag 0 : active / desactive
	flag 1 : in queueQ / not in queueQ
	flag 2 : in queueC / not in queueC
	flag 3 : desactivated during propagation / not desactivated during propagation
	*/
	/*FIFO queue handling pixels during propagation*/
	std::queue<int> queueQ;
	/*FIFO queue handling pixels that are updated during the propagation*/
	std::queue<int> queueC;

	/**vectors containing image memory address 
	offset corresponding to a given orientation ****/ 
	std::vector<int> upstreamN;
	std::vector<int> downstreamN;
	//create neighbourhood memory address offset 
	//according to a given orientation vector
	createNeighbourhood(	orientation,
							upstreamN,
							downstreamN,
							nb_col_padded);
	
	
	//get input image values
	//initialize downstream and upstream length image
	for(int j=0; j<dimy; j++) {
		for (int i=0; i<dimx; i++) {
			originalI[(dimx+4)*(j+2) + (i+2)] = input_buffer[ dimx*(j) + (i)];
			flagI[(dimx+4)*(j+2) + (i+2)] = 1;	
			downstreamI[(dimx+4)*(j+2) + (i+2)] = L;	
			upstreamI[(dimx+4)*(j+2) + (i+2)] = L;	
		}
	}
	//get image memory addresses sorted by their
	//corresponding original image value
    sort_image_value<T>(	originalI,
                  		image_size,
                  		sorted_im_idxI);
    					
    //seed pixel idx used for path propagation
    int seed_index;
    //index  value
    int iii=0;
    int iiii=0;

    //seed pixel value
    T threshold;
    double temp_threshold = -1.0;
    //longest path through a pixel
    int longest_path = 0;
    //pixel index in queueC
    int in_queueC;
    int temp_seed_index;
    //pixel coordinate in row and col
    int px_line_idx;
    int px_col_idx;
	//
    //scanning entire image
    while(iii<image_size) {
        //get seed pixel index and coordinate
        seed_index = sorted_im_idxI[iii];
        px_line_idx = (seed_index/nb_col_padded);
        px_col_idx = (seed_index %nb_col_padded);
        //propagation is allowed if not in the border and if
        //was not desactivated during the propagation of the path
        if( !(flagI[seed_index]&(1<<3)) &&
        	 px_line_idx > 0 &&
             px_line_idx <(nb_row_padded-1) &&
             px_col_idx > 0 &&
             px_col_idx <(nb_col_padded-1)) {
             
        	//get curr seed pixel value
        	threshold =  originalI[seed_index];
        	//desactivate every pixel active and which value <= threshold
        	if(temp_threshold < threshold) {
        		//
        		if((!queueC.empty()))
        		{
					//check queueC if total lenght trought pixel fell down to L
					while(!queueC.empty()) {
						in_queueC = queueC.front();
					  	//compute lenght
					   	longest_path = upstreamI[in_queueC] +
						            downstreamI[in_queueC] - 1;
					   	//check longest path
					   	if(longest_path < L) {
						 	//write into output image ( not be part of a L path at higher level)
						 	originalI[in_queueC] = temp_threshold;
						 	//desactivate pixel
						 	flagI[in_queueC] &= ~(1<<0);
						 	//set his downstream and upstream lenght to 0
						 	upstreamI[in_queueC] = 0;
						 	downstreamI[in_queueC] = 0;
						 	//canno't be seed at next threshold
						 	flagI[in_queueC] |= (1<<3);
					   	}
						//set flag to 0
						flagI[in_queueC] &= ~(1<<2);
						//set flag 10 to 0
			  			queueC.pop();      			
			  		}
        		}//end queueC is not empty
        		//
        		iiii=iii;
            	temp_seed_index = sorted_im_idxI[iiii];
            	while(originalI[temp_seed_index]<= threshold && iiii<image_size) {
                	if(flagI[temp_seed_index]&(1<<0)) {
                    	//set to inactive
                    	flagI[temp_seed_index] &= ~(1<<0);
                    	//set downstream and upstream lenght to 0
                    	upstreamI[temp_seed_index] = 0;
                    	downstreamI[temp_seed_index] = 0;
                	}
                	++iiii;
                	//update temp_pixel_coord
                	if ( iiii < image_size) {
                    	temp_seed_index = sorted_im_idxI[iiii];
                	}
                	if (iiii == image_size)
                    	break;
            	}			      
            	temp_threshold = threshold;
        	}
        	//path propagation
        	//update downstreamI changes
	  	 	propagatePath(	seed_index,
	 						downstreamI,
	 						flagI,
	 						upstreamN,
	 						downstreamN,
	 						queueQ,
	 						queueC
	 					 );
	 		//update upstreamI changes
	    	propagatePath(	seed_index,
	 						upstreamI,
	 						flagI,
	 						downstreamN,
	 						upstreamN,
	 						queueQ,
	 						queueC
	 							 );
                 
         ++iii;
        } else {
           ++iii;
        }     
    }
    
	//copy result image
	for(int j=0; j<dimy; j++) {
		for (int i=0; i<dimx; i++) {
			output_buffer[ dimx*(j) + (i)] = originalI[(dimx+4)*(j+2) + (i+2)];
		}
	}
}

//performing the union of RPO3D on four orientations
template <class T>
void UNION_PO2D(		T *input_buffer,
						T *output_buffer,
						int L,
						int dimx,
						int dimy)
{
	std::cout<<" 2D COMPLETE PATH OPENINGS UNION ON 4 ORIENTATIONS : L = "<<L<<std::endl;
	//allocate result memory
	T* res1= new T[	dimx*dimy];
	T* res2= new T[	dimx*dimy];
	T* res3= new T[	dimx*dimy];
	T* res4= new T[	dimx*dimy];


	// orientation vector
	std::vector<int> orientation1(2);
	orientation1[0] = 1;
	orientation1[1] = 0;
	std::vector<int> orientation2(2);
	orientation2[0] = 0;
	orientation2[1] = 1;
	std::vector<int> orientation3(2);
	orientation3[0] = 1;
	orientation3[1] = 1;
	std::vector<int> orientation4(2);
	orientation4[0] = 1;
	orientation4[1] = -1;

	//
	//calling RPO
	 #pragma omp parallel sections 
	 {
		#pragma omp section 
	   { 
		 PO2D<T>(input_buffer,res1,orientation1,L,dimx,dimy);
		 std::cout<<"orientation1 1 0  : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
		   PO2D<T>( input_buffer,res2,orientation2,L,dimx,dimy);
		   std::cout<<"orientation2 0 1  : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       PO2D<T>( input_buffer,res3,orientation3,L,dimx,dimy);
		   std::cout<<"orientation3 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
			
	       PO2D<T>( input_buffer,res4,orientation4,L,dimx,dimy);
		   std::cout<<"orientation4 1 -1: passed"<<std::endl;
	   }
	 }
	
	 //Union
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy;i++)
		output_buffer[i]=res1[i];
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy;i++)
		output_buffer[i]=max(res2[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy;i++)
		output_buffer[i]=max(res3[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy;i++)
		output_buffer[i]=max(res4[i],output_buffer[i]);
	//
	//desallocation
	delete[] res1;
	delete[] res2;
	delete[] res3;
	delete[] res4;
}


#endif
