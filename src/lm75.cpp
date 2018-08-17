/*
 Copyright © 2014 José Luis Zabalza  License LGPLv3+: GNU
 LGPL version 3 or later <http://www.gnu.org/copyleft/lgpl.html>.
 This is free software: you are free to change and redistribute it.
 There is NO WARRANTY, to the extent permitted by law.
*/

#include "lm75.h"


I2C i2c(I2C_SDA, I2C_SCL);

//-------------------------------------------------------------------------------
TempI2C_LM75::TempI2C_LM75( uint8_t i2c_addr, Resolution resolution)
{
    m_u8I2CAddr = (i2c_addr << 1);
}

//-------------------------------------------------------------------------------
float TempI2C_LM75::getTemp()
{
    return(int(getReg(temp_reg)) / 256.0);
}

//-------------------------------------------------------------------------------
unsigned TempI2C_LM75::getReg(LM75Register reg)
{
	  char c = reg, buff[2];
	
	  i2c.write(m_u8I2CAddr, &c, 1);
	  if(reg != config_reg)
		{
			i2c.read(m_u8I2CAddr | 0x01, buff, 2);
			return (buff[0] << 8) | buff[1];
		}
		else
		{
			i2c.read(m_u8I2CAddr, buff, 1);
			return buff[0];
		}
}
//-------------------------------------------------------------------------------
void TempI2C_LM75::setReg(LM75Register reg, unsigned newValue)
{
	  char low, high;
	  char c = reg;

	  i2c.write(m_u8I2CAddr, &c, 1, true);
	
    if(reg != config_reg)
    {
			high = (newValue>>8) & 0xFF;
			i2c.write(m_u8I2CAddr, &high, 1, true);
    }
		low = newValue & 0xFF;
		i2c.write(m_u8I2CAddr, &low, 1);
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTHyst(float newTHyst)
{
    setReg(THyst_reg,int(newTHyst * 256));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTOS(float newTOS)
{
    setReg(TOS_reg,int(newTOS * 256) );
}

//-------------------------------------------------------------------------------
float TempI2C_LM75::getTHyst(void)
{
    return(int(getReg(THyst_reg)) / 256.0F);
}

//-------------------------------------------------------------------------------
float TempI2C_LM75::getTOS(void)
{
    return(int(getReg(TOS_reg)) / 256.0F);
}

//-------------------------------------------------------------------------------
TempI2C_LM75::TermostatMode TempI2C_LM75::getTermostatMode()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(TermostatMode(regv.mbits.termostat_mode));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTermostatMode(TempI2C_LM75::TermostatMode newMode)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.termostat_mode = newMode;

    setReg(config_reg,unsigned(regv.mbyte));
}

//-------------------------------------------------------------------------------
TempI2C_LM75::TermostatFaultTolerance TempI2C_LM75::getTermostatFaultTolerance()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(TermostatFaultTolerance(regv.mbits.termostat_fault_tolerance));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setTermostatFaultTolerance(TermostatFaultTolerance newFaultTolerance)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.termostat_fault_tolerance = newFaultTolerance;

    setReg(config_reg,unsigned(regv.mbyte));
}

//-------------------------------------------------------------------------------
TempI2C_LM75::Resolution TempI2C_LM75::getResolution()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(Resolution(regv.mbits.resolution));
}
//-------------------------------------------------------------------------------
void TempI2C_LM75::setResolution(Resolution newResolution)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.resolution = newResolution;

    setReg(config_reg,unsigned(regv.mbyte));
}

//-------------------------------------------------------------------------------
TempI2C_LM75::OSPolarity TempI2C_LM75::getOSPolarity()
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);

    return(OSPolarity(regv.mbits.termostat_output_polarity));
}

//-------------------------------------------------------------------------------
void TempI2C_LM75::setOSPolarity(OSPolarity newOSPolarity)
{
    CfgRegister regv;

    regv.mbyte = getReg(config_reg);
    regv.mbits.termostat_output_polarity = newOSPolarity;

    setReg(config_reg,unsigned(regv.mbyte));
}

