/*
MPU6050 and other Sensors Class

(c)2013 Marlon Gräfing
*/

#include <wiringSerial.h>

class I2C{
private:
	int fd;	// File descrition
	int address;
	
public:
	I2C(int addr){													// Initalize I2C Connection
		address=addr;
		const char *fileName = "/dev/i2c-0";						// Name of the port we will be using
		
		if ((fd = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
			printf("Failed to open I2C port. Try modprobe I2C-dev, or run as root.\n");
		}
		
		if (ioctl(fd, I2C_SLAVE, addr) < 0) {						// Set the port options and set the address of the device we wish to speak to
			printf("Unable to get bus access to talk to slave. Check connection and Address. %x \n",address);
		}
	}
	
	I2C(){
	}

	void write_reg(unsigned char reg, unsigned char data){			// Write Into Register
		unsigned char tmp[2];
		tmp[0] = reg;												
		tmp[1] = data;
		
		if ((write(fd, tmp, 2)) != 2) {								// Write commands to the I2C port
			printf("Error writing to I2C slaves Register. %x \n",address);
		}
	}
	
	void send(unsigned char data){			// Write 
		while(1){
			int i=1;
			if ((write(fd, &data, 1)) != 1) {								// Write commands to the I2C port
				printf("Error Nr: %d writing to I2C Slave (Motor)\n" , i);
				i++;
			}
			else{
				return;
			}
		}
	}

	void read_bytes(unsigned char first_reg , unsigned char data[] , int n_bytest){// reads n-Bytes from register	
		data[0]=first_reg;
		
		if ((write(fd, data, 1)) != 1) {							// Send register to read from
			printf("Error writing to I2C slave %x \n",address);
		}
	
		if (read(fd, data, n_bytest) != n_bytest) {					// Read back data into data[]
			printf("Unable to read from slave %x \n",address);
		}
	}
};

//------------------------------Class for Sensor readings------------------------------

class Sensors{
private:
	I2C I2C_imu; // MPU6050
	I2C I2C_mag; // Magnetometer
	int gpsfd ;  // Filedesciptor for GPS

	//offsets in raw Data, 
	float offset_accx ;
	float offset_accy ;
	float offset_accz ;
	float offset_gyrox ;
	float offset_gyroy ;
	float offset_gyroz ;
	
	//raw Data
	int rawAcc[3];
	int rawGyro[3];
	int rawMag[3];
	
	//gps Buffer for NMEA-Code
	char gpsBuffer[1024];
	char gpsBufferIndex;
	
	void serialGetLine(int fd,char buffer[]){
		for(int i=0;; i++){
			buffer[i]=serialGetchar(fd);
			if(buffer[i]==*"\n"){
				return;
			}
		}
	}

	float getLat( char buffer []){
		char degree[2];
		degree[0]=buffer[0];
		degree[1]=buffer[1];
		double lat=atof(degree)+atof(&buffer[2])/60.0;
		return lat;
	}

	float getLon( char buffer []){
		char degree[3];
		degree[0]=buffer[0];
		degree[1]=buffer[1];
		degree[2]=buffer[2];
		float lon=atof(degree)+atof(&buffer[3])/60.0;
		return lon;
	}

public:
	//GPS Data:
	float position[3];
	float speed,heading;
	float accuracy[3];
	//Gyro Data
	double gyro[3];
	//Acc Data
	double acc[3];
	//Mag Data
	double mag[3];
	
	void updateGps(){
		serialDataAvail(gpsfd);
		for(int n=serialDataAvail(gpsfd);n>0;n--){
			gpsBuffer[gpsBufferIndex]=serialGetchar(gpsfd);
			if(gpsBuffer[gpsBufferIndex]=='\n'){
				interpretGps();
				gpsBufferIndex=0;
				for(int i=0;i<1024;i++){
					gpsBuffer[i]=0;
				}
			}
			else{
				gpsBufferIndex++;
			}
		}
	}
	
