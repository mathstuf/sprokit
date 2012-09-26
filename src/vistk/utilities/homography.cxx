/*ckwg +5
 * Copyright 2011-2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "homography.h"

#include "plane_ref.h"
#include "timestamp.h"
#include "utm.h"

/**
 * \file homography.cxx
 *
 * \brief Implementation of the homography classes.
 */

namespace vistk
{

homography_base
::homography_base(homography_base const& h)
  : m_transform(h.m_transform)
  , m_valid(h.m_valid)
  , m_new_reference(h.m_new_reference)
{
}

homography_base
::~homography_base()
{
}

homography_base::transform_t const&
homography_base
::transform() const
{
  return m_transform;
}

bool
homography_base
::is_valid() const
{
  return m_valid;
}

bool
homography_base
::is_new_reference() const
{
  return m_new_reference;
}

void
homography_base
::set_transform(transform_t const& trans)
{
  m_transform = trans;
}

void
homography_base
::set_identity()
{
  m_transform.set_identity();
}

void
homography_base
::set_valid(bool valid)
{
  m_valid = valid;
}

void
homography_base
::set_new_reference(bool new_reference)
{
  m_new_reference = new_reference;
}

bool
homography_base
::operator == (homography_base const& h) const
{
  return ((m_valid == h.m_valid) &&
          (m_new_reference == h.m_new_reference) &&
          (m_transform == h.m_transform));
}

homography_base
::homography_base()
  : m_valid(false)
  , m_new_reference(false)
{
  set_identity();
}

template <typename Source, typename Dest>
homography<Source, Dest>
::homography()
{
}

template <typename Source, typename Dest>
homography<Source, Dest>
::homography(self_t const& h)
  : homography_base(h)
  , m_source(h.m_source)
  , m_dest(h.m_dest)
{
}

template <typename Source, typename Dest>
homography<Source, Dest>
::~homography()
{
}

template <typename Source, typename Dest>
typename homography<Source, Dest>::source_t
homography<Source, Dest>
::source() const
{
  return m_source;
}

template <typename Source, typename Dest>
typename homography<Source, Dest>::dest_t
homography<Source, Dest>
::destination() const
{
  return m_dest;
}

template <typename Source, typename Dest>
void
homography<Source, Dest>
::set_source(source_t const& src)
{
  m_source = src;
}

template <typename Source, typename Dest>
void
homography<Source, Dest>
::set_destination(dest_t const& dest)
{
  m_dest = dest;
}

template <typename Source, typename Dest>
typename homography<Source, Dest>::inverse_t
homography<Source, Dest>
::inverse() const
{
  inverse_t result;

  result.set_transform(transform().get_inverse());
  result.set_source(m_dest);
  result.set_destination(m_source);
  result.set_valid(is_valid());
  result.set_new_reference(is_new_reference());

  return result;
}

template <typename Source, typename Dest>
bool
homography<Source, Dest>
::operator == (self_t const& h) const
{
  return ((homography_base::operator == (h)) &&
          (m_source == h.m_source) &&
          (m_dest == h.m_dest));
}

}

// Keep in sync with homography_types.h
template class vistk::homography<vistk::timestamp, vistk::timestamp>;
template class vistk::homography<vistk::timestamp, vistk::plane_ref_t>;
template class vistk::homography<vistk::plane_ref_t, vistk::timestamp>;
template class vistk::homography<vistk::timestamp, vistk::utm_zone_t>;
template class vistk::homography<vistk::utm_zone_t, vistk::timestamp>;
template class vistk::homography<vistk::plane_ref_t, vistk::utm_zone_t>;
template class vistk::homography<vistk::utm_zone_t, vistk::plane_ref_t>;

template <typename Source, typename Shared, typename Dest>
vistk::homography<Source, Dest>
vistk::operator * (vistk::homography<Shared, Dest> const& l, vistk::homography<Source, Shared> const& r)
{
  vistk::homography<Source, Dest> result;

  result.set_transform(l.transform() * r.transform());
  result.set_source(r.source());
  result.set_destination(l.destination());
  result.set_valid(l.is_valid() && r.is_valid());

  return result;
}

