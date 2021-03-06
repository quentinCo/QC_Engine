cmake_minimum_required(VERSION 3.9)

project(opengl_engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# Lib files
		# qc_graphic
	set(QC_GRAPHIC_DIR lib/qc_graphic)

	file(
        GLOB_RECURSE
		QC_GRAPHIC_SRC_FILES
		${QC_GRAPHIC_DIR}/src/*.c
		${QC_GRAPHIC_DIR}/src/*.cpp
	)

	file(
		GLOB_RECURSE
		QC_GRAPHIC_INCLUDE_FILES
		${QC_GRAPHIC_DIR}/include/qc_graphic/*.h
		${QC_GRAPHIC_DIR}/include/qc_graphic/*.hpp
	)

	file(
		GLOB_RECURSE
		QC_GRAPHIC_SHADER_FILES
		${QC_GRAPHIC_DIR}/shaders/*.glsl
	)

	add_library(
		qc_graphic
		${QC_GRAPHIC_SRC_FILES}
		${QC_GRAPHIC_INCLUDE_FILES}
		${QC_GRAPHIC_SHADER_FILES}
	)

	source_group(
		"Shaders"
		FILES
		${QC_GRAPHIC_SHADER_FILES}
	)

#	function(set_subDir_filter _rel_dir _sub_dir _file_list)
#		message("_sub_dir --> " ${_sub_dir})
#		foreach(_source IN ITEMS ${_file_list})
#			message("_source --> " "${_source}")
#			get_filename_component(_src_root_path ${_rel_dir} REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
#			message("_src_root_path --> " "${_src_root_path}")
#			get_filename_component(_source_path "${_source}" PATH)
#			message("_source_path --> " "${_source_path}")
#			file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
#			string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
#			message("_group_path --> " "${_group_path}")
#			string(FIND "${_group_path}" ${_sub_dir} _src_pos)
#			message("_src_pos --> " "${_src_pos}")
#			string(LENGTH "${_group_path}" _length)
#			message("_length --> " "${_length}")
#			string(SUBSTRING "${_group_path}" "${_src_pos}" -1 _src_subdir)
#			message("_src_subdir --> " "${_src_subdir}")
#			source_group("${_src_subdir}" FILES "${_source}")
#			
#		endforeach()
#	endfunction()
#	
#	SET(_arg "include")
#	set_subDir_filter(${QC_GRAPHIC_DIR}/include/qc_graphic "${_arg}" ${QC_GRAPHIC_INCLUDE_FILES})
#	SET(_arg "src")
#	set_subDir_filter(${QC_GRAPHIC_DIR}/src/ "${_arg}" ${QC_GRAPHIC_SRC_FILES})
	
	

set(LIBRARIES ${LIBRARIES} qc_graphic)

#add_subdirectory(${QC_GRAPHIC_DIR})
include_directories(${QC_GRAPHIC_DIR}/include)

	# third-parties
source_group ("third-party" REGULAR_EXPRESSION "third-party/*.*")

		# OpenGL
find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARIES})
include_directories(${OPENGL_INCLUDE_DIRS})
#[[
			# assimp
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(AddGTest_FOUND OFF CACHE BOOL "" FORCE)
	set(ASSIMP_DIR third-party/assimp)
	set(LIBRARIES ${LIBRARIES} assimp)
	add_subdirectory(${ASSIMP_DIR})
	include_directories(
		${ASSIMP_DIR}/include
		${ASSIMP_DIR}/code
	)

	# Add the temporary output directories to the library path to make sure the
	# Assimp library can be found, even if it is not installed system-wide yet.
	#link_directories( ${Assimp_BINARY_DIR} ${AssetImporter_BINARY_DIR}/lib )
]]

		# glad
set(GLAD_DIR third-party/glad)
file(GLOB
	GLAD_SRC_FILES
	${GLAD_DIR}/src/*.c
)
set(THIRD_PARTY_SRC_FILES ${THIRD_PARTY_SRC_FILES} ${GLAD_SRC_FILES})

include_directories(${GLAD_DIR}/include)

		# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLFW_DIR third-party/glfw)
set(LIBRARIES ${LIBRARIES} glfw)
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)

		# glm
set(GLM_DIR third-party/glm)
#set(LIBRARIES ${LIBRARIES} glm)
include_directories(${GLM_DIR})

#[[
			# imgui
	set(IMGUI_DIR third-party/imgui)
	file(GLOB
		IMGUI_SRC_FILES
		${IMGUI_DIR}/*.cpp
	)

	file(
		GLOB
		IMGUI_INCLUDE_FILES
		${IMGUI_DIR}/*.h
	)

	add_library(
		imgui
		${IMGUI_SRC_FILES}
		${IMGUI_INCLUDE_FILES}
	)
	#set(LIBRARIES ${LIBRARIES} imgui)
	include_directories(${IMGUI_DIR})

]]

		# stb
set(STB_DIR third-party/stb)
#set(LIBRARIES ${LIBRARIES} stb)
include_directories(${STB_DIR})

# App files
set(APP_DIR "app/engine")

include_directories(${APP_DIR}/header/)

file(GLOB_RECURSE APP_SRC_FILES ${APP_DIR}/*.c ${APP_DIR}/*.cpp)
file(GLOB_RECURSE APP_INCLUDE_FILES ${APP_DIR}/*.h ${APP_DIR}/*.hpp)

# Executable
	# Create
add_executable(engine ${APP_SRC_FILES} ${APP_INCLUDE_FILES} ${THIRD_PARTY_SRC_FILES}) 

	# Link
target_link_libraries(engine ${LIBRARIES})

	# Copy useful file
install(
	DIRECTORY ${APP_DIR}/shaders/
	DESTINATION shaders/${APP}
	FILES_MATCHING PATTERN "*.glsl"
)