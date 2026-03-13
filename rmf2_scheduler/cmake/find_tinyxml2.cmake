# Copyright 2025 ROS Industrial Consortium Asia Pacific
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# Find TinyXML2 Library
#

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
