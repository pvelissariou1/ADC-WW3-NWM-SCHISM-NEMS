foreach (flags_var_to_scrub
	CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
	CMAKE_C_FLAGS_RELEASE
	CMAKE_C_FLAGS_RELWITHDEBINFO
	CMAKE_C_FLAGS_MINSIZEREL)
	string (REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
	"${flags_var_to_scrub}" "${${flags_var_to_scrub}}")
endforeach()
