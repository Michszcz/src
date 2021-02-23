#include <X11/XF86keysym.h>
/* appearance */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 6;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "terminus-font:size=11" }; 
static const char dmenufont[]       = "terminus-font:size=16";
static char normfg[]      = "#b8b8b8";
static char normbg[]      = "#000000";
static char normborder[]  = "#141414";
static char selfg[]       = "#b8b8b8";
static char selbg[]       = "#141414";
static char selborder[]   = "#b8b8b8";
static char hidfg[]       = "#727272";
static char hidbg[]       = "#000000";
static char hidborder[]   = "#000000";

static char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfg, normbg, normborder },
	[SchemeSel]  = { selfg, selbg,  selborder  },
	[SchemeHid]  = { hidfg,  hidbg, hidborder  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "st-256color",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[t]",      tile },    /* first entry is default */
	{ "[f]",      NULL },    /* no layout function means floating behavior */
	{ "[m]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-p", "run:", NULL };
static const char *layoutmenu_cmd = "xmenulayout";

ResourcePref resources[] = {
		{ "normbg",        STRING,  &normbg },
		{ "normborder",    STRING,  &normborder },
		{ "normfg",        STRING,  &normfg },
		{ "selbg",         STRING,  &selbg },
		{ "selborder",     STRING,  &selborder },
		{ "selfg",         STRING,  &selfg },
		{ "hidbg",         STRING,  &hidbg },
		{ "hidborder",     STRING,  &hidborder },
		{ "hidfg",         STRING,  &hidfg },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",       	FLOAT,   &mfact },
};


static Key keys[] = {
	/* modifier                     key		funnction		argument */
	{ MODKEY,                       XK_d,  	        spawn,                  SHCMD("dmenu_run -p 'run:'") },
	{ MODKEY|ShiftMask,             XK_d,  	        spawn,                  SHCMD("xmenuprograms") },
        { MODKEY,    		        XK_a,  	        spawn,                  SHCMD("$TERMINAL") },
        { MODKEY,    		        XK_s,  	        spawn,                  SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,             XK_s,  	        spawn,                  SHCMD("$TERMINAL newsboat") },
	{ MODKEY|ShiftMask,             XK_f,  	        spawn,                  SHCMD("anki") },
	{ MODKEY|ShiftMask,             XK_a,  	        spawn,                  SHCMD("$TERMINAL ranger") },
	{ MODKEY|ControlMask,           XK_a,  	        spawn,                  SHCMD("pcmanfm") },
	{ MODKEY,	                XK_f,  	        spawn,                  SHCMD("$TERMINAL ncmpcpp") },
	{ MODKEY,	                XK_o,  	        spawn,                  SHCMD("takescreenshot") },
	{ MODKEY|ShiftMask,	        XK_o,  	        spawn,                  SHCMD("takescreenshot copyarea") },
	{ MODKEY,	                XK_p,  	        spawn,                  SHCMD("displayselect") },
	{ MODKEY|ShiftMask, 	        XK_p,  	        spawn,                  SHCMD("pkill picom || picom --experimental-backends  -f --backend glx  --blur-method dual_kawase --blur-background-exclude 'class_g = \"slop\"' --blur-strength 2 -I 0.12 -O 0.07") },


	{ MODKEY,		 	XK_r,      	spawn,       		SHCMD("light -s sysfs/backlight/intel_backlight -A 10") },
	{ MODKEY,		 	XK_e,      	spawn, 		        SHCMD("light -s sysfs/backlight/intel_backlight -U 10") },
	{ MODKEY|ShiftMask,	 	XK_r,      	spawn,        		SHCMD("light -s sysfs/backlight/intel_backlight -A 1") },
	{ MODKEY|ShiftMask,	 	XK_e,      	spawn,        		SHCMD("light -s sysfs/backlight/intel_backlight -U 1") },
	{ MODKEY|ControlMask,	 	XK_r,      	spawn,        		SHCMD("light -s sysfs/backlight/intel_backlight -A 0.1") },
	{ MODKEY|ControlMask,	 	XK_e,      	spawn,    		SHCMD("light -s sysfs/backlight/intel_backlight -U 0.1") },
	{ MODKEY|Mod1Mask,	 	XK_r,      	spawn,   		SHCMD("setbrightness up 10") },
	{ MODKEY|Mod1Mask,	 	XK_e,      	spawn,        		SHCMD("setbrightness down 10") },

	{ ControlMask, 			XK_Right,     	spawn,			SHCMD("volumecontrol up") },
	{ ControlMask,    		XK_Left,    	spawn,                  SHCMD("volumecontrol down") },
	{ ControlMask,		        XK_Down, 	spawn,                  SHCMD("volumecontrol mute") },
	{ ControlMask|ShiftMask,        XK_Down,      	spawn,                  SHCMD("$TERMINAL pulsemixer") },

	{ ControlMask,	       		XK_Up,         	spawn,                  SHCMD("songcontrol toggle") },
	{ ControlMask|ShiftMask,        XK_Left,	spawn,                  SHCMD("songcontrol prev") },
	{ ControlMask|ShiftMask,	XK_Right,      	spawn,                  SHCMD("songcontrol next") },

	{ 0,	       			XF86XK_AudioPlay, spawn,                SHCMD("songcontrol toggle") },
	{ 0,			        XF86XK_AudioPrev, spawn,                SHCMD("songcontrol prev") },
	{ 0,				XF86XK_AudioNext, spawn,                SHCMD("songcontrol next") },
	{ 0,				XF86XK_AudioStop, spawn,                SHCMD("songcontrol stop") },
	{ 0, 				XF86XK_AudioRaiseVolume,	 spawn,		SHCMD("volumecontrol up") },
	{ 0,    			XF86XK_AudioLowerVolume, 	 spawn,		SHCMD("volumecontrol down") },
	{ 0,		 	        XF86XK_AudioMute,      	 	 spawn,		SHCMD("volumecontrol mute") },
	{ 0,		 	        XF86XK_AudioMicMute,      	 spawn, 	SHCMD("volumecontrol mutemic") }, 
	{ 0,		 	        XF86XK_MonBrightnessUp,      	 spawn,	        SHCMD("light -s sysfs/backlight/intel_backlight -A 10") },
	{ 0,		 	        XF86XK_MonBrightnessDown,      	 spawn,	        SHCMD("light -s sysfs/backlight/intel_backlight -U 10") },
	{ 0,		 	        XF86XK_ScreenSaver,      	 spawn,	        SHCMD("slock") },
	{ 0,		 	        XF86XK_Display,      		 spawn,	        SHCMD("displayselect") },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstackvis,  {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstackvis,  {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      focusstackhid,  {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      focusstackhid,  {.i = -1 } },
	{ MODKEY,                       XK_w,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_q,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY, 	         	XK_z,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,           		XK_space,  togglefloating, {0} },
	{ MODKEY,		        XK_x,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_v,      show,           {0} },
	{ MODKEY,                       XK_c,      hide,           {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        layoutmenu,     {0} },

	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        killclient,     {0} }, 
	{ ClkWinTitle,          0,              Button3,        zoom,           {0} }, 

/*	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} }, */
/*	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} }, */
/*	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} }, */
/*	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} }, */
/*	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} }, */

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },

	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            0,       	Button2,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button2,        toggletag,      {0} },
};


/*static const char *browser[] 	= { "chromium", NULL };
static const char *steam[] 	= { "steam", NULL };
static const char *pcmanfm[] 	= { "pcmanfm", NULL };
static const char *rss[]  	= { term, "newsboat", NULL };
static const char *ncmpcpp[]  	= { term, "ncmpcpp", NULL };
static const char *ranger[]  	= { term, "ranger", NULL };
static const char *pulsemixer[]	= { term, "pulsemixer", NULL };
static const char *displayselect[]	= { "displayselect", NULL };

static const char *volup[] 	= { "volumecontrol", "up", NULL};
static const char *voldown[]  	= { "volumecontrol", "down", NULL};
static const char *volmute[]  	= { "volumecontrol", "mute", NULL,};
static const char *micmute[]  	= { "volumecontrol", "mutemic", NULL};
static const char *backlightupmajor[]  	= { "light", "-s", "sysfs/backlight/intel_backlight", "-A", "10", NULL };
static const char *backlightdownmajor[]	= { "light", "-s", "sysfs/backlight/intel_backlight", "-U", "10", NULL };
static const char *backlightupminor[]  	= { "light", "-s", "sysfs/backlight/intel_backlight", "-A", "1", NULL };
static const char *backlightdownminor[] = { "light", "-s", "sysfs/backlight/intel_backlight", "-U", "1", NULL };
static const char *backlightupfine[]  	= { "light", "-s", "sysfs/backlight/intel_backlight", "-A", "0.1", NULL };
static const char *backlightdownfine[]	= { "light", "-s", "sysfs/backlight/intel_backlight", "-U", "0.1", NULL };
static const char *setbrightnessup[]	= { "setbrightness", "up", "10", NULL };
static const char *setbrightnessdown[]	= { "setbrightness", "down", "10", NULL };

static const char *play_pause[]	= { "songcontrol", "toggle", NULL };
static const char *prev_song[]  = { "songcontrol", "prev", NULL };
static const char *next_song[]  = { "songcontrol", "next", NULL };

static const char *xmenuprograms[] 	= { "xmenuprograms", NULL };  */

/*static const char *termcmd[] 	= { "st", NULL }; */
//static const char term[]  	= "st";
