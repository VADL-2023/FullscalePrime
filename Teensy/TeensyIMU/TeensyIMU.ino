void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  
  Serial.println("VectorNav Teensy Test");
  
  delay(100);
  
  Serial1.write("$VNWRG,06,0*XX\r\n");
  Serial1.write("$VNWRG,82,1,0,0,0,0*XX\r\n");
  Serial1.write("$VNRRG,82*XX\r\n");
}

char data = ' ';
uint8_t cmdNum = 0;
uint32_t count = 0;

void loop() {
  
  //write the command
  

  //read the command
  data = Serial1.read();
  if (data == '$'){
    Serial.print("\n");
    Serial.print(Serial1.available());
  }
  if (' ' <= data && data <= '~'){
    Serial.print(data);

    if(data == '*'){
      if(cmdNum == 0){
        Serial1.write("$VNRRG,54*XX\r\n");
        cmdNum = 1;
      }
      else{
        Serial1.write("$VNRRG,80*XX\r\n");
        cmdNum = 0;
      }
    }
  }
  //Serial.println();
}
