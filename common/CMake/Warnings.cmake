if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Werror)
else()
  message(SEND_ERROR "Compiler is neither Clang nor GCC, no compiler warnings set. Consider patching the warnings file.")
endif()
