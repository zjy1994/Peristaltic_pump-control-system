/*
 * 功能：驱动 两个蠕动泵 测试，按照师姐的要求
 * 作者：张津阳
 * 日期：2018年1月31日
 * 备注：利用串口字符进行控制，我在串口发送一个'a'那么 蠕动泵A 就开始进行一个周期的 进样 和 延时
 *                               在串口发送一个'b'那么 蠕动泵B 就开始进行一个周期的 进样 和 延时
 * 改进：（1）与test4相比，增加了利用millis()函数将 进样时间 在串口打印出来
 *       （2）与test5相比，解决命令字符在 串口缓冲区 堆积的问题，这样在一个25s的工作周期 期间发送的多余的命令就不会被执行
 *       （3）增加了一个蠕动泵的控制，现在可以同时控制两个泵
 */
#include<FlexiTimer2.h>
const int T = 1000;   //脉冲信号的周期，单位：微秒
int half_T;           //脉冲信号周期的一半

int PulsePin_Pump1 = 2;
int DirPin_Pump1   = 3;
int switch_Pump1   = 4;
int PulsePin_Pump2 = 5;

const int LeftTime = 10;   //延时1秒的个数

boolean PUMPSTOP = false;

//String strCommand;     //命令
char ch;

unsigned long RunTime = 0;     //记录系统运行的时刻，用于计算进样的时间
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

void Pump_stop()   //泵1停止
{
  PUMPSTOP = true;
  }

  
void setup() {
    pinMode(PulsePin_Pump1,OUTPUT);
    pinMode(PulsePin_Pump2,OUTPUT);
    pinMode(DirPin_Pump1,OUTPUT);
    pinMode(switch_Pump1,INPUT);
    half_T = T / 2;

    Serial.begin(9600);

    FlexiTimer2::set(5000, Pump_stop);       //5秒之后，泵1停止
    // FlexiTimer2::start();    //开启定时器
    
}

void loop() {

  ch = Serial.read();
  
  if( ch == 'a' )
  {
    //step1
    FlexiTimer2::start();    //开启定时器
    RunTime = millis();      //记录当前的运行时刻

    //step2.驱动 蠕动泵
    while( PUMPSTOP == false )
    {
      pulseStart( PulsePin_Pump1, half_T );
      }

    // ********************  todo *******************************
    //step3.停发脉冲，停止蠕动泵
    pulseStop(PulsePin_Pump1);
    
    //step4.关闭 定时器
    FlexiTimer2::stop();

    //step5
    RunTime = millis() - RunTime;     //计算进样的时间
    Serial.print("Pump_A Sampling time:");
    Serial.print(RunTime);
    Serial.println("ms");
    
    //step6.延时 一个检测周期（25s）剩余的时间
    for( int i=0; i < LeftTime; i++ )
    {
      delay(1000);   //延时1秒
      }
      
    //step7.复一下位
    PUMPSTOP = false;
    //*************************************************************

    //step8
    while( Serial.read() >= 0 ){}  // arduino清空串口缓存
    
    }

    
  if( ch == 'b' )
  {
    //step1.开启定时器
    FlexiTimer2::start();    //开启定时器
    RunTime = millis();      //记录当前的运行时刻
    
    //step2.驱动 蠕动泵
    while( PUMPSTOP == false )
    {
      pulseStart( PulsePin_Pump2, half_T );
      }

    //step3.停下泵
    pulseStop(PulsePin_Pump2);
    
    //step4.关闭 定时器
    FlexiTimer2::stop();

    //step5.计算进样用时
    RunTime = millis() - RunTime;     //计算进样的时间
    Serial.print("Pump_B Sampling time:");
    Serial.print(RunTime);
    Serial.println("ms");

    //step6.延时 一个检测周期（25s）剩余的时间
    for( int i=0; i < LeftTime; i++ )
    {
      delay(1000);   //延时1秒
      }
      
    //step7.复一下位
    PUMPSTOP = false;
 
    //step8.// arduino清空串口缓存
    while( Serial.read() >= 0 ){} 
           
    }

}
