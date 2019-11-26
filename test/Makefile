PROGRAM = combinator averager new_dimension benchmark_dimension_unit  natural_number integer_number float
all : $(PROGRAM) 

CPP_FILES := $(foreach f, $(PROGRAM), $(f).cpp)

#COMPILE = clang -std=c++2a -o $@ $< 
COMPILE = g++ -std=c++2a -o $@ $< 

$(PROGRAM) : % : %.cpp
	$(COMPILE)

#depend: $(CPP_FILES)
#	g++ -MM $^ > .depend

#.cpp : .h
#	$(PROGRAM)


clean:
	rm -rf $(PROGRAM) 
