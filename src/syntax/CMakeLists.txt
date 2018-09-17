
aux_source_directory( . DIR_SRCS)

foreach( src ${DIR_SRCS})
	get_filename_component( name ${src} NAME_WE)
	add_executable( ${name}
		${src}
		${tools_src}
	)
endforeach()

