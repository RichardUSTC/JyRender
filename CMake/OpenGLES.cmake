## OpenGL ES Emulator
if(WIN32)

    set(OPENGLES_EMULATOR_PATH
        ${CMAKE_SOURCE_DIR}/External/Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit_unsigned
    )

    if(NOT TARGET CopyEmulatorDlls)
        add_custom_target(CopyEmulatorDlls ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${OPENGLES_EMULATOR_PATH}/libEGL.dll
            ${OPENGLES_EMULATOR_PATH}/libGLESv2.dll
            ${OPENGLES_EMULATOR_PATH}/libMaliEmulator.dll
            ${OPENGLES_EMULATOR_PATH}/log4cplus.dll
            ${OPENGLES_EMULATOR_PATH}/msvcp110.dll
            ${OPENGLES_EMULATOR_PATH}/msvcp120.dll
            ${OPENGLES_EMULATOR_PATH}/msvcr110.dll
            ${OPENGLES_EMULATOR_PATH}/msvcr120.dll
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${OPENGLES_EMULATOR_PATH}/openglessl
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/openglessl
        )
    endif()

    # GLESv2
    if(NOT TARGET GLES)
        find_library(OPENGLES3_LIBRARY
            NAMES libGLESv2 GLESv2
            PATHS ${OPENGLES_EMULATOR_PATH}
            )
        add_library(GLESv2 SHARED IMPORTED)
        set_target_properties(GLESv2 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${OPENGLES_EMULATOR_PATH}/include
        )
        if(WIN32)
            set_target_properties(GLESv2 PROPERTIES
                IMPORTED_IMPLIB ${OPENGLES3_LIBRARY})
        else()
            set_target_properties(GLESv2 PROPERTIES
                IMPORTED_LOCATION ${OPENGLES3_LIBRARY})
        endif()
    endif()

    #EGL
    if(NOT TARGET EGL)
        find_library(EGL_LIBRARY
            NAMES libEGL EGL
            PATHS ${OPENGLES_EMULATOR_PATH}
            )
        add_library(EGL SHARED IMPORTED)
        set_target_properties(EGL PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${OPENGLES_EMULATOR_PATH}/include
        )
        if(WIN32)
            set_target_properties(EGL PROPERTIES
                IMPORTED_IMPLIB ${EGL_LIBRARY})
        else()
            set_target_properties(GLESv2 PROPERTIES
                IMPORTED_LOCATION ${EGL_LIBRARY})
        endif()
    endif()

endif()