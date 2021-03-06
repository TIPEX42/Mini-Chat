RELEASE_NAME 			:=	mini_chat
DEBUG_NAME				:=	mini_chat_debug

# Source files
# ****************************************************************************

SRCS_FILE		:=	main.c	\
					init.c signal.c	\
					user_input.c	\
					server.c client.c packet.c

SRCS			:=	$(addprefix $(SRCS_DIR)/, $(SRCS_FILE))
RELEASE_OBJS	:=	$(addprefix $(RELEASE_OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))
DEBUG_OBJS		:=	$(addprefix $(DEBUG_OBJS_DIR)/, $(notdir $(SRCS:.c=d.o)))

DEPENDS			:=	$(RELEASE_OBJS:.o=.d) $(DEBUG_OBJS:.o=.d)

# Recipe
# ****************************************************************************

.PHONY: all
all: $(RELEASE_NAME)

.PHONY: debug
debug: $(DEBUG_NAME)

.PHONY: bonus
bonus: all

$(RELEASE_OBJS_DIR)/%.o:	$(SRCS_DIR)/%.c | $(RELEASE_OBJS_DIR)
	@$(BIN_CC) $(RELEASE_BIN_CFLAGS) -c $< -o $@
	@printf "$(_GREEN)█$(_END)"

$(RELEASE_NAME): $(RELEASE_OBJS) $(RELEASE_LEAKY_LIB)
	@printf "$(_BLUE)\nCompiled source files\n"
	@$(BIN_CC) $(RELEASE_BIN_CFLAGS) $(RELEASE_OBJS) $(RELEASE_LIBS) -o ../$@
	@printf "$(_GREEN)Finish compiling $(RELEASE_NAME)!$(_END)\n"

$(DEBUG_OBJS_DIR)/%d.o:	$(SRCS_DIR)/%.c | $(DEBUG_OBJS_DIR)
	@$(BIN_CC) $(DEBUG_BIN_CFLAGS) -c $< -o $@
	@printf "$(_GREEN)█$(_END)"

$(DEBUG_NAME): $(DEBUG_OBJS) $(DEBUG_LEAKY_LIB)
	@printf "$(_BLUE)\nCompiled source files\n"
	@$(BIN_CC) $(DEBUG_BIN_CFLAGS) $(DEBUG_OBJS) $(DEBUG_LIBS) -o ../$@
	@printf "$(_GREEN)Finish compiling $(DEBUG_NAME)!$(_END)\n"

clean:
	@printf "$(_YELLOW)Removing object files ...$(_END)\n"
	@rm -rf $(RELEASE_OBJS_DIR)
	@rm -rf $(DEBUG_OBJS_DIR)
	@rm -fr *.dSYM

fclean:	clean
	@printf "$(_RED)Removing Executable ...$(_END)\n"
	@rm -rf ../$(RELEASE_NAME)
	@rm -rf ../$(DEBUG_NAME)

re:		fclean all

$(RELEASE_OBJS_DIR):
	@mkdir -p $(RELEASE_OBJS_DIR)

$(DEBUG_OBJS_DIR):
	@mkdir -p $(DEBUG_OBJS_DIR)

-include $(DEPENDS)
