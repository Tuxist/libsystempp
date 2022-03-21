set(LIBCXX_ABI_NAMESPACE "__libsystempp")
set(LLVM_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(libcxxabi)
add_subdirectory(libcxx)
add_subdirectory(libunwind)
