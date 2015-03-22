#ifndef INTERSECT_HPP
#define INTERSECT_HPP

#include "simplex.hpp"

#define EPSILON 0.00000001

// returns true iff the intersection of the interiors of triangle_a and triangle_b is not empty

template <typename value_type>
bool intersect(const simplex<2, 3, value_type> &triangle_a, const simplex<2, 3, value_type> &triangle_b)
{
	// check if triangle_a lies on one side of the plane defined by triangle_b
	// perform the same check with triangle_b and the plane defined by triangle_a
	if(simplex_on_side(triangle_a,triangle_b) || simplex_on_side(triangle_b,triangle_a))
		return false;
	
	// check if the edges of triangle_a intersect the interior of triangle_b
	// perform the same check with the edges of triangle_b and triangle_a 
	if (MT_intersect_edges(triangle_a,triangle_b) || MT_intersect_edges(triangle_b,triangle_a))	
		return true;
	
	// even though the triangles intersect each others plane, their edges do not intersect the triangles' interiors
	return false;	
}


// MT_intersect_edges(): returns true iff an edge of triangle t1 intersects the interior of t2

template <typename value_type>
bool MT_intersect_edges(const simplex<2, 3, value_type> &t1, const simplex<2, 3, value_type> &t2)
{

	// first, calculate the distance from each vertex of t1 to the plane of t2
	// (the sign of this distance defines the side of the plane in which each point lies)

	point<3,value_type> normal_t2 = plane_normal(t2);
	const point<3, value_type> *vertices_t1 = t1.begin();		
	double d_t2 = -vector_dot(normal_t2,*t2.begin());			
	double d_t1[3];
	int v0, v1, v2;
	
	for(int i=0; i<3; d_t1[i++] = vector_dot(normal_t2, vertices_t1[i])+d_t2);
		
	// if t1 lies on the same plane, it must be evaluated by a 2D-intersection algorithm
	// the same for the case when a corner touches the plane or an edge lieson the plane
	if(d_t1[0]==0.0 && d_t1[1]==0.0 && d_t1[2]==0.0){	
			return false;
	}
	
	// next, find the vertex of t1 shared by the edges crossing the plane
	// so that v0 lies on one side of the plane and v1, v2 lie on the other
	
	// check if a vertex of t1 lies on the plane (v0) and the other two lie on opposite sides (v1,v2)
	if(d_t1[0]*d_t1[1]*d_t1[2]==0.0){					
		if(d_t1[0]==0 && d_t1[1]*d_t1[2]<0.0){			
			v0=0;
			v1=1;
			v2=2;
		}
		else{
			if (d_t1[1]==0 && d_t1[0]*d_t1[2]<0.0){
				v0=1;
				v1=2;
				v2=0;
			}
			else{
				if (d_t1[2]==0 && d_t1[0]*d_t1[1]<0.0){
					v0=2;
					v1=0;
					v2=1;
				}
			}
		}
		
	}
	
	// (default) no vertex of t1 lies on the plane of t2 
	else{
		if(d_t1[0]*d_t1[1]>0.0){			// t1.vertices[2] is the point on the other side of the plane
			v0=2;
			v1=0;
			v2=1;
		}
		else{
			if(d_t1[0]*d_t1[2]>0.0){		// t1.vertices[1] is the point on the other side of the plane
				v0=1;
				v1=2;
				v2=0;
			}
			else{						// t1.vertices[0] is the point on the other side of the plane
				v0=0;
				v1=1;
				v2=2;
			}
		}	
	}
	
	// check if the two edges shared by vertex t1.vertices[v0] intersect triangle t2
	
	return MT_intersect_ray(t2,vertices_t1[v0],vertices_t1[v1]) || MT_intersect_ray(t2,vertices_t1[v0],vertices_t1[v2]);

}


// MT_intersect_ray(): returns true iff edge p1,p2 intersects the interior of sa using Moller–Trumbore intersection algorithm

template <typename value_type>
bool MT_intersect_ray(const simplex<2, 3, value_type> &sa, const point<3, value_type> &p1, const point<3, value_type> &p2)
{

	const point<3, value_type> *vertices_sa = sa.begin();
	
	point<3, value_type> e1 = vector_diff(vertices_sa[1],vertices_sa[0]);
	point<3, value_type> e2 = vector_diff(vertices_sa[2],vertices_sa[0]);
	
	point<3, value_type> P;		
	point<3, value_type> Q;		
	point<3, value_type> T;		
	point<3, value_type> D;		
	
	double det, inv_det, t, u, v;
	
	D		= vector_diff(p2, p1);
	P		= vector_cross(D,e2);
	det		= vector_dot(e1,P);
	
	if(det>-EPSILON && det<EPSILON) 
		return 0;
	
	inv_det	= 1.f/det;
	T		= vector_diff(p1,vertices_sa[0]);
	u 		= vector_dot(T,P) * inv_det;
	
	if(u<0.f || u>1.f) 
		return false;
		
	Q		= vector_cross(T,e1);
	v 		= vector_dot(D, Q) * inv_det;
	
	if(v<0.f || u+v>1.f) 
		return false;

	t 		= vector_dot(e2, Q) * inv_det;
		
	if(t>EPSILON) {
    	return true;
	}
	
	return false;
}


