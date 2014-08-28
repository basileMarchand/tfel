if(ZSET_INSTALL_PATH)
  set(ZSETHOME "${ZSET_INSTALL_PATH}")
else(ZSET_INSTALL_PATH)
  set(ZSETHOME $ENV{ZSETHOME})
endif(ZSET_INSTALL_PATH)

if(ZSETHOME)
  find_path(ZSET_INCLUDE_DIR Behavior.h
    HINTS ${ZSETHOME}/include)
  if(ZSET_INCLUDE_DIR STREQUAL "ZSET_INCLUDE_DIR-NOTFOUND")
    message(FATAL_ERROR "Behavior.h not found")
  endif(ZSET_INCLUDE_DIR STREQUAL "ZSET_INCLUDE_DIR-NOTFOUND")
  add_definitions("-DZSET_ROOT=\\\"\"${ZSETHOME}\"\\\"")
  set(ZSET_ROOT ${ZSETHOME})
  message(STATUS "ZSET include files path detected: [${ZSET_INCLUDE_DIR}].")
else(ZSETHOME)
  message(WARNING "no ZSETHOME defined")
endif(ZSETHOME)