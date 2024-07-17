NAME = a.out
CODEDIRS = . 
INCDIRS = .
SOURCETYPE = cpp

CC = g++
OPT = -O0
DEPFLAGS = -MP -MD
CFLAGS=-Wall -Wextra -Werror -std=c++98 $(foreach D,$(INCDIRS),-I$(D)) $(OPT) 

CFILES = $(foreach D,$(CODEDIRS),$(wildcard $(D)/*.$(SOURCETYPE)))
OBJECTS = $(patsubst %.$(SOURCETYPE),%.o,$(CFILES))
DEPFILES = $(patsubst %.$(SOURCETYPE),%.d,$(CFILES))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.$(SOURCETYPE)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<


clean:
	rm -rf $(OBJECTS) $(DEPFILES)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

-include $(DEPFILES)

.PHONY: all %.o $(NAME) clean fclean re diff