########################################################################
### Panagiotis Velissariou <panagiotis.velissariou@noaa.gov> - 18/05/2021
###
### Version: 1.0 (18/05/2021)
########################################################################

# Location of source code and installation
SCHISM_ROOTDIR?=$(ROOTDIR)/SCHISM
SCHISM_SRCDIR?=$(SCHISM_ROOTDIR)/src
SCHISM_BLDDIR?=$(SCHISM_ROOTDIR)/build
SCHISM_BINDIR?=$(ROOTDIR)/SCHISM_INSTALL

# Location of the ESMF makefile fragment for this component:
schism_mk = $(SCHISM_BINDIR)/schism.mk
all_component_mk_files+=$(schism_mk)

# Make sure the expected directories exist and are non-empty:
$(call require_dir,$(SCHISM_SRCDIR),SCHISM source directory)

# ENV for SCHISM - exchange with NEMS ENV

SCHISM_ALL_OPTS= \
  COMP_SRCDIR="$(SCHISM_SRCDIR)" \
  COMP_BINDIR="$(SCHISM_BINDIR)" \
  MACHINE_ID="$(MACHINE_ID)"

########################################################################

# Rule for building this component:

# These line(s) will go away - for reference only
SCHISM_NETCDF=-DNetCDF_FORTRAN_DIR=/project/opt/software/netcdf/4.7.0/intel_2020.0 -DCMAKE_Fortran_COMPILER=mpiifort -DCMAKE_CXX_COMPILER=mpiicpc   -DNetCDF_C_DIR=`nc-config --prefix` -DCMAKE_C_COMPILER=mpiicc -DNetCDF_INCLUDE_DIR=/project/opt/software/netcdf/4.7.0/intel_2020.0/include

build_SCHISM: $(schism_mk)

$(schism_mk): configure $(CONFDIR)/configure.nems
   ### Configure CMake build for SCHISM
	+$(MODULE_LOGIC); echo "SCHISM_SRCDIR = $(SCHISM_SRCDIR)"; exec cmake -S $(SCHISM_SRCDIR) -B $(SCHISM_ROOTDIR)/build -DCMAKE_VERBOSE_MAKEFILE=TRUE
   ### Compile the SCHISM components
	+cd $(SCHISM_BLDDIR); exec $(MAKE) pschism
#	cd $(SCHISM_BLDDIR); exec $(MAKE) install
#	+$(MODULE_LOGIC); cd $(SCHISM_SRCDIR)/../schism-esmf/src/schism; exec $(MAKE) $(SCHISM_ALL_OPTS) nuopcinstall \
#          DESTDIR=/ "INSTDIR=$(SCHISM_BINDIR)"
#	@echo ""
#	test -d "$(SCHISM_BINDIR)"
#	@echo ""
#	test -s $(schism_mk)
#	@echo ""

########################################################################

# Rule for cleaning the SRCDIR and BINDIR:

clean_SCHISM:
#	+cd $(SCHISM_SRCDIR); exec $(MAKE) -f build/Makefile -k clean
#	@echo ""

distclean_SCHISM: clean_SCHISM
	+cd $(SCHISM_SRCDIR)/work ; exec $(MAKE) -k distclean
	rm -rf $(SCHISM_BINDIR)
	@echo ""

distclean_NUOPC:
	+cd $(SCHISM_SRCDIR)/../schism-esmf/src/schism ; exec rm -f *.o *.mod *.a schism.mk  # make clean/distclean here
	rm -rf $(SCHISM_BINDIR)
	@echo ""
