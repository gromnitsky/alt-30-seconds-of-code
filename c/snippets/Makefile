special-case := readline.c json_parse.c
c := $(filter-out $(special-case), $(wildcard *.c))

%.run: %.c; $(cmd)

run := $(c:%.c=%.run) readline.run json_parse.run
test: $(run); $(MAKE) --no-print-directory kill
$(run): server

readline.run: readline.c
	printf '' | $(cmd)
	ip route | $(cmd)

json_parse.run: json_parse.c
	cat $< | awk 'm==1 { print substr($$0, 4) } /^\/\/ test/ {m=1}' | $(cmd)

cmd = cling -Wall $< -lcurl -lcrypto -ljansson



server.cmd = node http_test_uploads.js

server: kill
	$(server.cmd) &
kill:
	-pkill -f '$(server.cmd)'
