special-case := str_vector_utils.c readline.c json_parse.c
c := $(filter-out $(special-case), $(wildcard *.c))

%.run: %.c; $(cmd)

test: $(c:%.c=%.run) readline.run

readline.run: readline.c
	printf '' | $(cmd)
	ip route | $(cmd)

json_parse.run: json_parse.c
	cat $< | awk 'm==1 { print substr($$0, 4) } /^\/\/ test/ {m=1}' | $(cmd)

cmd = cling $< -lcrypto -ljansson
