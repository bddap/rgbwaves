
//TODO: phase option

void setup(){
  Serial.begin(9600);
  displayState();
}

int pins[3] = {9,11,10};
float amplitudes[3] = {0.1,0.1,0.1};
float periods[3] = {10,10,10}; //in seconds
float levels[3] = {0.1,0.1,0.1};

const char colorNames[3][7] = {"red","green","blue"};

void updateLeds(){
  float now = ((float)millis())/1000*3.1415*2;
  for(int i = 0; i < 3; i++){
    int brightness = (sin(now/periods[i]) * amplitudes[i] + levels[i]) * 255;
    analogWrite(pins[i],brightness);
  }
}

void manageAmplitude(int color){
  if(amplitudes[color]>levels[color]) amplitudes[color] = levels[color];
  if(amplitudes[color]+levels[color]>1) amplitudes[color] = 1-levels[color];
}

void displayState(){
  Serial.println();
  for(int i = 0; i < 3; i++){
    Serial.print(colorNames[i]);
    Serial.println(":");
    Serial.print("  level:     ");
    Serial.println(levels[i]);
    Serial.print("  amplitude: ");
    Serial.println(amplitudes[i]);
    Serial.print("  period:    ");
    Serial.println(periods[i]);
  }
}


int colorSelect = 0;
void gotInput(){
  delay(1); //let the serial come in
  char rea = Serial.read();
  if (rea == 'r'){
    colorSelect = 0;
  }
  else if (rea == 'g'){
    colorSelect = 1;
  }
  else if(rea == 'b'){
    colorSelect = 2;
  }
  else if (rea == 'l'){
    levels[colorSelect] = Serial.parseFloat();
    if(levels[colorSelect]>1) levels[colorSelect] = 1;
    if(levels[colorSelect]<0) levels[colorSelect] = 0;
    manageAmplitude(colorSelect);
  }
  else if(rea == 'a'){
    amplitudes[colorSelect] = Serial.parseFloat();
    manageAmplitude(colorSelect);
  }
  else if (rea == 'p'){
    periods[colorSelect] = Serial.parseFloat();
  }
  else if (rea == '\n'){
    displayState();
  }
}

void loop(){
  if(Serial.available()){
    gotInput();
  }
  
  updateLeds();
}
