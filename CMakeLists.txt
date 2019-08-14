set(COMPONENT_SRCDIRS speex/libspeex)
set(COMPONENT_ADD_INCLUDEDIRS speex/include)

register_component()

target_compile_options(${COMPONENT_LIB} PRIVATE -DFLOATING_POINT -DHAVE_CONFIG_H)
