/*
 * 功能：驱动 两个蠕动泵 测试，按照师姐的要求
 * 作者：张津阳
 * 日期：2018年2月2日
 * 备注：目前的这个版本，蠕动泵控制系统的功能基本都已经实现
 * 改进：（1）与test4相比，增加了利用millis()函数将 进样时间 在串口打印出来
 *       （2）与test5相比，解决命令字符在 串口缓冲区 堆积的问题，这样在一个25s的工作周期 期间发送的多余的命令就不会被执行
 *       （3）增加了一个蠕动泵的控制，现在可以同时控制两个泵
 *       （4）与test7相比，启用命令字符串strCommand 代替 命令字符ch
 *       （5）那么，已经实现了通过Qt上位机设置蠕动泵的进样时间（也就是定时器的触发间隔时间）
 *       （6）那么，我现在已经可以单独设置每一个蠕动泵的进样时间了
 *       （7）那么，我现在已经可以单独设置每一个蠕动泵的剩余延时时间了
 *       （8）增加命令序列，如'ababababaabbaa...'
 */
#include<FlexiTimer2.h>
const long T = 1000;   //脉冲信号的周期，单位：微秒
long half_T_PumpA;           //脉冲信号周期的一半
long half_T_PumpB;           //脉冲信号周期的一半
int PulsePin_Pump1 = 2;
int DirPin_Pump1   = 3;
int switch_Pump1   = 4;
int PulsePin_Pump2 = 5;

int LeftTime_PumpA = 2;   //延时1秒的个数
int LeftTime_PumpB = 2;   //延时1秒的个数

boolean PUMPASTOP = false;
boolean PUMPBSTOP = false;

String strCommand = "";     //命令
//char ch;

unsigned long RunTime = 0;     //记录系统运行的时刻，用于计算进样的时间
int commaPosition;
int samplingTime_PumpA = 3000;
int samplingTime_PumpB = 3000;
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

void PumpA_Action()
{
    Serial.println("You input a char 'a'");
    Serial.println("PumpA started to work...");
    
    FlexiTimer2::set( samplingTime_PumpA, PumpA_stop );
    
    //step1
    FlexiTimer2::start();    //开启定时器
    RunTime = millis();      //记录当前的运行时刻

    //step2.驱动 蠕动泵
    while( PUMPASTOP == false )
    {
      pulseStart( PulsePin_Pump1, half_T_PumpA );
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
    for( int i=0; i < LeftTime_PumpA; i++ )
    {
      delay(1000);   //延时1秒
      }
      
    //step7.复一下位
    PUMPASTOP = false;
    //*************************************************************

    //step8
    while( Serial.read() >= 0 ){}  // arduino清空串口缓存
    
    //strCommand = "";

    Serial.println("A work cycle of PumpA is over!");
  }

void PumpB_Action()
{
    Serial.println("You input a char 'b'");
    Serial.println("PumpB started to work...");
    
    FlexiTimer2::set( samplingTime_PumpB, PumpB_stop );
    
    //step1.开启定时器
    FlexiTimer2::start();    //开启定时器
    RunTime = millis();      //记录当前的运行时刻
    
    //step2.驱动 蠕动泵
    while( PUMPBSTOP == false )
    {
      pulseStart( PulsePin_Pump2, half_T_PumpB );
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
    for( int i=0; i < LeftTime_PumpB; i++ )
    {
      delay(1000);   //延时1秒
      }
      
    //step7.复一下位
    PUMPBSTOP = false;
 
    //step8.// arduino清空串口缓存
    while( Serial.read() >= 0 ){} 
    
    //strCommand = ""; 
    
    Serial.println("A work cycle of PumpB is over!");
  }
  
void setup() {
    pinMode(PulsePin_Pump1,OUTPUT);
    pinMode(PulsePin_Pump2,OUTPUT);
    pinMode(DirPin_Pump1,OUTPUT);
    pinMode(switch_Pump1,INPUT);
    half_T_PumpA = T / 2;
    half_T_PumpB = half_T_PumpA;

    Serial.begin(9600);

    //FlexiTimer2::set( samplingTime, Pump_stop );       //5秒之后，泵1停止
    // FlexiTimer2::start();    //开启定时器

    Serial.println("Waiting for your command...");
}

void loop() {

  //ch = Serial.read();
  readSerial();  //读串口数据

  if( strCommand != "" )
  {
    commaPosition = strCommand.indexOf(',');
    
    if( commaPosition == -1 )
    {
      if( strCommand == "a" )
      {
        PumpA_Action();
        strCommand = "";
        }     
      else if( strCommand == "b" )
      {
        PumpB_Action();
        strCommand = "";
        }
      else
      {
        Serial.println( strCommand );
        for(int i=0; i < strCommand.length(); i++)
        {
          String inString = strCommand.substring( i, i+1 );
          Serial.println(inString);

          if( inString == "a" )
          { 
            Serial.println("PumpA");
            PumpA_Action(); 
            }
          if( inString == "b" )
          {
            Serial.println("PumpB"); 
            PumpB_Action(); 
            }
                      
          }
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
        if( strPart == "PALT" )
        {
          Serial.print("PumpA left time change to ");
          Serial.print(numberPart);
          Serial.println("s !");
    
          LeftTime_PumpA = numberPart;
          }
        if( strPart == "PBLT" )
        {
          Serial.print("PumpB left time change to ");
          Serial.print(numberPart);
          Serial.println("s !");
  
          LeftTime_PumpB = numberPart;
          }
        if( strPart == "PAspeed" )
        {
          Serial.print("PumpA frequency change to ");
          Serial.print(numberPart);
          Serial.println("Hz !");
          
          // 这里我把half_T从int型改为long型，而且将1000分成 10 和 100 相乘，测试结果还是比较准的
          half_T_PumpA = long( ( 1000 * 10 / double(numberPart) ) * 100 / 2 );   
          Serial.println(half_T_PumpA);       
          }
           
        if( strPart == "PBspeed" )
        {
          Serial.print("PumpB frequency change to ");
          Serial.print(numberPart);
          Serial.println("Hz !");
          
          // 这里我把half_T从int型改为long型，而且将1000分成 10 和 100 相乘，测试结果还是比较准的
          half_T_PumpB = long( ( 1000 * 10 / double(numberPart) ) * 100 / 2 );   
          Serial.println(half_T_PumpB);       
          }             
      strCommand = "";
      }
  }
}
