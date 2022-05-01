idf_component_register(
    SRC_DIRS speex/libspeex
    INCLUDE_DIRS speex/include
    PRIV_INCLUDE_DIRS speex/libspeex
)

target_compile_options(${COMPONENT_LIB} PRIVATE -DFLOATING_POINT -DHAVE_CONFIG_H -Wno-shift-negative-value)