// simplex_on_side() : returns true iff all vertices of sb lie on one side of the plane defined by sa

template <std::size_t m, std::size_t n, typename value_type>
bool simplex_on_side(const simplex<m, n, value_type> &sa, const simplex<m, n, value_type> &sb)
{
	point<n, value_type> normal_sa = plane_normal(sa);
	
	if(vector_is_zero(normal_sa)) return true;					// invalid triangle

	const point<n, value_type> *vertices_sb = sb.begin();		
	double d_sa = -vector_dot(normal_sa,*sa.begin());			
	double dist_sb[3];
	
	for(int i=0; i<3; dist_sb[i++] = vector_dot(normal_sa, vertices_sb[i])+d_sa);
	
	// first check if there is intersection with the plane
	if(dist_sb[0]*dist_sb[1]*dist_sb[2]==0.0)				//a vertex of sb lies on the plane of sa
		return false;
	if(dist_sb[0]*dist_sb[1]<0 || dist_sb[0]*dist_sb[2]<0)	//an edge of sb crosses the plane of sa
		return false;
	
	return true;											//no edge of sb crosses the plane of sa
}



//////////////////////
// vector functions //
//////////////////////


// vector_scalar() : returns the scalar multiplication of vector pa and scalar m 

template <std::size_t n, typename value_type>
point<n, value_type> vector_scalar(const point<n, value_type> &pa, const value_type &m)
{
    value_type values[n];
	const value_type *pointer_pa = pa.begin();
	
	for(int i=0; i<n; values[i++]=pointer_pa[i]*m);
	
    return point<n, value_type>(&values[0], &values[3]);
}


// vector_diff() : returns the difference between vectors pa and pb 

template <std::size_t n, typename value_type>
point<n, value_type> vector_diff(const point<n, value_type> &pa, const point<n, value_type> &pb)
{
    value_type values[n];
	const value_type *pointer_pa = pa.begin();
	const value_type *pointer_pb = pb.begin();
	
	for(int i=0; i<n; values[i++]=pointer_pa[i]-pointer_pb[i]);
	
    return point<n, value_type>(&values[0], &values[3]);
}


// vector_sum() : returns the addition between vectors pa and pb

template <std::size_t n, typename value_type>
point<n, value_type> vector_sum(const point<n, value_type> &pa, const point<n, value_type> &pb)
{
    value_type values[n];
	const value_type *pointer_pa = pa.begin();
	const value_type *pointer_pb = pb.begin();
	
	for(int i=0; i<n; values[i++]=pointer_pa[i]+pointer_pb[i]);
	
    return point<n, value_type>(&values[0], &values[3]);
}


// vector_dot() : returns the dot product of vectors pa and pb

template <std::size_t n, typename value_type>
double vector_dot(const point<n, value_type> &pa, const point<n, value_type> &pb)
{
    double dot_product=0.0;
	const value_type *pointer_pa = pa.begin();
	const value_type *pointer_pb = pb.begin();
	
	for(int i=0; i<n; i++)
		dot_product+=pointer_pa[i]*pointer_pb[i];
	
    return dot_product;
}


// vector_cross() : returns the cross product of vectors pa and pb

template <typename value_type>
point<3, value_type> vector_cross(const point<3, value_type> &pa, const point<3, value_type> &pb)
{
    value_type values[3];
	const value_type *pointer_pa = pa.begin();
	const value_type *pointer_pb = pb.begin();
	
	values[0]=pointer_pa[1]*pointer_pb[2]-pointer_pa[2]*pointer_pb[1];
	values[1]=pointer_pa[2]*pointer_pb[0]-pointer_pa[0]*pointer_pb[2];
	values[2]=pointer_pa[0]*pointer_pb[1]-pointer_pa[1]*pointer_pb[0];
    
	return point<3, value_type>(&values[0], &values[3]);
}


// plane_normal() : returns the normal vector of the plane defined by sa

template <std::size_t m, typename value_type>
point<3, value_type> plane_normal(const simplex<m, 3, value_type> &sa)
{
	const point<3, value_type> *vertices_sa = sa.begin();	
	return vector_cross(vector_diff(vertices_sa[1],vertices_sa[0]),vector_diff(vertices_sa[2],vertices_sa[0]));
}


// vector_is_zero() : returns true iff vector pa contains only zeros, i.e, (0,0,0,...,0)

template <std::size_t n, typename value_type>
bool vector_is_zero(const point<n, value_type> &pa)
{
	const value_type *pointer_pa = pa.begin();
	for(int i=0; i<n; i++)
		if(pointer_pa[i]!=0.0) return false;
    return true;
}


#endif
