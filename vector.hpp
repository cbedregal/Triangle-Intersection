#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "point.hpp"

// a class representing an n-dimensional vector

template <std::size_t n, typename value_type>
struct vector : public point<n,value_type>
{
	// vector_scalar() : returns the scalar multiplication of vector pa and scalar m 

	vector vector_scalar(const value_type &m)
	{
		value_type values[n];
		value_type *pointer_pa = begin();
		
		for(int i=0; i<n; pointer_pa[i++]*=m);
		
		return *this;
	}


	// vector_diff() : returns the difference between vectors pa and pb 

	vector& operator-= (const vector &pb)
	{
		value_type values[n];
		value_type *pointer_pa = begin();
		const value_type *pointer_pb = pb.begin();
		
		for(int i=0; i<n; pointer_pa[i++]-=pointer_pb[i]);
		
		return *this;
	}


	// vector_sum() : returns the addition between vectors pa and pb

	vector vector_sum(const vector &pa, const vector &pb)
	{
		value_type values[n];
		const value_type *pointer_pa = pa.begin();
		const value_type *pointer_pb = pb.begin();
		
		for(int i=0; i<n; values[i++]=*(pointer_pa++)+*(pointer_pb++));
		
		return vector(&values[0], &values[3]);
	}


	// vector_dot() : returns the dot product of vectors pa and pb

	double vector_dot(const vector &pa, const vector &pb)
	{
		double dot_product=0.0;
		const value_type *pointer_pa = pa.begin();
		const value_type *pointer_pb = pb.begin();
		
		for(int i=0; i<n; i++)
			dot_product+=pointer_pa[i]*pointer_pb[i];
		
		return dot_product;
	}


	// vector_cross() : returns the cross product of vectors pa and pb

	vector vector_cross(const vector &pa, const vector &pb)
	{
		value_type values[3];
		const value_type *pointer_pa = pa.begin();
		const value_type *pointer_pb = pb.begin();
		
		values[0]=pointer_pa[1]*pointer_pb[2]-pointer_pa[2]*pointer_pb[1];
		values[1]=pointer_pa[2]*pointer_pb[0]-pointer_pa[0]*pointer_pb[2];
		values[2]=pointer_pa[0]*pointer_pb[1]-pointer_pa[1]*pointer_pb[0];
		
		return vector(&values[0], &values[3]);
	}
};
#endif