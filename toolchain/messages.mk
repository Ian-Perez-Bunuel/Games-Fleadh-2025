# Message Color Codes
BLUE := \033[0;34m
GREEN := \033[0;32m
YELLOW := \033[0;33m
RED := \033[0;31m
RESET := \033[0m

# Color output function (Linux / MSYS on Windows)
define PRINT_MSG_UNIX
	/bin/echo -e "$(1)$(2)$(RESET)"
endef
define PRINT_MSG_WIN
	printf "%b%s%b" "$(1)" "$(2)" "$(RESET)\n"
endef

# Platform-specific message printing logic 
ifeq ($(IS_WINDOWS),TRUE)
  PRINT_MSG = $(PRINT_MSG_WIN)
else
  PRINT_MSG = $(PRINT_MSG_UNIX)
endif

# Message macros
define INFO_MSG
	$(call PRINT_MSG,$(BLUE),INFO: $(1))
endef
define SUCCESS_MSG
	$(call PRINT_MSG,$(GREEN),SUCCESS: $(1))
endef
define WARNING_MSG
	$(call PRINT_MSG,$(YELLOW),WARNING: $(1))
endef
define ERROR_MSG
	$(call PRINT_MSG,$(RED),ERROR: $(1))
endef