target_sources(
    ${TARGET}
    PUBLIC
		FILE_SET
			HEADERS
			BASE_DIRS ${MALLOY_SERVER_BASE_DIR}
			FILES
				router.hpp
				router.cpp

	PRIVATE
		router_config.cpp
)
