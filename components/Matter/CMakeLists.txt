set(PRIV_REQUIRES_LIST device esp_matter esp_matter_console app_reset)

idf_component_register(
    SRC_DIRS     "."
    INCLUDE_DIRS "."
    PRIV_REQUIRES   ${PRIV_REQUIRES_LIST}
    REQUIRES Utilities )    
