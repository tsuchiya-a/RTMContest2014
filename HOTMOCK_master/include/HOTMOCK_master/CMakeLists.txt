set(hdrs HOTMOCK_master.h
    hotmockclient.h
    hotmocksetting.h
    dynamic_port.hpp
    VectorConvert.h
    PARENT_SCOPE
    )

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

