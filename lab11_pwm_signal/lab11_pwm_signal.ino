

int CLK = 9; // clk signal
int DIR = 8; // DIR = 0 => CW, DIR = 1 =>CCW
int timer1_counter,timer1, Ns_transfer = 1, i = 0;
int pwm_begin;
float pulse_second,milli_second;
volatile float time1[300] = {};
volatile float task1 = 0;
volatile int Index_time = 0;
volatile int j = 0;
volatile int Ns = -1;

void setup() {
  Serial.begin(9600);
  Serial.flush();
  pinMode(CLK, OUTPUT);
  pinMode(DIR, OUTPUT);
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  // preload timer 65536-16MHz/256/2Hz
  timer1_counter = 63935; // Adjust the value accordingly 65535-10000 = 55535
  TCNT1 = timer1_counter; // preload timer
  TCCR1B = TCCR1B & B11111000 | B00000001; //Prescaler*(65535-timer_counter)/16Mhz
  //TCCR1B |= (1 << CS12); // 256 pre-scaler
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  interrupts(); // enable all interrupts
}

void loop() {
  if (Serial.available() > 0 ) {
    String dato = Serial.readStringUntil('\n');
    if (dato == "Ns") {
//      noInterrupts(); 
      pwm_begin = 0;
      String dato = Serial.readStringUntil('\n');
      Serial.println("ok");
      Ns = dato.toInt();
//      Serial.print("Ns_transfer =");
//      Serial.println(Ns_transfer);
      Serial.print("Ns =");
      Serial.println(Ns);
//      interrupts();
    }else if(dato == "End"){
      noInterrupts(); 
      Serial.println("Serial End");
      Index_time = i;
      i = 0;
      j = 2;
      digitalWrite(DIR, HIGH);
      pwm_begin = 1;
      for (int f=0;f<Ns;f++){
 //     Serial.println("time1[f]= ");
 //     Serial.println(time1[f]);
       pulse_second = (float)time1[f]/(float)0.08789;
 //     Serial.println("pulse_second= ");
 //     Serial.println(pulse_second);
       milli_second = 1/(float)pulse_second;       
 //     Serial.println("mili_second= ");
 //     Serial.println(milli_second);
        time1[f] = (float)milli_second/(float)0.1;
 //     Serial.println("adjusted_time1[f]= ");
  //    Serial.println(time1[f]);       
      }
      interrupts();   
    }else{
      time1[i] = dato.toFloat();
      Serial.println("ok");
//      Serial.print("The data sent was: ");
//      Serial.println(time1[i]);
      i++;
//        Serial.print("i=");
//        Serial.println(i);
    } 
  }
}

ISR(TIMER1_OVF_vect) // interrupt service routine
{
  if (pwm_begin == 1) {
    TCNT1 = timer1_counter;
    task1 = task1 + 0.1;
    if (task1 >= time1[j]) {
      Serial.print("task1 = ");
      Serial.println(task1);
      Serial.print("time1[j] = ");
      Serial.println(time1[j]);
      Serial.print("j = ");
      Serial.println(j);
      Serial.print("Ns =");
      Serial.println(Ns);
      digitalWrite(CLK, digitalRead(CLK) ^ 1);
      j++;
      if (j == Ns){
        j = 2;
        pwm_begin = 0;
      }
      task1 = 0;
    } 
  }
}
