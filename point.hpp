#ifndef POINT_HPP
#define POINT_HPP

#include <algorithm>
#include <iterator>

// a class representing an n-dimensional point whose coordinates are of type value_type

template <std::size_t n, typename value_type>
struct point
{
    value_type values[n] ;

    point()
    {
    }

    point(const point &p)
    {
        std::copy(p.begin(), p.end(), begin()) ;
    }

    template <typename value_iterator_type>
    point(value_iterator_type first, value_iterator_type last)
    {
        std::copy(first, last, begin()) ;
    }

    point &operator = (const point &p)
    {
        std::copy(p.begin(), p.end(), begin()) ;
        return *this ;
    }

    value_type *begin()
    {
        return &values[0] ;
    }

    value_type *end()
    {
        return &values[n] ;
    }

    value_type const *begin() const
    {
        return &values[0] ;
    }

    value_type const *end() const
    {
        return &values[n] ;
    }
	
} ;

template <std::size_t n, typename value_type>
bool operator == (const point<n, value_type> &pa, const point<n, value_type> &pb)
{
    return std::equal(pa.begin(), pa.end(), pb.begin()) ;
}

template <std::size_t n, typename value_type>
bool operator != (const point<n, value_type> &pa, const point<n, value_type> &pb)
{
    return !std::equal(pa.begin(), pa.end(), pb.begin()) ;
}

template <std::size_t n, typename value_type>
std::ostream &operator << (std::ostream &stream, const point<n, value_type> &p)
{
    std::copy(p.begin(), p.end(), std::ostream_iterator<value_type>(stream, " ")) ;
    return stream ;
}

template <typename value_type>
point<3, value_type> make_point(const value_type &x, const value_type &y, const value_type &z)
{
    const value_type values[3] = {x, y, z} ;
    return point<3, value_type>(&values[0], &values[3]) ;
}

#endif
