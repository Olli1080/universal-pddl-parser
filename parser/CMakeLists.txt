project(parser
  LANGUAGES CXX
)
add_library(${PROJECT_NAME})

set(INCLUDE_DIR include/universal-pddl/parser)

#generate_export_header(${PROJECT_NAME} 
#  EXPORT_MACRO_NAME
#    ICL_CORE_IMPORT_EXPORT
#  EXPORT_FILE_NAME
#    "${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR}/ImportExport.h"
#)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/universal-pddl>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(${PROJECT_NAME}
  PRIVATE    
    src/Action.cpp    
    src/And.cpp
    src/Derived.cpp
    src/Equals.cpp
    src/Exists.cpp
    src/Expression.cpp
    src/Forall.cpp
    src/Function.cpp
    src/FunctionModifier.cpp
    src/Ground.cpp
    src/GroundFunc.cpp
    src/Lifted.cpp
    src/Not.cpp
    src/Oneof.cpp
    src/Or.cpp
    src/ParamCond.cpp
    src/TemporalAction.cpp
    src/TypeGround.cpp
    src/When.cpp
  PUBLIC FILE_SET HEADERS 
  BASE_DIRS ${INCLUDE_DIR}
  FILES
    ${INCLUDE_DIR}/Action.h
    ${INCLUDE_DIR}/And.h
    ${INCLUDE_DIR}/Basic.h
    ${INCLUDE_DIR}/CondIter.h
    ${INCLUDE_DIR}/Condition.h
    ${INCLUDE_DIR}/Derived.h
    ${INCLUDE_DIR}/Domain.h
    ${INCLUDE_DIR}/EitherType.h
    ${INCLUDE_DIR}/Equals.h
    ${INCLUDE_DIR}/Exists.h
    ${INCLUDE_DIR}/Expression.h
    ${INCLUDE_DIR}/Filereader.h
    ${INCLUDE_DIR}/Forall.h
    ${INCLUDE_DIR}/Function.h
    ${INCLUDE_DIR}/FunctionModifier.h
    ${INCLUDE_DIR}/Ground.h
    ${INCLUDE_DIR}/GroundFunc.h
    ${INCLUDE_DIR}/Instance.h
    ${INCLUDE_DIR}/Lifted.h
    ${INCLUDE_DIR}/Not.h
    ${INCLUDE_DIR}/Oneof.h
    ${INCLUDE_DIR}/Or.h
    ${INCLUDE_DIR}/ParamCond.h
    ${INCLUDE_DIR}/Task.h
    ${INCLUDE_DIR}/TemporalAction.h
    ${INCLUDE_DIR}/TokenStruct.h
    ${INCLUDE_DIR}/Type.h
    ${INCLUDE_DIR}/TypeGround.h
    ${INCLUDE_DIR}/When.h
#    ${INCLUDE_DIR}/ImportExport.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

install(
  TARGETS 
    ${PROJECT_NAME}
  #EXPORT
  #  gpu_voxels-targets
  LIBRARY
    DESTINATION lib
  FILE_SET HEADERS
    DESTINATION ${INCLUDE_DIR}
)