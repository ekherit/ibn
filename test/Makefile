PROGRAM = unit combinator averager  benchmark_dimension_unit  natural_number integer_number opt2 benchmark_double valer pointer
all : $(PROGRAM) 

CPP_FILES := $(foreach f, $(PROGRAM), $(f).cpp)

#COMPILE = clang -std=c++2a -o $@ $< 
COMPILE = g++ -std=c++2a -Wall -o $@ $<  

$(PROGRAM) : % : %.cpp
	$(COMPILE)

#depend: $(CPP_FILES)
#	g++ -MM $^ > .depend

#.cpp : .h
#	$(PROGRAM)


clean:
	rm -rf $(PROGRAM) 
