#!/bin/sh

cat <<EOF | xmenu | sh &
Applications
	Web Browser | super + s	$BROWSER
	Torrent client	transmission-gtk
	pcmanfm 	pcmanfm
	ranger		$TERMINAL ranger
Terminal emulator 		$TERMINAL
dmenu			dmenu_run

Shutdown		poweroff
Reboot			reboot
EOF
