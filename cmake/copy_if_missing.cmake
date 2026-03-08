if(NOT EXISTS "${DEST}")
    file(COPY_FILE "${SRC}" "${DEST}")
endif()
