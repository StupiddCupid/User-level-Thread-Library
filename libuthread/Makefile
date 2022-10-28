targets := libuthread.a
objs    := queue.o uthread.o context.o #sem.o preempt.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -MMD
CFLAGS  += -g

ifneq ($(V),1)
Q = @
endif

# Dep tracking *must* be below the 'all' rule
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

all: $(targets)

%.a: $(objs)
	ar rcs $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<


clean:
	@echo "clean"
	$(Q)rm -f $(targets) $(objs) $(deps)

