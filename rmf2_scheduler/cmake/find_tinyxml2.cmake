macro(find_tinyxml2)
  if(USE_AMENT_VENDOR_EXTERNAL)
    find_package(tinyxml2_vendor QUIET)
    message(STATUS "Trying tinyxml2_vendor Ament CMake package")
  endif()
   
  # Trying vendor first
  if(NOT TinyXML2_FOUND)
    list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
    find_package(TinyXML2 REQUIRED)
  endif()
endmacro()
