#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

#include "libs/inih/cpp/INIReader.h"
#include "datastruct.h"

constexpr auto FORMAT = "mp3";
constexpr auto STORAGE = "/pirateradio";
constexpr auto SETTINGS = "/pirateradio/pirateradio.config";
constexpr auto DEFAULTFREQ = "107.0";
constexpr auto DEFAULTGAIN = "1.3";
constexpr auto DEFAULTBTGAIN = "1.7";
constexpr auto DEFAULTOUTPUT = "fm";
constexpr auto DEFAULTSTATIONNAME = "MPRADIO";
constexpr bool BTBOOST = true;
constexpr int STRLEN = 40;

settings s;

INIReader getReader()
{
	INIReader reader(SETTINGS);
	if (reader.ParseError() < 0) {
		cout << "Can't load config file, setting to defaults...\n";
	}

	return reader;
}

void getsettings()
{
	INIReader reader = getReader();
	s.freq=reader.Get("PIRATERADIO","frequency",DEFAULTFREQ);
	s.btGain=reader.Get("PIRATERADIO","btGain",DEFAULTBTGAIN);
	s.storageGain=reader.Get("PIRATERADIO","storageGain",DEFAULTGAIN);

	s.persistentPlaylist=reader.GetBoolean("PLAYLIST","persistentPlaylist",true);
	s.resumePlayback=reader.GetBoolean("PLAYLIST","resumePlayback",true);
	s.shuffle=reader.GetBoolean("PLAYLIST","shuffle",true);

	s.rdsUpdateInterval=reader.GetInteger("RDS","updateInterval",4);
	s.rdsCharsJump=reader.GetInteger("RDS","charsJump",6);
	s.rdsStationName=reader.Get("RDS","stationName",DEFAULTSTATIONNAME);

	s.storage=STORAGE;
	s.format=reader.Get("PLAYLIST","fileFormat",FORMAT);
	if(s.format=="all") s.format="mp3 -o -iname *.flac -o -iname *.wav";

	s.output=reader.Get("PIRATERADIO","output",DEFAULTOUTPUT);
	s.btBoost=reader.GetBoolean("PIRATERADIO","btBoost",BTBOOST);

	s.implementation="pi_fm_adv";
	s.implementation=reader.Get("PIRATERADIO","implementation","pi_fm_rds");
	if(s.implementation=="pi_fm_adv")
		s.opSwitch="--";
	else
		s.opSwitch="-";

	s.treble = reader.Get("PIRATERADIO","treble","0");

	cout<<"freq: "<<s.freq<<endl<<"btGain: "<<s.btGain<<" storageGain: "<<s.storageGain \
		<<endl<<"persistentPlaylist: "<<s.persistentPlaylist<<endl \
		<<"resumePlayback: "<<s.resumePlayback<<endl<<"shuffle: "<<s.shuffle<<endl \
		<<"rdsStationName: "<<s.rdsStationName<<endl \
		<<"rdsUpdateInterval: "<<s.rdsUpdateInterval<<endl<<"rdsCharsJump: " \
		<<s.rdsCharsJump<<endl<<"Output: "<<s.output<<endl<<"Format: "\
		<<s.format<<endl<<"implementation: "<<s.implementation<<endl<<"Treble: "<<s.treble<<endl;
}