#ifndef DOXYGEN_IGNORE

/**
 * \def INSTANTIATE_MULT_RAW
 *
 * \brief Instantiates multiplication with three distinct types.
 *
 * \param X The source type in the multiplication.
 * \param Y The shared type in the multiplication.
 * \param Z The destination type in the multiplication.
 */
#define INSTANTIATE_MULT_RAW(X, Y, Z) \
  template homography<X, Z> operator * <X, Y, Z>(homography<Y, Z> const&, homography<X, Y> const&)

/**
 * \def INSTANTIATE_SELF_MULT
 *
 * \brief Instantiates multiplication with homogeneous types.
 *
 * \param X The reference plane for the multiplication.
 */
#define INSTANTIATE_SELF_MULT(X) \
  INSTANTIATE_MULT_RAW(X, X, X)

/**
 * \def INSTANTIATE_DUAL_MULT_RAW
 *
 * \brief Instantiates multiplication with two distinct types.
 *
 * \param X The first type in the multiplication.
 * \param Y The second type in the multiplication.
 */
#define INSTANTIATE_DUAL_MULT_RAW(X, Y) \
  INSTANTIATE_MULT_RAW(X, Y, X);        \
  INSTANTIATE_MULT_RAW(X, X, Y);        \
  INSTANTIATE_MULT_RAW(X, Y, Y)

/**
 * \def INSTANTIATE_DUAL_MULT
 *
 * \brief Instantiates multiplication with two distinct types.
 *
 * This instantiates all permutations between the two types.
 *
 * \param X The first type in the multiplication.
 * \param Y The second type in the multiplication.
 */
#define INSTANTIATE_DUAL_MULT(X, Y) \
  INSTANTIATE_DUAL_MULT_RAW(X, Y);  \
  INSTANTIATE_DUAL_MULT_RAW(Y, X)

/**
 * \def INSTANTIATE_TRIP_MULT
 *
 * \brief Instantiates multiplication with three distinct types.
 *
 * This instantiates all permutations between the three types.
 *
 * \param X The first type in the multiplication.
 * \param Y The second type in the multiplication.
 * \param Z The third type in the multiplication.
 */
#define INSTANTIATE_TRIP_MULT(X,Y,Z)  \
  INSTANTIATE_MULT_RAW(X, Y, Z);      \
  INSTANTIATE_MULT_RAW(X, Z, Y);      \
  INSTANTIATE_MULT_RAW(Y, X, Z);      \
  INSTANTIATE_MULT_RAW(Y, Z, X);      \
  INSTANTIATE_MULT_RAW(Z, X, Y);      \
  INSTANTIATE_MULT_RAW(Z, Y, X)

namespace vistk
{

// Instantiate all allowable types
INSTANTIATE_SELF_MULT(vistk::timestamp);
INSTANTIATE_SELF_MULT(vistk::plane_ref_t);
INSTANTIATE_SELF_MULT(vistk::utm_zone_t);

INSTANTIATE_DUAL_MULT(vistk::timestamp,   vistk::plane_ref_t);
INSTANTIATE_DUAL_MULT(vistk::timestamp,   vistk::utm_zone_t);
INSTANTIATE_DUAL_MULT(vistk::plane_ref_t, vistk::utm_zone_t);

INSTANTIATE_TRIP_MULT(vistk::timestamp,   vistk::plane_ref_t, vistk::utm_zone_t);

}

#undef INSTANTIATE_MULT_RAW
#undef INSTANTIATE_SELF_MULT
#undef INSTANTIATE_DUAL_MULT_RAW
#undef INSTANTIATE_DUAL_MULT
#undef INSTANTIATE_TRIP_MULT

#endif // DOXYGEN_IGNORE