if(NOT EXISTS "${DEST}")
    file(COPY "${SRC}" DESTINATION "$ENV{HOME}/.config/nitch++")
    file(RENAME "$ENV{HOME}/.config/nitch++/example_config.toml" "${DEST}")
endif()
