#define BAUD_RATE 9600

#define PIN_R 9
#define PIN_B 10
#define PIN_G 11


static const uint16_t pattern[] = {
    0x10ff,         // RED 255
    0x2000,         // GREEN 0
    0x3000,         // BLUE 00
    0x01AF,         // SLEEP 431
    0x1000,         // RED 0
    0x20ff,         // GREEN 255
    0x3000,         // BLUE 0
    0x01FF,         // SLEEP 511
    0x1000,         // RED 0
    0x2000,         // GREEN 0
    0x30FF,         // BLUE 255
    0x01FF,         // SLEEP 511
    0x4004          // JMP 4
};

static uint16_t instruction_counter = 0;


class PatternInterpreter {
private:
    enum class OpCode {
        Sleep     = 0x00,
        SetRed    = 0x01,
        SetGreen  = 0x02,
        SetBlue   = 0x03,
        Jump      = 0x04,
    };

private:
    byte m_pin_red;
    byte m_pin_green;
    byte m_pin_blue;

public:
    PatternInterpreter(byte pin_red, byte pin_green, byte pin_blue);
    void execute(uint16_t op);
};


PatternInterpreter::PatternInterpreter(byte pin_red, byte pin_green, byte pin_blue)
    : m_pin_red(pin_red), m_pin_green(pin_green), m_pin_blue(pin_blue) {}


void PatternInterpreter::execute(uint16_t op) {
    OpCode opcode = (OpCode) (op >> 12);
    uint16_t operand = (op & 0xfff);
    
    switch (opcode) {
        case OpCode::Sleep: {
            operand = (op & 0x0fff);
            delay(operand);
            break;
        }

        case OpCode::SetRed: {
            analogWrite(m_pin_red, operand & 0xff);
            break;
        }

        case OpCode::SetGreen: {
            analogWrite(m_pin_green, operand & 0xff);
            break;
        }

        case OpCode::SetBlue: {
            analogWrite(m_pin_blue, operand & 0xff);
            break;
        }

        case OpCode::Jump: {
            instruction_counter = operand;
            break;
        }


        default: return;
    }
}


void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_G, OUTPUT);
}


void loop() {
    static PatternInterpreter interpreter(PIN_R, PIN_G, PIN_B);

    if (instruction_counter < (sizeof(pattern) / sizeof(pattern[1]))) {
        uint16_t next = pattern[instruction_counter++];
        interpreter.execute(next);
    }
}
