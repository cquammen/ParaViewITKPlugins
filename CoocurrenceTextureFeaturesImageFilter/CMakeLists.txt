# SET(PLUGIN  PVPCoocurrenceTextureFeaturesImageFilter)
# SET(XML	CoocurrenceTextureFeaturesImageFilter.xml)
# 
# ADD_PARAVIEW_PLUGIN(${PLUGIN} 1.0
#   SERVER_MANAGER_XML ${XML}
#   SERVER_MANAGER_SOURCES vtkCoocurrenceTextureFeaturesImageFilter.cxx
#   SOURCES ${XML}
# )
# 
# # This lets you build INSTALL and have it automatically drop the
# # resulting DLLs in the directory where they will be autoloaded
# # by ParaView.
# install(TARGETS ${PLUGIN} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
# 
# TARGET_LINK_LIBRARIES(${PLUGIN} ${ITK_LIBRARIES}
#   ParaViewITKPluginsCommon
# )

paraview_add_plugin(CoocurrenceTextureFeaturesImageFilter
  VERSION "1.0"
  REQUIRED_ON_SERVER
  MODULES ITK::vtkCoocurrenceTextureFeaturesImageFilter
  MODULE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/vtk/vtk.module"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/vtk.module"
  SERVER_MANAGER_XML CoocurrenceTextureFeaturesImageFilter.xml
)
