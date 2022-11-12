add_library(tasan tiny_sanitizer.cpp)
install(TARGETS tasan DESTINATION include)