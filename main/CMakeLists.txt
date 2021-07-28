set(COMPONENT_SRCS
    "main.cpp"
    "mainStarter.cpp"
)

set(COMPONENT_ADD_INCLUDEDIRS
)

register_component()

target_compile_options(${COMPONENT_LIB} PRIVATE 
)
