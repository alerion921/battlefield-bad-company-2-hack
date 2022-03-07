SET(PROJ_NAME hacklib_bf)
PROJECT(${PROJ_NAME})

SET(FILES_BFBC2
    BFBC2_Cam.h
    BFBC2_Input.h
    BFBC2_Library.h
    BFBC2_Main.h
    BFBC2_Mystate.h
    BFBC2_Skeleton.h
    BFBC2_Weapon.h)

SET(FILES_BF3
    BF3_Cam.h
    BF3_Input.h
    BF3_Library.h
    BF3_Main.h
    BF3_Skeleton.h
    BF3_Weapon.h)

SET(FILES_BF4
    BF4_Cam.h
    BF4_Input.h
    BF4_Library.h
    BF4_Main.h
    BF4_Weapon.h)

SET(FILES_SWBF
    SWBF_Cam.h
    SWBF_Input.h
    SWBF_Library.h
    SWBF_Main.h)

SET(FILES_ABSTRACTION_H
    BattlefieldData.h
    CamData.h
    OwnData.h
    Player.h
    Skeleton.h
    Weapon.h)

SET(FILES_ABSTRACTION_CPP
    BattlefieldData.cpp
    CamData.cpp
    OwnData.cpp
    Player.cpp
    Skeleton.cpp
    Weapon.cpp)

SET(FILES_H
    Input.h
    Aimbot.h
    BattlefieldDataMgr.h
    EspDrawer.h
    Hack.h
    main.h)

SET(FILES_CPP
    Input.cpp
    Aimbot.cpp
    BattlefieldDataMgr.cpp
    Error.cpp
    EspDrawer.cpp
    Hack.cpp
    main.cpp)


ADD_LIBRARY(${PROJ_NAME} SHARED
    main.cpp
    ${FILES_BFBC2}
    ${FILES_BF3}
    ${FILES_BF4}
    ${FILES_SWBF}
    ${FILES_ABSTRACTION_H}
    ${FILES_ABSTRACTION_CPP}
    ${FILES_H}
    ${FILES_CPP})

SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES FOLDER ${PROJ_NAME})

TARGET_LINK_LIBRARIES(${PROJ_NAME} hacklib)

SOURCE_GROUP("Game specific\\bfbc2" FILES ${FILES_BFBC2})
SOURCE_GROUP("Game specific\\bf3" FILES ${FILES_BF3})
SOURCE_GROUP("Game specific\\bf4" FILES ${FILES_BF4})
SOURCE_GROUP("Game specific\\swbf" FILES ${FILES_SWBF})
SOURCE_GROUP("Game abstraction\\Header Files" FILES ${FILES_ABSTRACTION_H})
SOURCE_GROUP("Game abstraction\\Source Files" FILES ${FILES_ABSTRACTION_CPP})