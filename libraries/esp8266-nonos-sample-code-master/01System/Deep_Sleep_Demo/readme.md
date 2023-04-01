##SDK Version: esp8266_nonos_sdk_v2.0.0_16_07_19
##Platform: ESP-LAUNCHER BOARD

##Purpose:
Test ESP8266 chip current under deep sleep mode.

##Procedure:
1.Connect GPIO16 to EXT_RSTB on hardware.

2.Please configure the ssid and the password of the router you want to connect to in user_main.c.

	os_sprintf(stationConf.ssid, "TP-LINK-FD");
	os_sprintf(stationConf.password, "aaaaaaaa");

3.Copy the examples next to bin/ folder in the SDK folder. The SDK folder should have folders inside it like : bin, examples, third party...Enter example folder, select the example you want to , run ./gen_misc.sh, and follow the tips and steps.

4.Select 1,1,2,0,2 during compiling step 1 to 5. Then bin files will generate in BIN_PATH folder which is bin/upgrade.

5.Download bin files to ESP-LAUNCHER as below sittings.

	Download address of each bin files
	blank.bin						0xFE000
	esp_init_data_default.bin		0xFC000
	boot_v1.6.bin					0x00000
	user1.2048.new.2.bin			0x01000
	
	Flash download tool settings.
	CrystalFreq: 26M
	SPI SPEED: 40MHz
	SPID MODE: QIO
	FLASH SIZE: 8Mbit

##Result:
1.Connect to the router your configure.

2.After 10 second, chip will go to deep sleep mode.

3.After 10 second, chip will reset and wake up from deep sleep mode, and connect to router again.
 
##Uart0 Log:

	ets Jan  8 2013,rst cause:2, boot mode:(3,2)