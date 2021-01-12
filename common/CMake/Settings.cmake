set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_options(-Wall -Wextra -Wshadow -Werror)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUGGING)
  add_compile_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
  add_link_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
endif()
