#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>

#define BYTE unsigned char

#define GPIO17 (0)		// BC1
#define GPIO4  (7)		// BDIR

#define GPIO22 (3)		// DATA
#define GPIO23 (4)		// LATCH(データピンに出力する)
#define GPIO24 (5)		// CLK

#define HC595_DATA	GPIO22
#define HC595_LATCH	GPIO23
#define HC595_CLK	GPIO24

#define AYBC1	GPIO17
#define AYBCDIR	GPIO4

void setPort()
{
	// AY
	pinMode( GPIO4, OUTPUT);
	pinMode(GPIO17, OUTPUT);
	// 74HC595
	pinMode(GPIO22, OUTPUT);
	pinMode(GPIO23, OUTPUT);
	pinMode(GPIO24, OUTPUT);

	// ALL 0
	delay(100);
	digitalWrite( GPIO4, 0);
	digitalWrite(GPIO17, 0);
	digitalWrite(GPIO22, 0);
	digitalWrite(GPIO23, 0);
	digitalWrite(GPIO24, 0);
	delay(100);
}

void HC595_out(int data)
{
	int	i;
	int	b;

	digitalWrite(HC595_CLK, 0);
	digitalWrite(HC595_LATCH, 0);

	//printf("%02x:", data);

	for (i = 0; i < 8; i++) {
		b = data & 0x80;		// 1000_0000
		b = b >> 7;
		data = data << 1;
		//printf("%d ", b);

		digitalWrite(HC595_DATA, b);
		digitalWrite(HC595_CLK, 1);
		digitalWrite(HC595_CLK, 0);
	}
	//printf("\n");
	digitalWrite(HC595_LATCH, 1);
	digitalWrite(HC595_LATCH, 0);
}

void AY_address()
{
	//digitalWrite(AYBC1, HIGH);
	//digitalWrite(AYBCDIR, HIGH);
	digitalWriteByte(0x81);		// 1000_0001
}

void AY_write()
{
	//digitalWrite(AYBC1, LOW);
	//digitalWrite(AYBCDIR, HIGH);
	digitalWriteByte(0x80);		// 1000_0000
}

void AY_inactive()
{
	//digitalWrite(AYBC1, LOW);
	//digitalWrite(AYBCDIR, LOW);
	digitalWriteByte(0x00);		// 0000_0000
}

void AY_out(int reg, int data)
{
	// address
	HC595_out(reg);
	AY_address();
	AY_inactive();

	// data
	HC595_out(data);
	AY_write();
	AY_inactive();
	// 必要ないかも?
	usleep(1);
}

int main(void)
{
	int	i;

	if (wiringPiSetup() == -1) {
		printf("setup error\n");
		return 1;
	}
	printf("setup OK\n");
	setPort();
	delay(100);

	AY_out(7,0x3e);
	AY_out(6,0);
	AY_out(8,0);
	AY_out(0,125);
	AY_out(1,0);

	for (i = 0; i < 0x10; i++) {
		delay(100);
		AY_out(8,i);
	}
	for (i = 0x0f; i >= 0; i--) {
		delay(100);
		AY_out(8,i);
	}

	return 0;
}
