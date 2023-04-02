cmake_minimum_required(VERSION 3.18.0)
project(pwl)
#
set(DOC_DIR                                 "${CMAKE_SOURCE_DIR}/doc")
set(DOXYGEN_CONF_FILE                       "doxygen.conf")
# doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${DOC_DIR}/${DOXYGEN_CONF_FILE} @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} "${DOC_DIR}/${DOXYGEN_CONF_FILE}"
        WORKING_DIRECTORY ${DOC_DIR}
        COMMENT "Generate API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
