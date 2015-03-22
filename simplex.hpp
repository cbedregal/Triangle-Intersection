#ifndef SIMPLEX_HPP
#define SIMPLEX_HPP

#include "point.hpp"

// a class representing an m-dimensional simplex embedded in n-dimensions

template <std::size_t m, std::size_t n, typename value_type>
struct simplex
{
    typedef point<n, value_type> point_type ;

    point_type vertices[m + 1] ;

    simplex()
    {
    }

    simplex(const simplex &s)
    {
        std::copy(s.begin(), s.end(), begin()) ;
    }

    template <typename value_iterator_type>
    simplex(value_iterator_type first, value_iterator_type last)
    {
        std::copy(first, last, begin()) ;
    }

    simplex &operator = (const simplex &s)
    {
        std::copy(s.begin(), s.end(), begin()) ;
        return *this ;
    }

    point_type *begin()
    {
        return &vertices[0] ;
    }

    point_type *end()
    {
        return &vertices[m + 1] ;
    }

    point_type const *begin() const
    {
        return &vertices[0] ;
    }

    point_type const *end() const
    {
        return &vertices[m + 1] ;
    }
} ;

template <std::size_t m, std::size_t n, typename value_type>
bool operator == (const simplex<m, n, value_type> &sa, const simplex<m, n, value_type> &sb)
{
    return std::equal(sa.begin(), sa.end(), sb.begin()) ;
}

template <std::size_t m, std::size_t n, typename value_type>
bool operator != (const simplex<m, n, value_type> &sa, const simplex<m, n, value_type> &sb)
{
    return !std::equal(sa.begin(), sa.end(), sb.begin()) ;
}

template <std::size_t m, std::size_t n, typename value_type>
std::ostream &operator << (std::ostream &stream, const simplex<m, n, value_type> &s)
{
    std::copy(s.begin(), s.end(), std::ostream_iterator<typename simplex<m, n, value_type>::point_type>(stream, " ")) ;
    return stream ;
}

template <std::size_t n, typename value_type>
simplex<2, n, value_type> make_simplex(const point<n, value_type> &p1, const point<n, value_type> &p2, const point<n, value_type> &p3)
{
    const point<n, value_type> vertices[3] = {p1, p2, p3} ;
    return simplex<2, n, value_type>(&vertices[0], &vertices[3]) ;
}

#endif
