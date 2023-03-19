recursive_hello: recursive_hello.c
	cc -g -Wall -Wextra recursive_hello.c -o recursive_hello -pthread

.PHONY: lint
lint:
	cpplint recursive_hello.c

.PHONY: memcheck
memcheck: recursive_hello
	valgrind --tool=memcheck --leak-check=full ./recursive_hello

.PHONY: helgrind
helgrind: recursive_hello
	valgrind --tool=helgrind ./recursive_hello

.PHONY: asan # invalid access & memory leaks
asan: recursive_hello.c
	cc -g -Wall -Wextra -fsanitize=address recursive_hello.c -o recursive_hello -pthread

.PHONY: msan # uninitialized memory
msan: recursive_hello.c
	clang -g -Wall -Wextra -fsanitize=memory recursive_hello.c -o recursive_hello -pthread

.PHONY: tsan # thread sanitizer
tsan: recursive_hello.c
	cc -g -Wall -Wextra -fsanitize=thread recursive_hello.c -o recursive_hello -pthread

.PHONY: ubsan # undefined behavior
ubsan: recursive_hello.c
	cc -g -Wall -Wextra -fsanitize=undefined recursive_hello.c -o recursive_hello -pthread

.PHONY: clean
clean:
	rm -f recursive_hello