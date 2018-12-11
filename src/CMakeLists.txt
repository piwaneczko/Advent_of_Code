set(project_name adventofcode)

############### Files #################

set (SOURCES main.cpp)
set (HEADERS )
set (RESOURCES resources/input.txt)

#######################################


########### Group Sources #############

source_group("Source" FILES ${SOURCES} ${HEADERS})
source_group("Resources" FILES ${RESOURCES})

#######################################



########### Include dirs ##############

include_directories (src)

#######################################


######### Config Executable ###########

add_executable(${project_name} ${SOURCES} ${HEADERS} ${RESOURCES})

#######################################

############ Installation #############

install(TARGETS ${project_name} EXPORT ${project_name}_config
        ARCHIVE DESTINATION lib COMPONENT lib
        LIBRARY DESTINATION lib COMPONENT lib
        RUNTIME DESTINATION bin COMPONENT bin
)

install(DIRECTORY resources
    DESTINATION bin
    COMPONENT bin)
    
    
#######################################