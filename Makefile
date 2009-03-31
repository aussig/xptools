BUILDDIR	:= build
LIBRARIES	:= ./libs/local/.xpt_libs
# default to public debug_opt configuration
ifndef conf
conf		:= debug_opt
endif
PLATFORM	:= $(shell uname)
ifneq (, $(findstring MINGW, $(PLATFORM)))
	PLATFORM	:= Mingw
endif
TARGETDIR	:= $(BUILDDIR)/$(PLATFORM)/$(conf)

.SILENT:
.PHONY: all
.PHONY: clean
.PHONY: distclean

.PHONY: libs
.PHONY: ObjView
.PHONY: WED
.PHONY: DSFTool
.PHONY: DDSTool
.PHONY: ObjConverter
.PHONY: MeshTool
.PHONY: RenderFarm
.PHONY: fonttool

all: WED MeshTool ObjView DSFTool DDSTool ObjConverter RenderFarm

libs: $(LIBRARIES)

$(LIBRARIES):
	$(MAKE) -C "./libs" all

ObjView: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

WED: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

DSFTool: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

DDSTool: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

ObjConverter: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

MeshTool: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

XGrinder: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

RenderFarm: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

fonttool: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/$@ -f makerules/rules.mk

ac3d: libs
	$(MAKE) conf=$(conf) cross=$(cross) BUILDDIR=$(TARGETDIR) TARGET=$(TARGETDIR)/XPlaneSupportLin.p -f makerules/rules.mk
	
clean:
	-rm -rf $(BUILDDIR)

distclean:	clean
	$(MAKE) -C "./libs" clean
