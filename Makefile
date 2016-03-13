# Location of pepper sdk. Environment variable should be set.
PEPPER=$(PEPPER_DIR)

PLATFORM=mac
BINDIR=bin
BUILD_VARIANT=Debug
TOOLCHAIN=$(PEPPER)/toolchain/$(PLATFORM)_pnacl/$(BINDIR)
CXX=$(TOOLCHAIN)/pnacl-clang++
LINK=$(CXX)
FINALIZE=$(TOOLCHAIN)/pnacl-finalize
PPAPI_INCLUDE=$(PEPPER)/include
PPAPI_LIBDIR=$(PEPPER)/lib/pnacl/$(BUILD_VARIANT)
LIBS=-lppapi_simple_cpp -lnacl_io -lsdk_util -lppapi_cpp -lppapi -lpthread

all: game.pexe

clean:
	rm *.pexe *.bc *.o test/*.out *.exe

debug: minimal_unstripped.bc
	/Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome --enable-nacl --enable-nacl-debug --no-sandbox http://localhost:8000/ &
	$(PEPPER_DIR)/toolchain/mac_x86_newlib/bin/x86_64-nacl-gdb -x script/setup-gdb

test_geometry.o: test/test_geometry.cc
	g++ -c -o test_geometry.o -pthread test/test_geometry.cc

testable_geometry.o: engine/geometry.cc
	g++ -c -o testable_geometry.o -pthread engine/geometry.cc

testable_util.o: engine/util.cc
	g++ -c -o testable_util.o -pthread engine/util.cc

test_geometry: test_geometry.o testable_geometry.o testable_util.o
	g++ -o test_geometry.exe test_geometry.o testable_geometry.o testable_util.o
	./test_geometry.exe > test/test_geometry.out
	diff test/test_geometry.out test/test_geometry.fixture

geometry.o: engine/geometry.cc
	$(CXX) -c -g -o geometry.o -pthread engine/geometry.cc

encoding.o: engine/encoding.cc
	$(CXX) -c -g -o encoding.o -pthread engine/encoding.cc

rendering.o: engine/rendering.cc
	$(CXX) -c -g -o rendering.o -pthread engine/rendering.cc

util.o: engine/util.cc
	$(CXX) -c -g -o util.o -pthread engine/util.cc

game.o: game.cc
	$(CXX) -c -g -o game.o -pthread game.cc

testable_game.o: game.cc
	g++ -c -o testable_game.o -pthread game.cc

testable_encoding.o: engine/encoding.cc
	g++ -c -o testable_encoding.o -pthread engine/encoding.cc

test_game.o: test/test_game.cc
	g++ -c -o test_game.o -pthread test/test_game.cc

rendering_stub.o: test/rendering_stub.cc
	g++ -c -o rendering_stub.o -pthread test/rendering_stub.cc

test_game: test_game.o testable_game.o rendering_stub.o testable_geometry.o testable_encoding.o testable_util.o
	g++ -o test_game.exe test_game.o testable_game.o rendering_stub.o testable_geometry.o testable_encoding.o testable_util.o
	cat test/frame.txt | ./test_game.exe

nacl.o: platform/nacl.cc
	$(CXX) -c -g -o nacl.o -pthread -I $(PPAPI_INCLUDE) platform/nacl.cc

minimal_unstripped.bc: game.o nacl.o encoding.o geometry.o rendering.o util.o
	$(LINK) -o minimal_unstripped.bc -pthread -L "$(PPAPI_LIBDIR)" game.o nacl.o encoding.o geometry.o rendering.o util.o $(LIBS)

game.pexe: minimal_unstripped.bc
	$(FINALIZE) -o game.pexe minimal_unstripped.bc
