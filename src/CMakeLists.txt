# Copyright (c) 2018-2020, The Arqma Network
# Copyright (c) 2014-2020, The Monero Project
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of
#    conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this list
#    of conditions and the following disclaimer in the documentation and/or other
#    materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may be
#    used to endorse or promote products derived from this software without specific
#    prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

function(morelo_private_headers group)
  source_group("${group}\\Private"
    FILES
      ${ARGN})
endfunction()

function(morelo_install_headers subdir)
  install(
    FILES       ${ARGN}
    DESTINATION "include/${subdir}"
    COMPONENT   development)
endfunction()

function(enable_stack_trace target)
  if(STACK_TRACE)
    set_property(TARGET ${target}
      APPEND PROPERTY COMPILE_DEFINITIONS "STACK_TRACE")
    if(STATIC)
      set_property(TARGET "${target}"
        APPEND PROPERTY LINK_FLAGS "-Wl,--wrap=__cxa_throw")
    endif()
  endif()
endfunction()

function(morelo_add_executable name)
  source_group("${name}"
    FILES
      ${ARGN})

  add_executable("${name}"
    ${ARGN})
  target_link_libraries("${name}"
    PRIVATE
      ${EXTRA_LIBRARIES})
  set_property(TARGET "${name}"
    PROPERTY
      FOLDER "prog")
  set_property(TARGET "${name}"
    PROPERTY
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  enable_stack_trace("${name}")
endfunction()

function(morelo_add_library name)
    morelo_add_library_with_deps(NAME "${name}" SOURCES ${ARGN})
endfunction()

function(morelo_add_library_with_deps)
  cmake_parse_arguments(morelo_ADD_LIBRARY "" "NAME" "DEPENDS;SOURCES" ${ARGN})
  source_group("${morelo_ADD_LIBRARY_NAME}" FILES ${morelo_ADD_LIBRARY_SOURCES})

  # Define a ("virtual") object library and an actual library that links those
  # objects together. The virtual libraries can be arbitrarily combined to link
  # any subset of objects into one library archive. This is used for releasing
  # libwallet, which combines multiple components.
  set(objlib obj_${morelo_ADD_LIBRARY_NAME})
  add_library(${objlib} OBJECT ${morelo_ADD_LIBRARY_SOURCES})
  add_library("${morelo_ADD_LIBRARY_NAME}" $<TARGET_OBJECTS:${objlib}>)
  if(morelo_ADD_LIBRARY_DEPENDS)
    add_dependencies(${objlib} ${morelo_ADD_LIBRARY_DEPENDS})
  endif()
  set_property(TARGET "${morelo_ADD_LIBRARY_NAME}" PROPERTY FOLDER "libs")
  target_compile_definitions(${objlib}
    PRIVATE $<TARGET_PROPERTY:${morelo_ADD_LIBRARY_NAME},INTERFACE_COMPILE_DEFINITIONS>)
endfunction()

include(Version)
morelo_add_library(version SOURCES ${CMAKE_BINARY_DIR}/version.cpp DEPENDS genversion)

option(MORELO_SLIM "Build only daemon, wallet-rpc and wallet-cli" OFF)

add_subdirectory(common)
add_subdirectory(crypto)
add_subdirectory(ringct)
add_subdirectory(checkpoints)
add_subdirectory(cryptonote_basic)
add_subdirectory(arqma_mq)
add_subdirectory(cryptonote_core)
add_subdirectory(lmdb)
add_subdirectory(multisig)
add_subdirectory(net)
add_subdirectory(rpc_sig)
add_subdirectory(blockchain_db)
add_subdirectory(mnemonics)
add_subdirectory(rpc)
if(NOT IOS)
  add_subdirectory(serialization)
endif()
add_subdirectory(wallet)
if(NOT IOS)
  add_subdirectory(p2p)
endif()
add_subdirectory(cryptonote_protocol)
if(NOT IOS)
  add_subdirectory(simplewallet)
  add_subdirectory(daemonizer)
  add_subdirectory(daemon)
  if(NOT MORELO_SLIM)
    add_subdirectory(gen_ssl_cert)
    #add_subdirectory(gen_multisig)
    add_subdirectory(blockchain_utilities)
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_subdirectory(debug_utilities)
endif()

if(PER_BLOCK_CHECKPOINT)
  add_subdirectory(blocks)
endif()

add_subdirectory(device)
