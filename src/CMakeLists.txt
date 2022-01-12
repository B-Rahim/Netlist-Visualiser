# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/8-10>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

cmake_minimum_required(VERSION 2.8.11)
project(SCHEMATIC)

set (CMAKE_CXX_FLAGS            "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
set (CMAKE_INSTALL_PREFIX       "../install" )

set (CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)


find_package(LibXml2 REQUIRED)
find_package(Qt5  COMPONENTS  Widgets Core Gui REQUIRED) 


   include_directories( ${SCHEMATIC_SOURCE_DIR}
                        ${LIBXML2_INCLUDE_DIR}
                        ${QT5_INCLUDE_DIR}
                      )

                   set( includes     Indentation.h
                                     XmlUtil.h 
                                     Point.h
                                     Box.h
                                     Term.h
                                     Net.h
                                     Instance.h
                                     Node.h
                                     Line.h
                                     Shape.h
                                     Symbol.h
                                     Cell.h
                      )

                   set( mocIncludes  SaveCellDialog.h
                                     OpenCellDialog.h
                                     CellWidget.h
                                     CellViewer.h
                                     InstancesWidget.h
                                     InstancesModel.h
                                     CellsModel.h
                                     CellsLib.h
                      )

                   set( cpps         Indentation.cpp
                                     XmlUtil.cpp 
                                     Box.cpp
                                     Term.cpp
                                     Net.cpp
                                     Instance.cpp
                                     Node.cpp
                                     Line.cpp
                                     Shape.cpp
                                     Symbol.cpp
                                     Cell.cpp
                                     SaveCellDialog.cpp
                                     OpenCellDialog.cpp
                                     CellWidget.cpp
                                     CellViewer.cpp
                                     InstancesWidget.cpp
                                     InstancesModel.cpp
                                     CellsModel.cpp
                                     CellsLib.cpp
                                     Main.cpp
                      )

          qt5_wrap_cpp( mocCpps      ${mocIncludes} )

        add_executable( visual       ${cpps} ${mocCpps} )
 target_link_libraries( visual       Qt5::Widgets Qt5::Core Qt5::Gui  ${LIBXML2_LIBRARIES} ) 
               install( TARGETS      visual                      DESTINATION bin )
               install( FILES        ${includes} ${mocIncludes}  DESTINATION include )