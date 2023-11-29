target_sources(
    ${TARGET}
    PUBLIC
		FILE_SET
			HEADERS
			BASE_DIRS ${CPPSERVER_BASE_DIR}
			FILES
				file_content.hpp
)
