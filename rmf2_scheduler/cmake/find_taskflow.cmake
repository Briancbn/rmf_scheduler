macro(find_taskflow)
  find_package(Taskflow QUIET)
   
  # Trying vendor first
  if(NOT Taskflow_FOUND)
    message(STATUS "Trying taskflow_vendor Ament CMake package")
  
    if(USE_AMENT_VENDOR_EXTERNAL)
      find_package(taskflow_vendor QUIET)
      if(NOT taskflow_vendor_FOUND)
        message(STATUS "taskflow_vendor not found")
      endif()
    endif()
  endif()

  # Use CPM next
  if(NOT Taskflow_FOUND)
    if(USE_CPM_EXTERNAL)
      CPMAddPackage(
        NAME Taskflow
        GITHUB_REPOSITORY taskflow/taskflow
        GIT_TAG v3.11.0
        EXCLUDE_FROM_ALL YES
        OPTIONS
          "TF_BUILD_TESTS OFF"
          "TF_BUILD_EXAMPLES OFF"
      )

      if(Taskflow_ADDED AND TARGET Taskflow::Taskflow)
        message(STATUS "CPM added Taskflow")
        set(Taskflow_FOUND TRUE)
      endif()
    endif()
  endif()
  
  if(NOT Taskflow_FOUND)
    message(FATAL_ERROR "Taskflow not found")
  endif()
endmacro()
