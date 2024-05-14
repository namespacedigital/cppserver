target_sources(
    ${TARGET}
    PUBLIC
		FILE_SET
			HEADERS
			BASE_DIRS ${CPPSERVER_BASE_DIR}
			FILES
				attribute.hpp
				attributes.hpp
				router.hpp
				route.hpp
				request.hpp
				header.hpp
				response.hpp
				listener.hpp
				http_worker.hpp
				ws_worker.hpp
				ws_handler.hpp
				ws_context.hpp
				mime_type.hpp

		PRIVATE
			attribute.cpp
			attributes.cpp
			router.cpp
			route.cpp
			listener.cpp
			http_worker.cpp
	
)
