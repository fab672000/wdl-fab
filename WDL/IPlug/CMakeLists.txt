add_subdirectory(doc)

SET(IPLUG_SRC
	Hosts.cpp
	IBitmapMonoText.cpp
	IControl.cpp
	IGraphics.cpp
	IParam.cpp
	IPlugBase.cpp
	IPlugStructs.cpp
	IPopupMenu.cpp
	Log.cpp
	
	app_main.cpp
	app_dialog.cpp

)

SET(IPLUG_HEADERS
	Containers.h
	Hosts.h
	IAutoGUI.h
	IBitmapMonoText.h
	IControl.h
	IGraphics.h
	IGraphicsCarbon.h
	IGraphicsCocoa.h
	IGraphicsMac.h
	IGraphicsWin.h
	IKeyboardControl.h
	IMidiQueue.h
	IParam.h
	IPlugAAX.h
	IPlugAU.h
	IPlugBase.h
	IPlugOSDetect.h
	IPlugRTAS.h
	IPlugStandalone.h
	IPlugStructs.h
	IPlugVST.h
	IPlugVST3.h
	IPlug_include_in_plug_hdr.h
	IPlug_include_in_plug_src.h
  IPlugCocoaSrc.h
	IPopupMenu.h
	Log.h
	NChanDelay.h
  )
if (WIN32)
SET (IPLUG_SRC
	${IPLUG_SRC}
	IGraphicsWin.cpp
)
elseif(APPLE)
SET (IPLUG_SRC
	${IPLUG_SRC}
	IGraphicsCarbon.cpp
#	IPlugCocoa.mm
)
endif(WIN32)

add_library(iplug ${IPLUG_SRC} ${IPLUG_HEADERS} ${IPLUG_DOC_HEADERS} )

