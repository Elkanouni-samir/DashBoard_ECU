from machine import Pin, SPI

# MCP2515 instruction set
INSTRUCTION_RESET = 0xC0
INSTRUCTION_WRITE = 0x02
INSTRUCTION_READ = 0x03

# MCP2515 register addresses
REGISTER_CANCTRL = 0x0F
REGISTER_CANSTAT = 0x0E

spi = SPI(0, baudrate=10_000_000, polarity=0, phase=0, bits=8, sck=Pin(2), mosi=Pin(3), miso=Pin(4))
cs = Pin(5, Pin.OUT) # Chip select for the MCP2515 module
p2 = Pin(2, Pin.IN) # Interrupt coming from MCP2515 module

def initialize():
    # Reset the MCP2515 module
    cs.value(1)
    spi.write(bytes([INSTRUCTION_RESET]))
    cs.value(0)

    # Wait for the module to complete the reset
    while spi.read(1) != bytes([0x00]):
        pass

    # Configure the MCP2515 module
    write_register(REGISTER_CANCTRL, 0x80) # Set configuration mode
    write_register(0x2A, 0x07) # Enable interrupt pin
    write_register(0x2B, 0x0F) # Enable interrupt on RX0 and RX1

    # Check the module status
    status = read_register(REGISTER_CANSTAT)
    if status[0] != 0x80:
        raise Exception("MCP2515 module is not in configuration mode")

def read_register(address):
    cs.value(1)
    spi.write(bytes([INSTRUCTION_READ, address]))
    value = spi.read(1)
    cs.value(0)
    return value

def write_register(address, value):
    cs.value(1)
    spi.write(bytes([INSTRUCTION_WRITE, address, value]))
    cs.value(0)

def irq_handler(pin):
    print("Interrupt from MCP2515 module")
    # Read the interrupt flags
    flags = read_register(0x2C)
    print("Interrupt flags: ", flags)

    # Read the data from RXB0 buffer
    write_register(0x60, 0x00) # Set RXB0CTRL to read mode
    cs.value(1)
    spi.write(bytes([0x90, 0x00])) # Read data from RXB0 buffer
    data = spi.read(5) # Read 5 bytes of data
    cs.value(0)
    print("Data received from MCP2515 module: ", data)

# Initialize and configure the MCP2515 module
initialize()

# Set up the interrupt pin
p2.irq(trigger=Pin.IRQ_RISING, handler=irq_handler)
