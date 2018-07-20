// This file is part of the Acts project.
//
// Copyright (C) 2016-2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <tuple>
#include <type_traits>
#include "Acts/Utilities/detail/MPL/all_of.hpp"

namespace Acts {

namespace detail {

  /// This sctruct defines an extendable std::tuple
  ///
  /// all Extensions have to :
  ///   - default constructible
  ///   - copy constructible
  ///   - move constructible
  ///
  /// This is needed in order to allow custom construction of objects
  template <typename... Extensions>
  struct Extendable
  {
    // clang-format off
    static_assert(detail::all_of_v<std::is_default_constructible<Extensions>::value...>,
                  "all extensions must be default constructible");
    static_assert(detail::all_of_v<std::is_copy_constructible<Extensions>::value...>,
                  "all extensions must be copy constructible");
    static_assert(detail::all_of_v<std::is_move_constructible<Extensions>::value...>,
                  "all extensions must be move constructible");
    // clang-format on

    template <typename R>
    const R&
    get() const
    {
      return std::get<R>(m_tExtensions);
    }

    template <typename R>
    R&
    get()
    {
      return std::get<R>(m_tExtensions);
    }

    const std::tuple<Extensions...>&
    tuple() const
    {
      return m_tExtensions;
    }

    std::tuple<Extensions...>&
    tuple()
    {
      return m_tExtensions;
    }

  private:
    std::tuple<Extensions...> m_tExtensions;
  };

}  // namespace detail

}  // namespace Acts