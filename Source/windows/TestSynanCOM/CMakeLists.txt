project(TestSynanCOM)

add_executable (${PROJECT_NAME}
        TestSynanCOM.cpp
        ../common/COMSyntaxHolder.cpp
)

add_dependencies(${PROJECT_NAME}
    agramtab
    SynAn
    Lemmatizer
    MAPost
    graphan
)


target_link_libraries(${PROJECT_NAME}
    aot_common
)


add_test(
    NAME Synan_COM_Russian 
    COMMAND TestSynanCOM test/Russian/text.txt
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_test(
    NAME Synan_COM_Russian_cmp 
    COMMAND ${CMAKE_COMMAND} -E compare_files test/Russian/canon.synan test/Russian/text.txt.synan
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
