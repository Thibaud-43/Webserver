#!/bin/sh

SERVER=/etc/nginx/sites-available/ftserver
ACTIVE=`cat $SERVER | grep autoindex | cut -d' ' -f2 | cut -d';' -f1`

if [ "$#" -ne 1 ]
then
	echo "Invalid number of arguments."
	exit 1
fi

if [ "$1" = "on" ] || [ "$1" = "off" ]
then
	if [ "$1" = "on" ]
	then
		if [ "$ACTIVE" = "on" ]
		then
			echo "Autoindex is already active."
		else
			sed -i "s/autoindex off/autoindex on/" $SERVER
			echo "Autoindex has been activate. Reloading Nginx..."
			service nginx reload
		fi
	fi
	if [ "$1" = "off" ]
	then
		if [ "$ACTIVE" = "off" ]
		then
			echo "Autoindex is already disabled."
		else
			sed -i "s/autoindex on/autoindex off/" $SERVER
			echo "Autoindex has been desactivate. Reloading Nginx..."
			service nginx reload
		fi
	fi
else
	echo "Argument must be on or off."
	exit 1
fi
