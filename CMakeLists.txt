CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

# clang -std=c++11 -g
ADD_COMPILE_OPTIONS(-std=c++11)


# Generate Unit Test Execute File
AUX_SOURCE_DIRECTORY(./test TEST)
AUX_SOURCE_DIRECTORY(./src/utils UTILS)
ADD_EXECUTABLE(test_output ${TEST} ${UTILS})

# Generate Examples Execute File
AUX_SOURCE_DIRECTORY(./examples EXAMPLES)
ADD_EXECUTABLE(ex_output ${EXAMPLES} ${UTILS})