declare_cmake_min_version()

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/BinDict)

macro (CompileSimpleGrammar name grmFile tableFile)
    add_custom_target( 
        ${name}
        SOURCES ${tableFile}                     
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    add_custom_command(
        OUTPUT ${tableFile}
        COMMAND SimpleGrammarPrecompiled --language German --input ${grmFile} --output -
        DEPENDS SimpleGrammarPrecompiled ${grmFile}
        COMMENT "Compile ${grmFile} "
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        VERBATIM
    )

endmacro(CompileSimpleGrammar)



add_subdirectory (Obor)
add_subdirectory (GerObor)
add_subdirectory (EngObor)
add_subdirectory (Aoss)
add_subdirectory (Ross)
add_subdirectory (Collocs)
add_subdirectory (EngCollocs)
add_subdirectory (TimeRoss)
add_subdirectory (Trigram)
add_subdirectory (GerSynan)
add_subdirectory (SimpleGrammar)
add_subdirectory (SrcBinDict)

add_custom_target ( 
    GermanGLRSynan
    DEPENDS GerNPGLRSynan ExampleGLRSynan PersonGLRSynan
)


