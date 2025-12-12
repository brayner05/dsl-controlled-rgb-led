#define BAUD_RATE 9600

#define PIN_R 9
#define PIN_B 10
#define PIN_G 11

enum class OpCode {
  Sleep     = 0x00,
  SetRed    = 0x01,
  SetGreen  = 0x02,
  SetBlue   = 0x03,
  Jump      = 0x04,
};


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


static void execute(uint16_t op) {
  OpCode opcode = (OpCode) (op >> 12);
  uint16_t operand = (op & 0xfff);
  
  switch (opcode) {
    case OpCode::Sleep: {
      operand = (op & 0x0fff);
      delay(operand);
      break;
    }

    case OpCode::SetRed: {
      analogWrite(PIN_R, operand & 0xff);
      break;
    }

    case OpCode::SetGreen: {
      analogWrite(PIN_G, operand & 0xff);
      break;
    }

    case OpCode::SetBlue: {
      analogWrite(PIN_B, operand & 0xff);
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
  if (instruction_counter < (sizeof(pattern) / sizeof(pattern[1]))) {
    uint16_t next = pattern[instruction_counter++];
    execute(next);
  }
}
