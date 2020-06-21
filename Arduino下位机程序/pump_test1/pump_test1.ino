/*
 * 功能：驱动单个蠕动泵测试
 * 作者：张津阳
 * 日期：2018年1月30日
 */
const int T = 1000;   //脉冲信号的周期，单位：微秒
int half_T;           //脉冲信号周期的一半

int PulsePin_Pump1 = 2;
int DirPin_Pump1   = 3;
int switch_Pump1   = 4;


/*
 * 自定义函数
 */
void pulseStart( int pulsePin , int t )   // t单位是微秒
{
  digitalWrite(pulsePin,HIGH);
  delayMicroseconds(t);
  digitalWrite(pulsePin,LOW);
  delayMicroseconds(t);          // 频率 f = ( 1 / (2*t) ) * 1e6
  }

void pulseStop( int pulsePin )
{
  digitalWrite(pulsePin,LOW);    //停止发脉冲
  }


  
void setup() {
    pinMode(PulsePin_Pump1,OUTPUT);
    pinMode(DirPin_Pump1,OUTPUT);
    pinMode(switch_Pump1,INPUT);
    half_T = T / 2;
    
}

void loop() {

  if( !digitalRead(switch_Pump1) )
  {
    pulseStart( PulsePin_Pump1, half_T );
    }
  else{
    pulseStop(PulsePin_Pump1);
    }
}
