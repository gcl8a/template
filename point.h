//05/20/02 added math operators to TPoint2D
//modified 02/28/01

#ifndef __TEMPLATE_POINT_H
#define __TEMPLATE_POINT_H

#include <iostream.h>

template <class T> class TPoint2D
{
 public:
  T x;
  T y;
 public:
  TPoint2D(void) {x=y=0;}
  TPoint2D(T X, T Y) {x=X; y=Y;}
  int operator == (const TPoint2D& point) const
    {
      return (x==point.x && y==point.y);
    }
  int operator != (const TPoint2D& point) const
    {
      return !((*this)==point);
    }

  TPoint2D& operator += (const TPoint2D&);
  TPoint2D operator + (const TPoint2D&);
  TPoint2D operator - (const TPoint2D&);
  TPoint2D operator * (const T&);
  TPoint2D operator / (const T&);

  friend ostream& operator << <>(ostream&, const TPoint2D<T>&);
};

template <class T> TPoint2D<T>& TPoint2D<T>::operator += (const TPoint2D<T>& p)
{
  x+=p.x;
  y+=p.y;
  return *this;
}

template <class T> TPoint2D<T> TPoint2D<T>::operator + (const TPoint2D<T>& p)
{
  TPoint2D<T> t;
  t.x=x+p.x;
  t.y=y+p.y;
  return t;
}

template <class T> TPoint2D<T> TPoint2D<T>::operator - (const TPoint2D<T>& p)
{
  TPoint2D<T> t;
  t.x=x-p.x;
  t.y=y-p.y;
  return t;
}

template <class T> TPoint2D<T> TPoint2D<T>::operator * (const T& t)
{
  TPoint2D<T> p;
  p.x=x*t;
  p.y=y*t;
  return p;
}

template <class T> TPoint2D<T> TPoint2D<T>::operator / (const T& t)
{
  TPoint2D<T> p;
  p.x=x/t;
  p.y=y/t;
  return p;
}

template <class T> ostream& operator <<
(ostream& stream, const TPoint2D<T>& point)
{
  stream<<point.x<<'\t'<<point.y;
  return stream;
}

template <class T> class TPoint3D
{
 public:
  T x;
  T y;
  T z;
 public:
  TPoint3D(void) {x=y=z=0;}
  TPoint3D(T X, T Y, T Z) {x=X; y=Y; z=Z;}
  int operator == (const TPoint3D<T> tP)
    {return x==tP.x && y==tP.y && z==tP.z;}
  int operator != (const TPoint3D& point) const
    {
      return !((*this)==point);
    }
  
  friend ostream& operator << <>(ostream&, const TPoint3D<T>&);
};

template <class T> ostream& operator <<
(ostream& stream, const TPoint3D<T>& point)
{
  stream<<point.x<<'\t'<<point.y<<'\t'<<point.z;
  return stream;
}

#endif












