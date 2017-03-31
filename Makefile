CC=mpicc
FLAGS=-O3
EXECS=int_ring array_ring

all: ${EXECS}

int_ring: int_ring.c
	${CC} ${FLAGS} $^ -o int_ring

array_ring: array_ring.c
	${CC} ${FLAGS} $^ -o array_ring
