special-case := str_vector_utils.c readline.c
c := $(filter-out $(special-case), $(wildcard *.c))

%.run: %.c; $(cmd)

test: $(c:%.c=%.run) readline.run

readline.run: readline.c
	printf '' | $(cmd)
	ip route | $(cmd)

cmd = cling $< -lcrypto
