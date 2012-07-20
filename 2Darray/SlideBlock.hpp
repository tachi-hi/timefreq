/***********************************************************************************
2D FIFO Sliding Block Analysis
(c) 2008-2010, 2012, Hideyuki Tachibana (tachi-hi @ GitHub)

	2D FIFO data structure.

Usage: 

	operators:
	
		You can access each component by both
			X(i, j)
		and
			X[j][i]

	  operator()(int, int) is defined in the base class (TwoDimArray<T>)
	
	push data:
		sample code:		

			FIFO_SlideBlock<double> X(5, 3);
			double tmp[] = {0.1, 0.2, 0.3, 0.4, 0.5};
			X.push(tmp);

		This is equivalent to the following code:

			FIFO_SlideBlock<double> X(5, 3);
			double tmp[] = {0.1, 0.2, 0.3, 0.4, 0.5};
			for(int i = 0; i < 5; i++)
		 		X.new_frame_ptr()[i] = tmp[i];
			X.next_frame();


************************************************************************************/

#pragma once

#include<cassert>
#include<vector>
#include"TwoDimArray.hpp"

template<typename T>
class FIFO_SlideBlock: virtual public TwoDimArray<T>{
 public:

	FIFO_SlideBlock (int Freq_max, int Time_max);
	virtual ~FIFO_SlideBlock ();

	// operators
	T* const operator[] (int time) const;

	// get
	const int get_ncol(void)     const {return current_size;}
	const int get_max_ncol(void) const {return TwoDimArray<T>::get_ncol();}

	// push
	void push (T* in);
	T* const new_frame_ptr () const;
	void next_frame();

	// pop
	void pop (T *out);
	void kill_frame(void);

	// etc
	void rewind_frame(void);

 private:
	int current_size;
	T** alias;
};

//-----------------------------------------------------------------------------
// Constructor and Destructor
//-----------------------------------------------------------------------------
template<typename T>
FIFO_SlideBlock<T>::FIFO_SlideBlock(int k, int t)
	: TwoDimArray<T>(k, t)
{
	current_size = 0;
	alias = new T*[ TwoDimArray<T>::ncol ];
	for(int i = 0; i < TwoDimArray<T>::ncol; i++){
		alias[i] = TwoDimArray<T>::alias[ i ];
	}
}

template<typename T>
FIFO_SlideBlock<T>::~FIFO_SlideBlock(){
	delete[] alias;	
};


//-----------------------------------------------------------------------------
// operator
//-----------------------------------------------------------------------------
template<typename T>
T* const FIFO_SlideBlock<T>::operator[](int t) const {
	assert(t >= 0 && t < this->get_ncol());
	return alias[t];
}

//-----------------------------------------------------------------------------
// push
//-----------------------------------------------------------------------------
template<typename T>
T* const FIFO_SlideBlock<T>::new_frame_ptr() const {
	assert( current_size < TwoDimArray<T>::ncol );
	return alias[ current_size ];
}

template<typename T>
void FIFO_SlideBlock<T>::next_frame(){
	assert( current_size < TwoDimArray<T>::ncol  );
	current_size++;
}

template<typename T>
void FIFO_SlideBlock<T>::push(T* in){
	for(int j = 0; j < TwoDimArray<T>::nrow; j++){
		this->new_frame()[j] = in[j];
	}
	this->next_frame();
}

//-----------------------------------------------------------------------------
// pop
//-----------------------------------------------------------------------------
template<typename T>
void FIFO_SlideBlock<T>::kill_frame(void ){
	T* tmp = alias[ 0 ];
	for(int i = 0; i < TwoDimArray<T>::ncol; ++i){
		alias[ i ] = alias[ i + 1 ];
	}
	alias[ TwoDimArray<T>::ncol - 1 ] = tmp;
	--current_size;
}

template<typename T>
void FIFO_SlideBlock<T>::pop(T* out){
	for(int j = 0; j < TwoDimArray<T>::nrow; j++){
		out[j] = alias[ 0 ][j];
	}
	kill_frame();
}

//-----------------------------------------------------------------------------
// etc
//-----------------------------------------------------------------------------
template<typename T>
void FIFO_SlideBlock<T>::rewind_frame(void ){
	T* tmp = alias[ TwoDimArray<T>::ncol - 1 ];
	for(int i = TwoDimArray<T>::ncol - 1 ; i > 0; --i){
		alias[ i ] = alias[ i - 1 ];
	}
	alias[ 0 ] = tmp;
	++current_size;
}


