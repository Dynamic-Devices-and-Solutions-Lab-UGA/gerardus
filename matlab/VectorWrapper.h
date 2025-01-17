/*
 * VectorWrapper.h
 *
 * There are classes in ITK, e.g.  that are conceptually vectors, but
 * don't have the same methods as e.g. std::vector. For example,
 * itk::Size<Dimension> has the static method
 * itk::Size<Dimension>::GetDimensionSize, while std::vector has the
 * method v.size().
 *
 * This means that a function in Gerardus would need to be coded
 * twice, in both cases with the same code except for the
 * ::GetDimensionSize (know at compilation time) or .size() (known at
 * run time).
 *
 * Another example. In the CGAL library,
 * CGAL::Point_3<CGAL::Simple_cartesian<double> > is a 3-vector. This
 * vector is different from the other two in that its elements cannot
 * be populated using e.g. v[0] = 1.0; Instead, the vector is
 * populated with the constructor,
 * CGAL::Point_3<CGAL::Simple_cartesian<double> > v 
 *   = CGAL::Point_3<CGAL::Simple_cartesian<double> >(1.0, -2.3, 4.5);
 *
 * To overcome this problem, this wrapper provides a common interface
 * to different vector classes, so that we can interact with all these
 * vector-like classes the same way. By default, we assume that we
 * have a std::vector. For other types, we specialise the
 * VectorWrapper class
 */

 /*
  * Author: Ramon Casero <rcasero@gmail.com>
  * Copyright © 2012-2013 University of Oxford
  * Version: 0.6.3
  *
  * University of Oxford means the Chancellor, Masters and Scholars of
  * the University of Oxford, having an administrative office at
  * Wellington Square, Oxford OX1 2JD, UK. 
  *
  * This file is part of Gerardus.
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details. The offer of this
  * program under the terms of the License is subject to the License
  * being interpreted in accordance with English Law and subject to any
  * action against the University of Oxford being under the jurisdiction
  * of the English Courts.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see
  * <http://www.gnu.org/licenses/>.
  */

#ifndef VECTORWRAPPER_H
#define VECTORWRAPPER_H

/* ITK headers */
#include "itkSize.h"
#include "itkPoint.h"

/* CGAL headers */
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

/* Boost headers */
#include <boost/lexical_cast.hpp>

/*
 * This is the default definition of the VectorWrapper class. The
 * corresponding methods read Matlab into a std::vector.
 *
 * To read Matlab data into another type of vector,
 * e.g. itk::SizeType, CGAL::Point_3, etc, we overload the
 * VectorWrapper class using partial template specialization in the
 * sections below.
 *
 * VectorValueType: type of each element in the vector returned to the user
 * VectorType:      type of the vector itself
 * VectorSize:      number of elements in the vector (ignored in the default 
 *                  implementation of the class, but required for some of 
 *                  the partial specialisations)
 * MatlabValueType: type of each element in the Matlab input buffer
 *
 * The reason why we need to have all VectorValueType, VectorType and
 * VectorSize as templates is because of the great variety of vector
 * types. Some are dynamic, e.g. std::vector, and some need their
 * length at compilation time, e.g. itk::Size<VectorSize>. In some
 * cases, to avoid code duplication, we need the VectorValueType so
 * that we can do a partial specialisation like
 * itk::FixedArray<VectorValueType, VectorSize>.
 */
template<class VectorValueType, class VectorType, class MatlabValueType>
  class VectorWrapper{

 public:

  VectorWrapper() {}

  // read a row from a Matlab matrix
  VectorType ReadRowVector(const mxArray *pm, mwIndex row, std::string paramName);

  // read a whole array into a vector
  VectorType ReadArrayAsVector(const mxArray *pm, std::string paramName);

  // read the argument dimensions into a vector (size = 2 * halfsize + 1)
  VectorType ReadSize(const mxArray *pm, std::string paramName);
  VectorType ReadHalfSize(const mxArray *pm, std::string paramName);

};

/*
 * Partial specialisation if we want to put Matlab's row data into a vector-like class:
 * itk::Size<VectorSize> 
 * itk::FixedArray<VectorSize>
 * itk::PointType
 * itk::Vector<VectorValueType, Dimension> (including itk::Image::SpacingType)
 */

// ReadItkRowVector
//
// auxiliary functions so that we don't need to rewrite code that is
// the same for all fixed ITK vector-like types, despite the
// particular partial specialisation
template <class VectorValueType, class VectorType, class MatlabValueType>
VectorType
ReadItkRowVector(const mxArray *pm, mwIndex row, std::string paramName);

template <class VectorValueType, class VectorType, class MatlabValueType>
VectorType
ReadItkSize(const mxArray *pm, mwIndex row, std::string paramName);

template <class VectorValueType, class VectorType, class MatlabValueType>
VectorType
ReadItkHalfSize(const mxArray *pm, mwIndex row, std::string paramName);

