LLVM_VERSION=14

LLVM_PATH=/opt/homebrew/opt/llvm@$(LLVM_VERSION)

CC=$(LLVM_PATH)/bin/clang++
LLC=$(LLVM_PATH)/bin/llc

CXX_FLAGS=-std=c++17
LLVM_CONFIG=$(LLVM_PATH)/bin/llvm-config
LLVM_INCLUDE=-I$(shell $(LLVM_CONFIG) --includedir)
LLVM_LIB=-L$(shell $(LLVM_CONFIG) --libdir)

CFLAGS=$(LLVM_INCLUDE) $(LLVM_LIB) $(CXX_FLAGS) -lunwind.1.0


PYTHON_VERSION=3.12

PYTHON_LDFLAGS=-L$(shell python3-config --configdir) -lpython$(PYTHON_VERSION)
VENV=$(CURDIR)/../venv
PY=$(VENV)/bin/python$(PYTHON_VERSION)


LDFLAGS = $(CFLAGS) $(PYTHON_LDFLAGS)


MAIN=main
MAIN_CPP=$(CURDIR)/$(MAIN).cpp
MAIN_PROG=$(CURDIR)/$(MAIN)

CALCULATION=calculation
CALCULATION_LLVM=$(CURDIR)/$(CALCULATION).ll
CALCULATION_OBJ=$(CURDIR)/$(CALCULATION).o
CALCULATION_PY=$(CURDIR)/$(CALCULATION).py


RUNTIME_LOOKUP=-undefined dynamic_lookup  # defer resolving NRT symbols until runtime


all: $(MAIN_PROG)


$(MAIN_PROG): $(CALCULATION_OBJ) $(MAIN_CPP)
	$(CC) $(MAIN_CPP) $(CALCULATION_OBJ) -o $(MAIN_PROG) $(LDFLAGS) $(RUNTIME_LOOKUP)


$(CALCULATION_OBJ): $(CALCULATION_LLVM)
	$(LLC) -filetype=obj $(CALCULATION_LLVM) -o $(CALCULATION_OBJ)


$(CALCULATION_LLVM): $(CALCULATION_PY)
	$(PY) $(CALCULATION_PY)


clean:
	@rm -f $(CALCULATION_OBJ) $(MAIN_PROG) $(CALCULATION_LLVM)


NUMBA_RUNTIME=$(VENV)/lib/python$(PYTHON_VERSION)/site-packages/numba/core/runtime
NUMBA_NRT_LIB=$(NUMBA_RUNTIME)/_nrt_python.cpython-$(subst .,,$(PYTHON_VERSION))-darwin.so


run:
	$(MAIN_PROG) $(NUMBA_NRT_LIB)
