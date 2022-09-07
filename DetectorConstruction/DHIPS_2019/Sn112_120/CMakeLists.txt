option(ROTATE_TARGET "Rotate target by 180 degrees" OFF)
configure_file(
	"${CMAKE_CURRENT_LIST_DIR}/Sn112_120Config.h.in"
	"${CMAKE_CURRENT_LIST_DIR}/Sn112_120Config.h"
)
