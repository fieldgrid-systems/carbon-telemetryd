CC := gcc
CFLAGS := -O0 -g -fno-stack-protector -no-pie
LDFLAGS := -Wl,-z,norelro
TARGET := carbon_telemetryd

all: $(TARGET)

$(TARGET): carbon_telemetryd.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
