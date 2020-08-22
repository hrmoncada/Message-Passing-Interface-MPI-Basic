# $ gcc -c example_1_METIS.c -o driver.o
# $ gcc -o out driver.o -L. /home/hrmoncada/Desktop/MPAS_Ocean/Download/metis-5.1.0/build/Linux-x86_64/libmetis/libmetis.a -lm
# Automatic variables :There are a bunch of automatic variables that you can use to save yourself a lot of typing.
# Here are the ones that I use most: https://kbroman.org/minimal_make/
#     $@    the file name of the target
#     $<    the name of the first prerequisite (i.e., dependency)
#     $^    the names of all prerequisites (i.e., dependencies)
#     $(@D)    the directory part of the target
#     $(@F)    the file part of the target
#     $(<D)    the directory part of the first prerequisite (i.e., dependency)
#     $(<F)    the file part of the first prerequisite (i.e., dependency

# Choose compiler
CC = mpicc
CXX = mpic++
FC = mpifort
FC90 = mpif90

# Choose  flags
OPTFLAGS = -O3
CFLAGS = $(OPTFLAGS) -Wall -g
CXXFLAGS = $(OPTFLAGS) -Wall -g
FFLAGS = $(OPTFLAGS) -Wall -g
OPENMP = -fopenmp 

# CPU LIBS & INCLUDE - User configurable options
INCLUDES = -I.
MATH_LIBS=-lm
LIBS = -L.

# Execute
MPIRUN = mpirun

# Target binaries
OUTPUT = HelloWorld\
	 Barier\
	 Master
	
HelloWorld: 0_MPI_Hello_World.c
	${CC} $(CFLAGS) -o $@ $< $(LIBS) $(INCLUDES) $(MATH_LIBS)

Barrier: 0_MPI_Hello_World_Barrier.c
	${CC} $(CFLAGS) -o $@ $< $(LIBS) $(INCLUDES) $(MATH_LIBS)

Master: 0_MPI_Hello_World_MASTER.c
	${CC} $(CFLAGS) -o $@ $< $(LIBS) $(INCLUDES) $(MATH_LIBS)

run:
	$(MPIRUN) -np 4 ./$(OUTPUT)

clean:
	rm -f $(OUTPUT) *~ *.o
