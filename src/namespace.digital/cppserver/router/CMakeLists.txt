target_sources(
    ${TARGET}
    PUBLIC
		FILE_SET
			HEADERS
			BASE_DIRS ${CPPSERVER_BASE_DIR}
			FILES
				router.hpp
			

	PRIVATE
		router_config.cpp
		router.cpp
)
