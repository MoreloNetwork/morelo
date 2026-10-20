set(LIBZMQ_PREFIX ${CMAKE_BINARY_DIR}/libzmq)
set(ZeroMQ_VERSION 4.3.5)
set(LIBZMQ_URL https://github.com/zeromq/libzmq/releases/download/v${ZeroMQ_VERSION}/zeromq-${ZeroMQ_VERSION}.tar.gz)
set(LIBZMQ_HASH SHA512=a71d48aa977ad8941c1609947d8db2679fc7a951e4cd0c3a1127ae026d883c11bd4203cf315de87f95f5031aec459a731aec34e5ce5b667b8d0559b157952541)

if(LIBZMQ_TARBALL_URL)
    # make a build time override of the tarball url so we can fetch it if the original link goes away
    set(LIBZMQ_URL ${LIBZMQ_TARBALL_URL})
endif()

if(DEPENDS)
  add_library(lib-zmq STATIC IMPORTED GLOBAL)
  set_target_properties(lib-zmq PROPERTIES IMPORTED_LOCATION ${ZMQ_LIBRARIES})
  target_link_libraries(lib-zmq INTERFACE sodium)
  target_include_directories(lib-zmq INTERFACE ${ZMQ_INCLUDE_DIRS})
  message(STATUS "${ZMQ_INCLUDE_DIRS} ${ZMQ_LIBRARIES}")
else()
  file(MAKE_DIRECTORY ${LIBZMQ_PREFIX}/include)

  include(ExternalProject)
  include(ProcessorCount)

  set(ZeroMQ_CONFIGURE ./configure --prefix=${LIBZMQ_PREFIX})

  if(APPLE OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(ZeroMQ_CONFIGURE ${ZeroMQ_CONFIGURE} --host=x86_64-apple-darwin)
  endif()

  set(ZeroMQ_CONFIGURE ${ZeroMQ_CONFIGURE} --without-docs --enable-static=yes --enable-shared=no --with-libsodium=yes --with-pgm=no --with-norm=no --disable-perf --disable-Werror --disable-drafts --disable-libunwind --disable-libbsd --disable-curve-keygen)

  if(NOT (APPLE OR WIN32 OR MSVC OR MINGW))
    set(ZeroMQ_CONFIGURE ${ZeroMQ_CONFIGURE} --with-pic)
  endif()

  ExternalProject_Add(libzmq_external
      BUILD_IN_SOURCE ON
      PREFIX ${LIBZMQ_PREFIX}
      URL ${LIBZMQ_URL}
      URL_HASH ${LIBZMQ_HASH}
      CONFIGURE_COMMAND ${ZeroMQ_CONFIGURE}
      BUILD_COMMAND make -j${PROCESSOR_COUNT}
      INSTALL_COMMAND ${MAKE}
      BUILD_BYPRODUCTS ${LIBZMQ_PREFIX}/lib/libzmq.a ${LIBZMQ_PREFIX}/include
  )

  add_library(lib-zmq STATIC IMPORTED GLOBAL)
  add_dependencies(libzmq libzmq_external)
  target_link_libraries(lib-zmq INTERFACE sodium)
  set_property(TARGET lib-zmq PROPERTY IMPORTED_LOCATION ${LIBZMQ_PREFIX}/lib/libzmq.a)
  target_include_directories(lib-zmq INTERFACE ${LIBZMQ_PREFIX}/include)

  set(ZMQ_INCLUDE_DIRS ${LIBZMQ_PREFIX}/include CACHE STRING "ZMQ Include path")

  link_dep_libs(lib-zmq INTERFACE "${ZMQ_LIBRARY_DIRS}" ${LIBZMQ_PREFIX}/lib/libzmq.a)
  target_include_directories(lib-zmq INTERFACE ${ZMQ_INCLUDE_DIRS})
endif()
