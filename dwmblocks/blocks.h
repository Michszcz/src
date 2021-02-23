//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"", 		"bar-mpd", 		0,			10},
	{"TEMP:", 	"gettemp",		4,			1},
	{"RAM:", 	"free --mega | grep Mem | awk '{print $3}' |tr -d '\n' ; printf 'M'", 	6,			1},
	{"VOL:", 	"bar-volume", 		0,			10},
	{"BAT:",	"getbattery",		60,			1},
	{"", 		"date +'%a %d/%m %R'",	15,			1},
};

//Sets delimiter between status commands. NULL character ('\0') means no delimiter.
static char delim[] = " ";
static unsigned int delimLen = 5;
