/***********************************************************************************
2D array
(c) 2012, Hideyuki Tachibana (tachi-hi @ GitHub)

Usage: 

	operators:
	
		You can access each component by both
			X(i, j)
		and
			X[j][i]


************************************************************************************/


#pragma once

#include<vector>
#include<cassert>

template<typename T>
class TwoDimArray{

 public:
	TwoDimArray(const int nrow, const int ncol);
	virtual ~TwoDimArray();

	// get
	int get_ncol(void) const {return ncol;}
	int get_nrow(void) const {return nrow;}

	// operators
	virtual T* const operator[](int x) const;
	T& operator()(int x, int y);
	const T operator()(int x, int y) const;

	// + - / * 

	// iterator
	T* begin(void) const { return &( content[0] );}
	T* end(void)   const { return &( content[ ncol * nrow - 1] );}

	// apply
	template<typename S1, typename S2> friend TwoDimArray<S1> apply( S1(*func)(const S2), const TwoDimArray<S2>& tmp);

 protected:
	int ncol, nrow;
	std::vector<T> content;
	T** alias;
};

//-----------------------------------------------------------------------------
// Constructor and Destructor
//-----------------------------------------------------------------------------
template<typename T>
TwoDimArray<T>::TwoDimArray(const int nrow, const int ncol)
{
	this->ncol = ncol;
	this->nrow = nrow;
	content.resize( ncol * nrow );
	alias = new T* [ ncol ];
	for(int i = 0; i < ncol; i++){
		alias[i] = &(content[ nrow * i ]);
	}
}


template<typename T>
TwoDimArray<T>::~TwoDimArray()
{
	delete[] alias;
};


//-----------------------------------------------------------------------------
// operator
//-----------------------------------------------------------------------------

template<typename T>
T* const TwoDimArray<T>::operator[](int x) const {
	assert( x >= 0 && x < ncol );
	return alias[x];
}

template<typename T>
T& TwoDimArray<T>::operator()(int x, int y) {
	assert( x >= 0 && x < nrow );
	return (*this)[y][x];
}

template<typename T>
const T TwoDimArray<T>::operator()(int x, int y) const{
	assert( x >= 0 && x < nrow );
	return (*this)[y][x];
}

//-----------------------------------------------------------------------------
// apply
//-----------------------------------------------------------------------------
template<typename S, typename T>
TwoDimArray<S> apply( S(*func)(const T), const TwoDimArray<T>& x){
	TwoDimArray<S> ret( x.get_ncol(), x.get_nrow() );
	T* i = x.begin();
	S* j = ret.begin();
	while( i != x.end() )
		*j++ = func( *i++ );
	return ret;
}


