set(UMUSIC_APP umusic)

set(UMUSIC_APP_HEADERS
    auth.h
    usubsonic.h
    uartist.h
    ualbum.h
    usong.h
    umodel.h
    )

set(UMUSIC_APP_SRC
    auth.cpp
    main.cpp
    usubsonic.cpp
    uartist.cpp
    ualbum.cpp
    usong.cpp
    umodel.cpp
    )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QT_INCLUDE_DIR}
    ${KQOAUTH_INCLUDE_DIRS}
    ${QJSON_INCLUDE_DIRS}
    ${QT_PHONON_INCLUDE_DIR}
    )

qt4_wrap_cpp(UMUSIC_APP_MOC_SRCS ${UMUSIC_APP_HEADERS})
add_executable(${UMUSIC_APP}
    ${UMUSIC_APP_MOC_SRCS}
    ${UMUSIC_APP_SRC}
    )

target_link_libraries(${UMUSIC_APP}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${QT_PHONON_LIBRARY}
    ${KQOAUTH_LDFLAGS}
    ${QJSON_LDFLAGS}
    )
