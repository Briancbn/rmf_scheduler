macro(find_croncpp)
  find_package(croncpp QUIET)
   
  # Trying vendor first
  if(NOT croncpp_FOUND)
    message(STATUS "Trying croncpp_vendor Ament CMake package")
  
   if(USE_AMENT_VENDOR_EXTERNAL)
      find_package(croncpp_vendor QUIET)
      if(NOT croncpp_vendor_FOUND)
        message(STATUS "croncpp_vendor not found")
      endif()
    endif()
  endif()

  # Use CPM next
  if(NOT croncpp_FOUND)
    if(USE_CPM_EXTERNAL)
      CPMAddPackage(
        NAME croncpp
        PATCHES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/croncpp.patch
        GITHUB_REPOSITORY mariusbancila/croncpp
        GIT_TAG v2023.03.30
        EXCLUDE_FROM_ALL YES
        OPTIONS
          "CRONCPP_BUILD_TESTS OFF"
          "CRONCPP_BUILD_BENCHMARK OFF"
      )

      if(croncpp_ADDED AND TARGET croncpp::croncpp)
        message(STATUS "CPM added croncpp")
        set(croncpp_FOUND TRUE)
      endif()
    endif()
  endif()
  
  if(NOT croncpp_FOUND)
    message(FATAL_ERROR "croncpp not found")
  endif()
endmacro()
