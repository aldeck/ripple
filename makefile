TYPE = APP

NAME = Ripple

SRCS = App.cpp
SRCS += MainWindow.cpp
SRCS += MathUtils.cpp
SRCS += TestRenderer.cpp
SRCS += WaveRenderer.cpp

LIBS = be game

RDEFS = Ripple.rdef

## include the makefile-engine
include $(BUILDHOME)/etc/makefile-engine
