CXX = g++
STD = -std=c++17
CFLAG = -Wall -Wextra -Werror -pedantic
TEST_FILES = tests.cpp Model/parsingmodel.cpp
TEXI2DVI = makeinfo --html

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	GTEST:= $(shell pkg-config --cflags --libs gtest 2>/dev/null)
	ifeq ($(GTEST),)
		GTEST:=-DGTEST_HAS_PTHREAD=1 -lgtest -pthread
	endif
	LEAKS_CMD = CK_FORK=no valgrind --tool=memcheck --leak-check=full --track-origins=yes  --leak-check=full --show-leak-kinds=all ./test
endif
ifeq ($(UNAME), Darwin)
	LEAKS_CMD = CK_FORK=no leaks --atExit -- ./test
	GTEST = -fsanitize=address -DGTEST_HAS_PTHREAD=1 -lgtest -pthread
endif

all: clean install tests

build:
	mkdir build
	cd ./build/ && qmake6 ../ && make && mv 3D_Viewer_v2.app 3DVIEWERv2.app

install: build
	mkdir $(HOME)/Desktop/3DVIEWERv2/
	cp -rf build/3DVIEWERv2.app $(HOME)/Desktop/3DVIEWERv2/
	make clean

uninstall:
	rm -rf $(HOME)/Desktop/3DVIEWERv2/

dvi:
	@$(TEXI2DVI) 3DViewer_v2.1.texi
	open 3DViewer_v2.1/index.html

dist:
	rm -rf 3DVIEWERv_2
	mkdir 3DVIEWERv_2
	mkdir 3DVIEWERv_2/src
	cp Makefile *.texi *.cpp *.pro .clang-format 3DVIEWERv_2/src/
	cp -R Controller Model Objects OpenGL QtGifImage Resources Settings View 3DVIEWERv_2/src
	tar -cvzf 3DVIEWERv_2.tar.gz 3DVIEWERv_2
	mv 3DVIEWERv_2.tar.gz $(HOME)/Desktop/
	rm -rf 3DVIEWERv_2/

tests: clean
	$(CXX) $(STD) $(CFLAG) -o test $(TEST_FILES) $(GTEST)
	./test

gcov_report: clean
	$(CXX) $(STD) $(CFLAG) -o test $(TEST_FILES) $(GTEST) --coverage
	./test || true
	lcov -t "test" -o test.info --no-external -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *.o  *.gc* report *.a tests test test.info build *.out *.pro.user *.pro.user.* *.dSYM 3DViewer_v2.1

rebuild: clean all

leaks: tests
	CK_FORK=no leaks --atExit -- ./test

clang:
	clang-format -i Controller/*.cpp Controller/*.h \
									Model/*.cpp Model/*.h \
									OpenGL/*.cpp OpenGL/*.h \
									Settings/*.cpp Settings/*.h \
									View/*.cpp View/*.h \
									*.cpp
	clang-format -n Controller/*.cpp Controller/*.h \
									Model/*.cpp Model/*.h \
									OpenGL/*.cpp OpenGL/*.h \
									Settings/*.cpp Settings/*.h \
									View/*.cpp View/*.h \
									*.cpp

check: leaks clang