	//NMEA-Parsing
	void interpretGps(){
		if(strncmp(gpsBuffer,"$GPRMC",6)==0){
			int komma=0;
			for(int i=0;i<1024;i++){
				if(gpsBuffer[i]==',' ){
					komma++;
					if(gpsBuffer[i+1]!=','){
						switch(komma){
							//Longitude
							case 3: position[0]=getLat(&gpsBuffer[i+1])  ;break;
							case 4:
								if(gpsBuffer[i+1]=='S'){
									position[0]=-position[0];
								} 
								break;
							//Latitude
							case 5: position[1]=getLon(&gpsBuffer[i+1])  ;break;
							case 6:
								if(gpsBuffer[i+1]=='W'){
									position[1]=-position[1];
								} 
								break;
							//Speed
							case 7: speed=atof(&gpsBuffer[i+1]); break;
							//heading
							case 8: heading=atof(&gpsBuffer[i+1]); break;
							
						}
					}
				}
			}
		}
		if(strncmp(gpsBuffer,"$GPGBS",6)==0){
			//get acuracy
			//printf(gpsBuffer);
		}
		
		if(strncmp(gpsBuffer,"$GPGGA",6)==0){
			int komma=0;
			for(int i=0;i<1024;i++){
				if(gpsBuffer[i]==',' ){
					komma++;
					if(gpsBuffer[i+1]!=','){
						switch(komma){
							//Horiziontal dillutation of precission
							case 8: accuracy[0]=atof(&gpsBuffer[i+1])  ;break;
							//Altitude
							case 9: position[2]=atof(&gpsBuffer[i+1])  ;break;
							
							
						}
					}
				}
			}			
		}
	}
	
	void getPosition(float data[3]){
		data[0]=position[0];
		data[1]=position[1];
		data[2]=position[2];
	}


	void updateAcc(){											// reads Acc Data from sensor
		unsigned char buf[6];
		I2C_imu.read_bytes(0x3B, buf,6);
		
		rawAcc[0]= ((signed short)(buf[0]*256+buf[1]));
		rawAcc[1]= ((signed short)(buf[2]*256+buf[3]));
		rawAcc[2]= ((signed short)(buf[4]*256+buf[5]));
		
		// Register einträge in Daten (m/s²)umwandeln
		acc[0]= (rawAcc[0] - offset_accx)/2048.0f*9.81f;
		acc[1]= (rawAcc[1] - offset_accy)/2048.0f*9.81f;
		acc[2]= (rawAcc[2] - offset_accz)/2048.0f*9.81f; 	
	}


	void get_rawacc(signed int data[3]){									// Gets raw Acc Data 
		data[0]= rawAcc[0];
		data[1]= rawAcc[1];
		data[2]= rawAcc[2]; 	
	}


	void updateGyro(){											// Gets Gyro Data in rad/s
		unsigned char buf[6];
		I2C_imu.read_bytes(0x43, buf,6);
		
		rawGyro[0]= ((signed short)(buf[0]*256+buf[1]));
		rawGyro[1]= ((signed short)(buf[2]*256+buf[3]));
		rawGyro[2]= ((signed short)(buf[4]*256+buf[5]));
		
		// Regitster einträge in Daten(rad/s) umwandeln
		gyro[0]= (rawGyro[0] - offset_gyrox)/16.4f/180*PI;
		gyro[1]= (rawGyro[1] - offset_gyroy)/16.4f/180*PI;
		gyro[2]= (rawGyro[2] - offset_gyroz)/16.4f/180*PI; 
	}


