/*
 * 功能：驱动 两个蠕动泵 测试，按照师姐的要求
 * 作者：张津阳
 * 日期：2018年2月1日
 * 备注：目前的问题就是，去实现独立设置每一个蠕动泵的剩余演示时间
 * 改进：（1）与test4相比，增加了利用millis()函数将 进样时间 在串口打印出来
 *       （2）与test5相比，解决命令字符在 串口缓冲区 堆积的问题，这样在一个25s的工作周期 期间发送的多余的命令就不会被执行
 *       （3）增加了一个蠕动泵的控制，现在可以同时控制两个泵
 *       （4）与test7相比，启用命令字符串strCommand 代替 命令字符ch
 *       （5）那么，已经实现了通过Qt上位机设置蠕动泵的进样时间（也就是定时器的触发间隔时间）
 *       （6）那么，我现在已经可以单独设置每一个蠕动泵的进样时间了
 */
#include<FlexiTimer2.h>
const int T = 1000;   //脉冲信号的周期，单位：微秒
int half_T;           //脉冲信号周期的一半

int PulsePin_Pump1 = 2;
int DirPin_Pump1   = 3;
int switch_Pump1   = 4;
int PulsePin_Pump2 = 5;

const int LeftTime = 10;   //延时1秒的个数

boolean PUMPASTOP = false;
boolean PUMPBSTOP = false;

String strCommand = "";     //命令
//char ch;

unsigned long RunTime = 0;     //记录系统运行的时刻，用于计算进样的时间
int commaPosition;
int samplingTime_PumpA = 3000;
int samplingTime_PumpB = 5000;
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

void PumpA_stop()   //泵1停止
{
  PUMPASTOP = true;
  }
void PumpB_stop()   //泵1停止
{
  PUMPBSTOP = true;
  }

void readSerial()  //读串口数据
{
  while( Serial.available() > 0 )
  {
    char inchar = Serial.read();  //一个字符一个字符地读串口的数据
    strCommand += (char)inchar;
    delay(10);    //等待输入字符完全进入接收缓冲区
    }
   
  }


  
void setup() {
    pinMode(PulsePin_Pump1,OUTPUT);
    pinMode(PulsePin_Pump2,OUTPUT);
    pinMode(DirPin_Pump1,OUTPUT);
    pinMode(switch_Pump1,INPUT);
    half_T = T / 2;

    Serial.begin(9600);

    //FlexiTimer2::set( samplingTime, Pump_stop );       //5秒之后，泵1停止
    // FlexiTimer2::start();    //开启定时器

    Serial.println("Waiting for your command...");
}

void loop() {

  //ch = Serial.read();
  readSerial();  //读串口数据
  commaPosition = strCommand.indexOf(',');
  
  if( commaPosition == -1 )
  {
    if( strCommand == "a" )
    {
      Serial.println("You input a char 'a'");
      FlexiTimer2::set( samplingTime_PumpA, PumpA_stop );
      
      //step1
      FlexiTimer2::start();    //开启定时器
      RunTime = millis();      //记录当前的运行时刻
  
      //step2.驱动 蠕动泵
      while( PUMPASTOP == false )
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
      PUMPASTOP = false;
      //*************************************************************
  
      //step8
      while( Serial.read() >= 0 ){}  // arduino清空串口缓存
      
      strCommand = "";
      }
  
      
    if( strCommand == "b" )
    {
      Serial.println("You input a char 'b'");
      FlexiTimer2::set( samplingTime_PumpB, PumpB_stop );
      
      //step1.开启定时器
      FlexiTimer2::start();    //开启定时器
      RunTime = millis();      //记录当前的运行时刻
      
      //step2.驱动 蠕动泵
      while( PUMPBSTOP == false )
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
      PUMPBSTOP = false;
   
      //step8.// arduino清空串口缓存
      while( Serial.read() >= 0 ){} 
      
      strCommand = "";       
      }
  }
  else
  {
    Serial.println("strCommand include a ','");

    String strPart = strCommand.substring( 0, commaPosition );
    int numberPart = strCommand.substring( commaPosition+1, strCommand.length() ).toInt();

    Serial.println(strPart);
    Serial.println(numberPart);

    if( strPart == "PAST" )    //蠕动泵A要改变进样时间
    {
      Serial.print("PumpA samplingTime change to ");
      Serial.print(numberPart);
      Serial.println("ms !");
      
      samplingTime_PumpA = numberPart;
      FlexiTimer2::set( samplingTime_PumpA, PumpA_stop );
      
     }
     if( strPart == "PBST" )    //蠕动泵B要改变进样时间
     {
      Serial.print("PumpB samplingTime change to ");
      Serial.print(numberPart);
      Serial.println("ms !");
      
      samplingTime_PumpB = numberPart;
      FlexiTimer2::set( samplingTime_PumpB, PumpB_stop );      
      }      
    strCommand = "";
    }

}