#define MethodsVectorWrapperItk						\
  public:								\
  VectorWrapper() {}							\
  VectorType								\
  ReadRowVector(const mxArray *pm, mwIndex row, std::string paramName) { \
    return ReadItkRowVector<VectorValueType, VectorType, MatlabValueType>(pm, row, paramName); \
  }									\
  VectorType								\
  ReadSize(const mxArray *pm, std::string paramName) {			\
    return ReadItkSize<VectorValueType, VectorType, MatlabValueType>(pm, paramName); \
  }									\
  VectorType								\
  ReadHalfSize(const mxArray *pm, std::string paramName) {		\
    return ReadItkHalfSize<VectorValueType, VectorType, MatlabValueType>(pm, paramName); \
  }

// partial specialisations itk::Size<VectorSize>
template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, typename itk::Size<2>, MatlabValueType>{
 private:
  typedef typename itk::Size<2> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, typename itk::Size<3>, MatlabValueType>{
 private:
  typedef typename itk::Size<3> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, typename itk::Size<4>, MatlabValueType>{
 private:
  typedef typename itk::Size<4> VectorType;
  MethodsVectorWrapperItk
};

// partial specialisations itk::FixedArray<VectorValueType, 2>
template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::FixedArray<VectorValueType, 2>, MatlabValueType>{
 private:
  typedef itk::FixedArray<VectorValueType, 2> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::FixedArray<VectorValueType, 3>, MatlabValueType>{
 private:
  typedef itk::FixedArray<VectorValueType, 3> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::FixedArray<VectorValueType, 4>, MatlabValueType>{
 private:
  typedef itk::FixedArray<VectorValueType, 4> VectorType;
  MethodsVectorWrapperItk
};

// partial specialisations itk::Point<VectorValueType, 2>
template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::Point<VectorValueType, 2>, MatlabValueType>{
 private:
  typedef itk::Point<VectorValueType, 2> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::Point<VectorValueType, 3>, MatlabValueType>{
 private:
  typedef itk::Point<VectorValueType, 3> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::Point<VectorValueType, 4>, MatlabValueType>{
 private:
  typedef itk::Point<VectorValueType, 4> VectorType;
  MethodsVectorWrapperItk
};

// partial specialisations itk::Vector<VectorValueType, Dimension>
// This includes itk::Image::SpacingType
template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::Vector<VectorValueType, 2>, MatlabValueType>{
 private:
  typedef itk::Vector<VectorValueType, 2> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::Vector<VectorValueType, 3>, MatlabValueType>{
 private:
  typedef itk::Vector<VectorValueType, 3> VectorType;
  MethodsVectorWrapperItk
};

template<class VectorValueType, class MatlabValueType>
  class VectorWrapper<VectorValueType, itk::Vector<VectorValueType, 4>, MatlabValueType>{
 private:
  typedef itk::Vector<VectorValueType, 4> VectorType;
  MethodsVectorWrapperItk
};

#undef MethodsVectorWrapperItk

/*
 * Partial specialisation if we want to put Matlab's row data into a
 * CGAL::Point_3<CGAL::Simple_cartesian<double> > 
 * CGAL::Direction_3<CGAL::Simple_cartesian<double> >
 * CGAL::Point_3<CGAL::Exact_predicates_exact_constructions_kernel> 
 * CGAL::Point_3<CGAL::Exact_predicates_inexact_constructions_kernel> 
 * vector-like class
 *
 * Note that in these partial specializations, the template
 * VectorValueType is ignored, so when calling ReadRowVector, it can
 * be set to e.g. "void". For example, ReadRowVectorFromMatlab<void, Point>.
 */

// ReadCgalRowVector
//
// auxiliary function so that we don't need to rewrite this code in
// every partial specialization
template <class VectorType, class MatlabValueType>
  VectorType
  ReadCgalRowVector(const mxArray *pm, mwIndex row, std::string paramName);

#define VectorWrapperCgal(VECTORTYPE)					\
  template<class VectorValueType, class MatlabValueType>		\
    class VectorWrapper<VectorValueType, VECTORTYPE, MatlabValueType>{	\
  private:								\
    typedef VECTORTYPE VectorType;					\
  public:								\
    VectorWrapper() {}							\
    VectorType								\
      ReadRowVector(const mxArray *pm, mwIndex row, std::string paramName) { \
      return ReadCgalRowVector<VectorType, MatlabValueType>(pm, row, paramName); \
    }									\
    VectorType								\
      ReadRowVector(const mxArray *pm, std::string paramName) {		\
      return ReadCgalRowVector<VectorType, MatlabValueType>(pm, 0, paramName); \
    }									\
  };

// partial specialisations
VectorWrapperCgal(typename CGAL::Point_3<CGAL::Simple_cartesian<double> >)
VectorWrapperCgal(typename CGAL::Direction_3<CGAL::Simple_cartesian<double> >)
VectorWrapperCgal(typename CGAL::Point_3<CGAL::Exact_predicates_exact_constructions_kernel >)
VectorWrapperCgal(typename CGAL::Point_3<CGAL::Exact_predicates_inexact_constructions_kernel >)

#undef VectorWrapperCgal

#ifndef ITK_MANUAL_INSTANTIATION
#include "VectorWrapper.hxx"
#endif

#endif /* VECTORWRAPPER_H */
