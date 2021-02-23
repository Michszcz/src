/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nobody";

static const char *colorname[NUMCOLS] = {
	[INIT] =   "#000000",     /* after initialization */
	[INPUT] =  "#141414",   /* during input */
	[FAILED] = "#8a0000",   /* wrong password */
	[CAPS] = "#FFFFFF",         /* CapsLock on */
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 0;

/* default message */
static const char * message = "";

/* text color */
static const char * text_color = "#b8b8b8";

/* text size (must be a valid size) */
static const char * font_name = "-xos4-terminus-medium-r-normal--22-220-72-72-c-110-iso10646-1";
