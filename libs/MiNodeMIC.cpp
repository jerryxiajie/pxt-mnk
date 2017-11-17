#include "MiNodeMIC.h"

MiNodeMIC::MiNodeMIC() :
pin(NULL)
{
  this->baseId = MINODE_ID_MODULE_LIGHT;
  system_timer_add_component(this);
}

MiNodeMIC::~MiNodeMIC()
{
  if(pin) {
    delete pin;
  }
  system_timer_remove_component(this);
}

void MiNodeMIC::update(void)
{
  static int adHolder = 530;
  int newValue = pin->read_u16();

  if(newValue - adHolder > 20)
  {
    MicroBitEvent evt(this->baseId + this->id,MINODE_MIC_EVT_NOISE);
  }
  adHolder = newValue;

}

void MiNodeMIC::attach(AnalogConnName connName)
{
  if(this->cn != MN_NC) {
    return;
  }

  MiNodeComponent::initAConnector(connName);

  PinName pinName = MiNodeConn::calcP0Name(this->cna);
  if(pin) {
    delete pin;
  }
  pin = new AnalogIn(pinName);

  NRF_ADC->CONFIG = (ADC_CONFIG_RES_8bit << ADC_CONFIG_RES_Pos) |
                    (ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos) |
                    (ADC_CONFIG_REFSEL_SupplyOneThirdPrescaling << ADC_CONFIG_REFSEL_Pos) |
                    (pinName << ADC_CONFIG_PSEL_Pos) |
                    (ADC_CONFIG_EXTREFSEL_None << ADC_CONFIG_EXTREFSEL_Pos);

  // ToDo: Init a timer      
  timer.attach_us(this, &MiNodeMIC::update, 50);
}


int MiNodeMIC::getMicLevel()
{
  unsigned int temp=0;
  temp = pin->read_u16();
  if (temp > 530)
  {
    temp -= 530;
  }
  else
  {
    temp = 530-temp;
  }

  if ((temp > MICROBIT_MIC_MIN) && (temp < MICROBIT_MIC_LEVEL_A+1))
  {
    return MICROBIT_MIC_LEVEL_1;
  }
  else if ((temp > MICROBIT_MIC_LEVEL_A) && (temp < MICROBIT_MIC_LEVEL_B+1))
  {
    return MICROBIT_MIC_LEVEL_2;
  }
  else if ((temp > MICROBIT_MIC_LEVEL_B) && (temp < MICROBIT_MIC_LEVEL_C+1))
  {
    return MICROBIT_MIC_LEVEL_3;
  }
  else if ((temp > MICROBIT_MIC_LEVEL_C) && (temp < MICROBIT_MIC_LEVEL_D+1))
  {
    return MICROBIT_MIC_LEVEL_4;
  }
  else if ((temp > MICROBIT_MIC_LEVEL_D) && (temp < MICROBIT_MIC_MAX+1))
  {
    return MICROBIT_MIC_LEVEL_5;
  }
  else
  {
    return MICROBIT_MIC_LEVEL_ERROR;
  }
}


