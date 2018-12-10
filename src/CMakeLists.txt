set(project_name adventofcode)

############### Files #################

set (SOURCES main.cpp)
set (HEADERS main.cpp)

#######################################


########### Group Sources #############

source_group("Source" FILES ${SOURCES} ${HEADERS})

#######################################



########### Include dirs ##############

include_directories (src)

#######################################


######### Config Executable ###########

add_executable(${project_name} ${SOURCES} ${HEADERS})

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