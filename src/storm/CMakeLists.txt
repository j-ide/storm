file(GLOB_RECURSE ALL_FILES ${PROJECT_SOURCE_DIR}/src/storm/*.h ${PROJECT_SOURCE_DIR}/src/storm/*.cpp)

register_source_groups_from_filestructure("${ALL_FILES}" storm)

#############################################################
##
##	Source file aggregation and clustering
##
#############################################################
file(GLOB_RECURSE STORM_SOURCES ${PROJECT_SOURCE_DIR}/src/storm/*/*.cpp)
file(GLOB_RECURSE STORM_HEADERS ${PROJECT_SOURCE_DIR}/src/storm/*.h)

# Additional include files like the storm-config.h
file(GLOB_RECURSE STORM_BUILD_HEADERS ${PROJECT_BINARY_DIR}/include/*.h)

# Add custom additional include or link directories
if (ADDITIONAL_INCLUDE_DIRS)
	message(STATUS "Storm - Using additional include directories ${ADDITIONAL_INCLUDE_DIRS}")
	include_directories(${ADDITIONAL_INCLUDE_DIRS})
endif(ADDITIONAL_INCLUDE_DIRS)
if (ADDITIONAL_LINK_DIRS)
	message(STATUS "Storm - Using additional link directories ${ADDITIONAL_LINK_DIRS}")
	link_directories(${ADDITIONAL_LINK_DIRS})
endif(ADDITIONAL_LINK_DIRS)


###############################################################################
##                                                                            
##	Binary creation (All link_directories() calls must be made before this point.)
##
###############################################################################

# Create libstorm.
add_library(storm SHARED ${STORM_3RDPARTY_SOURCES} ${STORM_SOURCES} ${STORM_HEADERS}
		storage/memorystructure/SparseModelDARewardProduct.h)
target_precompile_headers(storm PRIVATE ${STORM_PRECOMPILED_HEADERS})
# Remove define symbol for shared libstorm.
set_target_properties(storm PROPERTIES DEFINE_SYMBOL "")
add_dependencies(storm resources)
#The library that needs symbols must be first, then the library that resolves the symbol.
target_link_libraries(storm PUBLIC ${STORM_DEP_TARGETS} ${STORM_DEP_IMP_TARGETS} ${STORM_LINK_LIBRARIES})
#target_include_directories(storm PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/storm>$<INSTALL_INTERFACE:include/storm>")
target_include_directories(storm PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/>$<INSTALL_INTERFACE:include/storm>")
target_include_directories(storm PUBLIC "$<BUILD_INTERFACE:${STORM_3RDPARTY_BINARY_DIR}>$<INSTALL_INTERFACE:include>") # the interface loc is not right
# Add base folder for better inclusion paths
list(APPEND STORM_TARGETS storm)
set(STORM_TARGETS ${STORM_TARGETS} PARENT_SCOPE)

# Install storm headers to include directory.
foreach(HEADER ${STORM_HEADERS})
	string(REGEX REPLACE "${PROJECT_SOURCE_DIR}/src/?" "" RELATIVE_HEADER_PATH ${HEADER})
	string(REGEX MATCH "(.*)[/\\]" RELATIVE_DIRECTORY ${RELATIVE_HEADER_PATH})
	string(REGEX REPLACE "${RELATIVE_DIRECTORY}/?" "" HEADER_FILENAME ${RELATIVE_HEADER_PATH})
	add_custom_command(
		OUTPUT ${CMAKE_BINARY_DIR}/include/${RELATIVE_DIRECTORY}${HEADER_FILENAME}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/${RELATIVE_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy ${HEADER} ${CMAKE_BINARY_DIR}/include/${RELATIVE_DIRECTORY}${HEADER_FILENAME}
		DEPENDS ${HEADER}
	)
	list(APPEND STORM_OUTPUT_HEADERS "${CMAKE_BINARY_DIR}/include/${RELATIVE_DIRECTORY}${HEADER_FILENAME}")
endforeach()
add_custom_target(copy_storm_headers DEPENDS ${STORM_OUTPUT_HEADERS} ${STORM_HEADERS})
add_dependencies(storm copy_storm_headers)
add_dependencies(storm copy_resources_headers)


# installation
install(TARGETS storm EXPORT storm_Targets RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION include/storm
		FILES_MATCHING PATTERN "*.h")
