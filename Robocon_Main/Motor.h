class MotorI2C {       // The class
  private:             // Access specifier
    short unsigned driverAddress;
    short unsigned motorAddress;
    bool swap_dir = false;
  public:
    MotorI2C( short unsigned driverAddress, short unsigned motorAddress)
    {
      this->driverAddress = driverAddress;
      this->motorAddress = driverAddress;
    }
    MotorI2C( short unsigned driverAddress, short unsigned motorAddress , bool swap_dir)
    {
      this->driverAddress = driverAddress;
      this->motorAddress = driverAddress;
      this->swap_dir = swap_dir;
    }

    run( float pwm )
    {
      Wire.beginTransmission( this->driverAddress );
      Wire.write( this->motorAddress );
      
      if( swap_dir ) int dir = pwm > 0  ? 2 : 1;
      else int dir = pwm > 0  ? 1 : 2;

      if (pwm == 0) { dir = 3; }
      Wire.write(dir);
      Wire.write(int(abs(pwm)));
      Wire.endTransmission();
    }
    

};

class MotorPin{
    private:
      short unsigned in1Pin;
      short unsigned in2Pin;
      short unsigned pwmPin;
    public:
      MotorPin( short unsigned in1Pin , short unsigned in2Pin , short unsigned pwmPin )
      {
        this->in1Pin = in1Pin;
        this->in2Pin = in2Pin;
        this->pwmPin = pwmPin;
        pinMode( in1Pin , OUTPUT );
        pinMode( in2Pin , OUTPUT );
        pinMode( pwmPin , OUTPUT );
      }

      run( float pwm )
      {
        digitalWrite( in1Pin , HIGH );
        digitalWrite( in2Pin , LOW );
        analogWrite( pwmPin , pwm );
      }
      run( float pwm , long delay )
      {
        digitalWrite( in1Pin , HIGH );
        digitalWrite( in2Pin , LOW );
        analogWrite( pwmPin , pwm );

        delay( delay );
        stop();

      }
      stop()
      {

        digitalWrite( in1Pin , LOW );
        digitalWrite( in2Pin , LOW );
        analogWrite( pwmPin , 0 );

      }
};

