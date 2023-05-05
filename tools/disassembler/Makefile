# Compiler
CC=gcc
LD=gcc

# Flags
CFLAGS=-O2 -Wall -Wno-tautological-compare -Wno-int-in-bool-context
LDFLAGS=

# Tools
MKDIR=mkdir
RM=rm
RMDIR=rmdir

# Directories
OBJDIR=obj/
SRCDIR=src/

# Files
BIN=dump

HEADERS_RAW= \
	file.h \

SOURCES_RAW= \
	main.c \

# assembler.c \

SOURCES=$(addprefix $(SRCDIR)/, $(SOURCES_RAW))
HEADERS=$(addprefix $(SRCDIR)/, $(HEADERS_RAW))

OBJECTS=$(addprefix $(OBJDIR)/, $(SOURCES:$(SRCDIR)/%.c=%.o))

# Rules
.DEFAULT_GOAL := all
all: $(OBJDIR) $(BIN)

run: all
	./$(BIN)

clean:
	$(RM) $(BIN) || true
	$(RM) $(OBJECTS) || true
	$(RMDIR) $(OBJDIR) || true

print_vars:
	echo $(SOURCES)
	echo $(HEADERS)
	echo $(OBJECTS)

$(OBJDIR):
	$(MKDIR) -p $(OBJDIR)

$(BINDIR):
	$(MKDIR) -p $(BINDIR)

$(BIN): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(SRCDIR)/%.c: $(HEADERS)
