# Modified by Ramon Casero for project Gerardus (see comments headed
# by "gerardus:").
#
# Version: 0.1.0
# $Rev$
# $Date$

if ( NOT CGAL_Boost_Setup )

  # gerardus: We distribute the boost libraries with gerardus, so we
  # don't need to look for them as an external package. In fact, if we
  # don't comment out the find_package() below, we get a recursive
  # loop with CGAL that throws a configuration warning

  # include(CGAL_TweakFindBoost)
  # # In the documentation, we say we require Boost-1.39, but technically we
  # # require 1.33.1. Some packages may require more recent versions, though.
  # find_package( Boost 1.33.1 REQUIRED thread system )
  
  # message( STATUS "Boost include:     ${Boost_INCLUDE_DIRS}" )
  # message( STATUS "Boost libraries:   ${Boost_LIBRARIES}" )
  # message( STATUS "Boost definitions: ${Boost_DEFINITIONS}" )
  
  set ( CGAL_USE_BOOST 1 )
  
  include(CGAL_Macros)
  
  add_to_cached_list(CGAL_3RD_PARTY_INCLUDE_DIRS   ${Boost_INCLUDE_DIRS} )
  add_to_cached_list(CGAL_3RD_PARTY_LIBRARIES_DIRS ${Boost_LIBRARY_DIRS} )
  add_to_cached_list(CGAL_3RD_PARTY_DEFINITIONS    ${Boost_DEFINITIONS}  )
  
  if ( NOT MSVC )
    add_to_cached_list(CGAL_3RD_PARTY_LIBRARIES ${Boost_LIBRARIES} )
  endif()
  
  message( STATUS "USING BOOST_VERSION = '${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}'" )
  
  set ( CGAL_Boost_Setup TRUE )
  
endif()

