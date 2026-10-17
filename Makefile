SRCS = $(shell find . -name "*.cc" -o -name "*.h")

.PHONY = format

format:
	clang-format -i $(SRCS)