	void get_rawgyro(signed int data[3]){									// Gets raw Gyro Data
		data[0]= rawGyro[0];
		data[1]= rawGyro[1];
		data[2]= rawGyro[2]; 
	}

	
	void updateMag(){
		unsigned char buf[6];
		I2C_mag.read_bytes(3, buf,6);
		
		rawMag[0]= ((signed short)(buf[0]*256+buf[1]));
		rawMag[1]= ((signed short)(buf[2]*256+buf[3]));
		rawMag[2]= ((signed short)(buf[4]*256+buf[5])); 
		
		// Convert Regitster entrys in usable Data (Gauss) 
		mag[0]= (rawMag[0])/660.0 ;
		mag[2]= (rawMag[1])/660.0 ;
		mag[1]= (rawMag[2])/660.0 ; 
	}

	void get_rawmag(int data[3]){
		data[0]= rawMag[0];
		data[1]= rawMag[1];
		data[2]= rawMag[2]; 
	}


	void getGyroOffset(){
		int i;
		long sum[3];
		int gyro[3];
		sum[0]=0;
		sum[1]=0;
		sum[2]=0;
		for(i=0;i<500;i++){
			updateGyro();
			get_rawgyro(gyro);
			sum[0]+=gyro[0];
			sum[1]+=gyro[1];
			sum[2]+=gyro[2];
		}
		offset_gyrox=sum[0]/500.0f;
		offset_gyroy=sum[1]/500.0f;
		offset_gyroz=sum[2]/500.0f;
		printf("Gyro Offsets: X=%.4f y=%.4f Z=%.4f \n",offset_gyrox,offset_gyroy,offset_gyroz);
	}

	
	void getAccOffset(){  
		int i;
		long sum[3];
		int acc[3];
		sum[0]=0;
		sum[1]=0;
		sum[2]=0;
		for(i=0;i<500;i++){
			updateAcc();
			get_rawacc(acc);
			sum[0]+=acc[0];
			sum[1]+=acc[1];
			sum[2]+=(acc[2]-2048);
		}
		offset_accx=sum[0]/500.0f;
		offset_accy=sum[1]/500.0f;
		offset_accz=sum[2]/500.0f;
		
		printf("Offsets: X=%.4f y=%.4f Z=%.4f \n",offset_accx,offset_accy,offset_accz);
	}
	
	void update(){
		updateGps();
		updateAcc();
		updateGyro();
	}
	
	
	Sensors(int wait){ //initalize the Sensors
		gpsBufferIndex=0;
	
		I2C_imu=I2C(0x68); //MPU6050
		I2C_mag=I2C(0x1e); //Magnetometer
		gpsfd = serialOpen("/dev/ttyAMA0",38400); //GPS
	
		offset_accx = -7.3400;
		offset_accy = -7.9360;
		offset_accz = 373.1860;
		offset_gyrox = -58;
		offset_gyroy = 32;
		offset_gyroz = 11;
		
		// reset device
		I2C_imu.write_reg( 107 , 0x80);
		
		sleep(wait);
		
		// Disable sleep mode and set  gyro as clock source
		I2C_imu.write_reg( 107 , 0x01);

		// Set full scale range of gyroscopes
		I2C_imu.write_reg( 27 , 0x18);
		I2C_imu.write_reg( 28 , 0x18);

		// Enable data ready interrupt:
		I2C_imu.write_reg( 56 , 0x01);
		
		//enable Passtrough Mode
		I2C_imu.write_reg( 55 , 0x02);	
		
		// Wait one Second to let the Gyros stabilize	
		sleep(wait); 
		
		if (wait){ //nur wenn Quadrocopter (gerade und) ruhig steht!!! TODO Überprüfen ob gerade und ruhig
			getGyroOffset(); 
			//getAccOffset();
		}
		
		//enable FIFO (not implemented)
		

		//Magnetometer Settings
		I2C_mag.write_reg( 0, 0x78); // 0111 1000 (8 measures avareged,highest sample rate evrything else normal)
		I2C_mag.write_reg( 1, 0x60); // 0011 0000 (gain; LSb/Gauss = 660)
		I2C_mag.write_reg( 2, 0x00); // 0000 0000 (continues measurement, no high speed I2C)
	}
	Sensors(){}
};